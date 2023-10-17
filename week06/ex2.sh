gcc scheduler.c -o scheduler
gcc worker.c -o worker

./scheduler

rm scheduler
rm worker
