#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define PAGE_SIZE 8

// Page table entry
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    pid_t mmu_pid;
    int disk_accesses;
};

// Global variables
pid_t pid;
int pages;
int current_frame = 0;
int disk_accesses = 0;
struct PTE *page_table;

void cont_handler();
void nonValid_page_handler(int page);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <pages> <reference_string> <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pages = atoi(argv[1]);
    char *reference_string = malloc(sizeof(char) * argc - 3);
    pid = atoi(argv[argc - 1]);

    for (int i = 2; i < argc - 1; i++) {
        strcat(reference_string, argv[i]);
    }

    printf("Reference string: %s\n", reference_string);

    printf("MMU process (pid=%d) is running\n", getpid());
    printf("Pager process (pid=%d) is running\n", pid);

    printf("Opening pagetable\n");
    int page_table_fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (page_table_fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Truncating pagetable\n");
    ftruncate(page_table_fd, sizeof(struct PTE) * pages);

    printf("Mapping pagetable\n");
    page_table = mmap(NULL, sizeof(struct PTE) * pages, PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }   
    page_table->mmu_pid = getpid();

    printf("-------------------------\n");
    printf("Initialized page table\n");
    printf("Page table:\n");
    for (int i = 0; i < pages; i++) {
        printf("Page %d: valid=%d frame=%d dirty=%d referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");

    printf("len of reference string: %ld\n", strlen(reference_string));

    printf("Running through reference string\n");
    for (int i = 0; i < strlen(reference_string); i = i + 2) {
        char mode = reference_string[i];
        int page = reference_string[i + 1] - '0';

        if (!page_table[page].valid) {
            printf("It is not a valid page --> page fault\n");
            nonValid_page_handler(page);
        } else {
            printf("It is a valid page\n");
        }

        if (mode == 'W') {
            printf("Write Request for page %d\n", page);    
            page_table[page].dirty = true;
        } else {
            printf("Read Request for page %d\n", page);
        }

        printf("Page table\n");
        for (int j = 0; j < pages; j++) {
            printf("Page %d ---> valid=%d frame=%d dirty=%d referenced=%d\n", j, page_table[j].valid, page_table[j].frame, page_table[j].dirty, page_table[j].referenced);
        }
        printf("-------------------------\n");
    }

    printf("Done all requests.\n");

    munmap(page_table, sizeof(struct PTE) * pages);
    close(page_table_fd);

    printf("MMU sends SIGUSR1 to the pager. \nMMU terminates.\n");
    kill(pid, SIGUSR1);

    return 0;
}


void cont_handler() {
    printf("MMU resumed by SIGCONT signal from pager\n");
}

void nonValid_page_handler(int page){
    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");

    page_table[page].referenced = 1;

    kill(pid, SIGUSR1);

    signal(SIGCONT, cont_handler);
    pause();
}