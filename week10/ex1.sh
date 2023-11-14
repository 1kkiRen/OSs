gcc monitor.c -o monitor
gcc ex1.c -o ex1

./monitor <path_to_watched_directory>
./ex1 <path_to_watched_directory>

rm monitor
rm ex1