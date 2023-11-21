gcc -pthread ex3.c -o ex3
./ex3 10000000 1 >> ex3_res.txt
./ex3 10000000 2 >> ex3_res.txt
./ex3 10000000 4 >> ex3_res.txt
./ex3 10000000 10 >> ex3_res.txt
./ex3 10000000 100 >> ex3_res.txt
rm ex3