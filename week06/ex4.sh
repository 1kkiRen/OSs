gcc scheduler_rr.c -o scheduler_rr
gcc worker.c -o worker

./scheduler_rr

rm scheduler_rr
rm worker
