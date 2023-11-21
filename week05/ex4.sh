gcc -pthread ex4.c -o ex4
./ex4 10000000 1 >> ex4_res.txt
./ex4 10000000 2 >> ex4_res.txt
./ex4 10000000 4 >> ex4_res.txt
./ex4 10000000 10 >> ex4_res.txt
./ex4 10000000 100 >> ex4_res.txt
rm ex4