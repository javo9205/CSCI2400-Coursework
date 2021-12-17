//-*-c++-*-
#ifndef _helper_routines_h_
#define _helper_routines_h_

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);
void usage(void);
void unix_error(const char *msg);
void app_error(const char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);


pid_t Fork(void);
size_t sio_strlen(char s[]);
void sio_reverse(char s[]);
void sio_ltoa(long v, char s[], int b);
ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);
void sio_error(char s[]);

#endif
