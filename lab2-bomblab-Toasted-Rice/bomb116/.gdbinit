shell if test ! -f .crisis_counter; then echo -n -e \\x00\\x00\\x00\\x00 > .crisis_counter; fi
shell if test ! -f solution.txt; then touch solution.txt; fi

break phase_1
break phase_2
break phase_3
break phase_4
break phase_5
break phase_6
break secret_phase
break main


break explode_bomb
commands

set $i = $rsp
restore .crisis_counter binary $i
set {int}$i = {int}$i + 1
dump binary value .crisis_counter {int}$i

printf "\n"
printf " > IMMINENT DETONATION DETECTED\n"
printf " > INITIATING KILL-SWITCH......\n"
printf " > .......CRISIS AVERTED.......\n"
printf " > %d EXPLOSION(S) PREVENTED\n",{int}$i
printf " > REBOOTING SCENARIO..........\n"

run solution.txt
end

define show_jt
set $i=0
while ($i < $arg1)
p/a $arg0 + *($arg0 + 4*$i++)
end
end

layout asm
run solution.txt
continue
