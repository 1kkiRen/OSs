gcc scheduler_sjf.c -o scheduler_sjf
gcc worker.c -o worker

./scheduler_sjf

rm scheduler_sjf
rm worker
