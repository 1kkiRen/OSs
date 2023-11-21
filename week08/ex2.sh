gcc pager.c -o pager
gcc mmu.c -o mmu

pager_pid=$(cat /tmp/ex2/pager_pid)

xterm -e ./pager 4 2
xtrem -e ./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid

rm pager mmu