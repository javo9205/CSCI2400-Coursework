// 
// tsh - A tiny shell program with job control
// 
// James Vogenthaler (javo9205)
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"

//
// Needed global variable definitions
//

static char prompt[] = "tsh> ";
int verbose = 0;
sigset_t mask_all, mask_none;

//
// You need to implement the functions eval, builtin_cmd, do_bgfg,
// waitfg, sigchld_handler, sigstp_handler, sigint_handler
//
// The code below provides the "prototypes" for those functions
// so that earlier code can refer to them. You need to fill in the
// function bodies below.
// 

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

void sio_status(pid_t pid, int sig, char tail_msg[]);

//
// main - The shell's main routine 
//
int main(int argc, char **argv)
{
    int emit_prompt = 1; // emit prompt (default)

    //
    // Redirect stderr to stdout (so that driver will get all output
    // on the pipe connected to stdout)
    //
    dup2(1, 2);

    /* Parse the command line */
    char c;
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':               // print help message
            usage();
            break;
        case 'v':               // emit additional diagnostic info
            verbose = 1;
            break;
        case 'p':               // don't print a prompt
            emit_prompt = 0;    // handy for automatic testing
            break;
        default:
            usage();
        }
    }

    sigemptyset(&mask_none);
    sigemptyset(&mask_all);
    sigaddset(&mask_all, SIGCHLD);
    sigaddset(&mask_all, SIGTSTP);
    sigaddset(&mask_all, SIGINT);

    // Install the signal handlers
    Signal(SIGINT,  sigint_handler);     // ctrl-c
    Signal(SIGTSTP, sigtstp_handler);    // ctrl-z
    Signal(SIGCHLD, sigchld_handler);    // Terminated or stopped child
    Signal(SIGQUIT, sigquit_handler); 
    
    // Initialize the job list
    initjobs(jobs);
    
    // Execute the shell's read/eval loop
    for(;;) {
        // Read command line
        if (emit_prompt) {
            printf("%s", prompt);
            fflush(stdout);
        }

        // Extract command line to buffer
        char cmdline[MAXLINE];
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) 
            app_error("fgets error");
        
        // End of file? (did user type ctrl-d?)
        if (feof(stdin)) {
            fflush(stdout);
            break;
        }

        // Evaluate command line
        eval(cmdline);
        fflush(stdout);
        fflush(stdout);
    } 

    exit(0);
}
    
/////////////////////////////////////////////////////////////////////////////
//
// eval - Evaluate the command line that the user has just typed in
// 
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.    Note:
// each child process must have a unique process group ID so that our
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when we type ctrl-c (ctrl-z) at the keyboard.
//
void eval(char *cmdline) 
{
    char *argv[MAXARGS];
    int bg = parseline(cmdline, argv);
    pid_t pid;

    if (argv[0] == NULL) return;
    
    if (!builtin_cmd(argv)) {
        sigset_t mask_one, mask_prev;
        sigemptyset(&mask_one);
        sigaddset(&mask_one, SIGCHLD);

        sigprocmask(SIG_BLOCK, &mask_one, &mask_prev);
        if ((pid = Fork()) == 0) { // Child Process
            sigprocmask(SIG_SETMASK, &mask_prev, NULL);
            setpgid(0,0);
            if (execve(argv[0], argv, NULL) < 0) {
                fprintf(stderr,"%s: Command not found\n", argv[0]);
                exit(0);
            }
        }

        sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(jobs, pid, 1+(bg>0), cmdline);
        if (bg) sio_status(pid, 0, "");
        else waitfg(pid);
        sigprocmask(SIG_SETMASK, &mask_prev, NULL);


    }

    return;
}


