/*
In this exercise, you should use mmap system call to a big text file
text.txt in chunks.
This exercise requires generating a relatively large file of size
500MiB.

The content of the text file text.txt should be generated from
“/dev/random”. The file “/dev/random” is a special character file
that generates pseudorandom numbers.

Write the program ex2.c to perform the following tasks using
memory mapping.

Create the empty file text.txt. Open the file “/dev/random”, and
read a character c at a time.

If the generated character c is a printable character (use isprint
function from ctype.h to check it), then we should add it to the file
text.txt. Otherwise, we ignore the character and generate a new
character.

You need to add a new line to text.txt after adding 1024
characters (max line length). You should continue adding
characters till you get a file of size 500 MiB= 500*1024KiB.
After finishing the process above, you would get a relatively large
file in your file system.

Open the whole file text.txt in chunks where the chunk size is
1024th multiple of the page size in your system. You can get the
page size in C as follows: (if page size is 4KiB then chunk size is
4MiB)

#include <unistd.h>
long sz = sysconf(_SC_PAGESIZE);

Count the capital letters in the mapped chuncks. Print the total
number of the capital letters in the file to stdout.
Replace the capital letters with lowercase letters in the file.
unmap the mapped memory.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILE_SIZE 500*1024*1024

int main() {
    int fd = open("text.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, FILE_SIZE);

    char *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        printf("Error mapping file!\n");
        exit(EXIT_FAILURE);
    }

    char *random = "/dev/random";
    int random_fd = open(random, O_RDONLY);
    if (random_fd < 0) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    char c;
    int i = 0;
    while (i < FILE_SIZE) {
        read(random_fd, &c, 1);
        if (isprint(c)) {
            map[i] = c;
            i++;
        }
    }

    long cs = sysconf(_SC_PAGESIZE);
    int count = 0;
    for (int i = 0; i < FILE_SIZE; i += cs) {
        for (int j = i; j < i + cs; j++) {
            if (isupper(map[j])) {
                count++;
                map[j] = tolower(map[j]);
            }
        }
    }

    printf("Number of capital letters: %d\n", count);

    munmap(map, FILE_SIZE);
    close(fd);
    close(random_fd);

    return 0;
}