/////////////////////////////////////////////////////////////////////////////
//
// builtin_cmd - If the user has typed a built-in command then execute
// it immediately. The command name would be in argv[0] and
// is a C string. We've cast this to a C++ string type to simplify
// string comparisons; however, the do_bgfg routine will need 
// to use the argv array as well to look for a job number.
//
int builtin_cmd(char **argv) 
{
    if (!strcmp(argv[0],"quit")) exit(0); 
    else if (!strcmp(argv[0], "&")) return 1;
    else if (!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg")) {
        do_bgfg(argv);
        return 2;
    }
    else if (!strcmp(argv[0], "jobs")) {
        listjobs(jobs);
        return 3;
    }
    return 0;         /* not a builtin command */
}

/////////////////////////////////////////////////////////////////////////////
//
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
    pid_t pid=-1;
    sigset_t mask_prev;
    struct job_t *job=NULL;
        
    /* Ignore command if no argument */
    if (argv[1] == NULL) {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }
        
    /* Parse the required PID or %JID arg */
    if (isdigit(argv[1][0])) {
        pid = atoi(argv[1]);
        if (!(job = getjobpid(jobs, pid))) {
            printf("(%d): No such process\n", pid);
            return;
        }
    }
    else if (argv[1][0] == '%') {
        int jid = atoi(&argv[1][1]);
        if (!(job = getjobjid(jobs, jid))) {
            printf("%s: No such job\n", argv[1]);
            return;
        }
        pid = jid2pid(jid);
    }	        
    else {
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        return;
    }

    sigprocmask(SIG_BLOCK, &mask_all, &mask_prev);

    if (pid < 0) pid = -pid;
    kill(-pid, SIGCONT);
    if (!strcmp(argv[0],"fg")) {
        job->state = FG;
        waitfg(pid);
    }
    else {
        job-> state = BG;
        sio_status(pid, 0, "");
    }
    sigprocmask(SIG_SETMASK, &mask_prev, NULL);

    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid)
{
    job_t *job = getjobpid(jobs, pid);
    while (job->pid == pid && job->state == FG) sigsuspend(&mask_none);
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// Signal handlers
//


/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//         a child job terminates (becomes a zombie), or stops because it
//         received a SIGSTOP or SIGTSTP signal. The handler reaps all
//         available zombie children, but doesn't wait for any other
//         currently running children to terminate.    
//
void sigchld_handler(int sig) 
{
    sigset_t mask_prev;
    sigprocmask(SIG_BLOCK, &mask_all, &mask_prev);

    pid_t pid;
    int status;
    int olderrno = errno;

    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        job_t *job = getjobpid(jobs,  pid);
        if (WIFSTOPPED(status)) {
            sio_status(pid, WSTOPSIG(status), "stopped");
            job->state = ST;
            continue;
        }
        else if (WIFSIGNALED(status))
            sio_status(pid, WTERMSIG(status), "terminated");
        deletejob(jobs, pid);
    }

    errno = olderrno;

    sigprocmask(SIG_SETMASK, &mask_prev, NULL);
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//        user types ctrl-c at the keyboard.    Catch it and send it along
//        to the foreground job.    
//
void sigint_handler(int sig) 
{
    pid_t pid = getpgrp();
    pid_t fg = fgpid(jobs);
    if (fg && fg != pid) {
        if (fg > 0) fg = -fg;
        kill(fg, SIGINT);
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//         the user types ctrl-z at the keyboard. Catch it and suspend the
//         foreground job by sending it a SIGTSTP.    
//
void sigtstp_handler(int sig) 
{
    pid_t pid = getpgrp();
    pid_t fg = fgpid(jobs);
    if (fg && fg != pid) {
        if (fg > 0) fg = -fg;
        kill(fg, SIGTSTP);
    }
    return;
}

/*********************
 * End signal handlers
 *********************/

void sio_status(pid_t pid, int sig, char effect[]) {
    job_t *job = getjobpid(jobs, pid);
    if (sig > 0) sio_puts("Job ");
    sio_puts("[");
    sio_putl(job->jid);
    sio_puts("] (");
    sio_putl(job->pid);
    sio_puts(") ");
    if (sig > 0) {
        sio_puts(effect);
        sio_puts(" by signal ");
        sio_putl(sig);
        sio_puts("\n");
    }
    else sio_puts(job->cmdline);
    return;
}