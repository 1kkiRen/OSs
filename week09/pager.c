#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define PAGE_SIZE 8
#define MAX_PAGES 100
#define MAX_FRAMES 100

// Page table entry
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    pid_t mmu_pid;
    int disk_accesses;
    int page_replacement;
};

// Global variables
int pages;
int frames;
int current_frame = 0;
char *reference_string;
int disk_accesses = 0;
char **disk;
char **ram;
struct PTE *page_table;

void sigusr1_handler();

int my_random(struct PTE* page_table) {
    int victim_page = -1;
    while (1) {
        int random_index = rand() % pages;
        if (page_table[random_index].valid == 1) {
            victim_page = random_index;
            break;
        }
    }
}

int nfu(struct PTE* page_table) {
    int min_counter = INT_MAX;
    int page_to_evict = -1;

    for (int i = 0; i < pages; i++) {
        if (page_table[i].valid) {
            if (page_table[i].referenced < min_counter) {
                min_counter = page_table[i].referenced;
                page_to_evict = i;
            }
        }
    }

    return page_to_evict;
}

int aging(struct PTE* page_table) {
    int min_counter = INT_MAX;
    int page_to_evict = -1;

    for (int i = 0; i < pages; i++) {
        if (page_table[i].valid) {
            page_table[i].referenced = (page_table[i].referenced >> 1) | (page_table[i].referenced << 7);

            if (page_table[i].referenced < min_counter) {
                min_counter = page_table[i].referenced;
                page_to_evict = i;
            }
        }
    }

    return page_to_evict;
}

int main(int argc, char *argv[]) {
    pid_t pid = getpid();
    FILE *pid_file = fopen("/tmp/ex2/pager_pid", "w");   
    fprintf(pid_file, "%d", pid);
    fclose(pid_file);

    printf("Pager pid: %d\n", pid);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <number_of_pages> <number_of_frames> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pages = atoi(argv[1]);
    frames = atoi(argv[2]);
    char *page_replacement = argv[3];
    int replacement_type = -1;

    if (strcmp(page_replacement, "random") == 0) {
        printf("Random page replacement algorithm is selected\n");
        replacement_type = 0;
    } else if (strcmp(page_replacement, "nfu") == 0) {
        printf("NFU page replacement algorithm is selected\n");
        replacement_type = 1;
    } else if (strcmp(page_replacement, "aging") == 0) {
        printf("Aging page replacement algorithm is selected\n");
        replacement_type = 2;
    } else {
        fprintf(stderr, "Error: unknown page replacement algorithm\n");
        exit(EXIT_FAILURE);
    }


    if (pages > MAX_PAGES || frames > MAX_FRAMES) {
        fprintf(stderr, "Error: number of pages or frames exceeds maximum\n");
        exit(EXIT_FAILURE);
    }

    printf("Creating page table\n");
    int page_table_fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT);
    if (page_table_fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Truncating file to size of page table\n");
    ftruncate(page_table_fd, sizeof(struct PTE) * pages);

    printf("Mapping page table to memory\n");
    page_table = mmap(NULL, sizeof(struct PTE) * pages, PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("-------------------------\n");
    printf("Initializing page table\n");
    for (int i = 0; i < pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
        printf("Page %d: valid=%d frame=%d dirty=%d referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }

    page_table->page_replacement = replacement_type;

    printf("Creating RAM\n");
    ram = malloc(sizeof(char *) * frames);
    if (ram == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    printf("Initializing RAM\n");
    for (int i = 0; i < frames; i++) {
        ram[i] = malloc(sizeof(char) * PAGE_SIZE);
        if (ram[i] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        ram[i][0] = '\0';
    }

    printf("-------------------------\n");
    printf("Initialized RAM\nRAM array\n");
    for (int i = 0; i < frames; i++) {
        printf("Frame %d ---> %s\n", i, ram[i]);
    }

    printf("-------------------------\n");
    printf("Initializing disk\n");
    disk = malloc(sizeof(char *) * pages);
    for (int i = 0; i < pages; i++) {
        disk[i] = malloc(sizeof(char) * PAGE_SIZE);
        if (disk[i] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }
    srand(time(NULL));
    for (int i = 0; i < pages; i++) {
        for (int j = 0; j < PAGE_SIZE - 1; j++) {
            disk[i][j] = 'a' + rand() % 26;
        }
        disk[i][PAGE_SIZE - 1] = '\0';
    }

    printf("Disk array\n");
    for (int i = 0; i < pages; i++) {
        printf("Page %d ----> %s\n", i, disk[i]);
    }

    signal(SIGUSR1, sigusr1_handler);
    
    while (1) {
        
    }
}

void sigusr1_handler() {
    int zero_referenced = 0;

    printf("MMU process (pid=%d) is running\n", page_table->mmu_pid);
    for (int i = 0; i < pages; i++) {
        if (page_table[i].referenced != 0) {
            if (current_frame < frames) {
                if (!page_table[i].valid) {
                    printf("A disk access request from MMU Process (pid=%d)\n", page_table->mmu_pid);
                    printf("Page %d is referenced\n", i);

                    page_table[i].referenced = 0;
                    page_table[i].frame = current_frame;
                    page_table[i].valid = 1;

                    printf("We can allocate it to free frame %d\n", page_table[i].frame);
                    printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, page_table[i].frame);
                    
                    ram[page_table[i].frame][0] = '\0';
                    strcpy(ram[current_frame], disk[i]);

                    printf("RAM array\n");
                    for (int k = 0; k < frames; k++) {
                        printf("Frame %d ---> %s\n", k, ram[k]);
                    }

                    disk_accesses++;
                    printf("disk accesses is %d so far\n", disk_accesses);
                    current_frame++;
                    printf("Resume MMU process\n");
                    printf("-------------------------\n");
                    kill(page_table->mmu_pid, SIGCONT);
                }
            } else {
                printf("current_frame is %d\n", current_frame);
                printf("A disk access request from MMU Process (pid=%d)\n", page_table->mmu_pid);

                printf("Page %d is referenced\n", i);
                page_table[i].referenced = 0;

                printf("We do not have free frames in RAM\n");
                printf("We have to evict a page\n");

                int victim_page = -1;
                int page_replace = page_table->page_replacement;
                if (page_replace == 0) {
                    victim_page = my_random(page_table);
                } else if (page_replace == 1) {
                    victim_page = nfu(page_table);
                } else if (page_replace == 2) {
                    victim_page = aging(page_table);
                }


                printf("We choose page %d as a victim page\n", victim_page);
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, page_table[victim_page].frame);
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, page_table[victim_page].frame);
                
                ram[page_table[victim_page].frame][0] = '\0';
                strcpy(ram[page_table[victim_page].frame], disk[i]);

                printf("RAM array\n");
                for (int k = 0; k < frames; k++) {
                    printf("Frame %d ---> %s\n", k, ram[k]);
                }

                if (page_table[victim_page].dirty == 1) {
                    printf("The victim page is dirty\n");
                    printf("Copy data from RAM (frame=%d) to disk (page=%d)\n", page_table[victim_page].frame, victim_page);
                    strcpy(disk[victim_page], ram[page_table[victim_page].frame]);

                    printf("Disk array\n");
                    for (int k = 0; k < pages; k++) {
                        printf("Page %d ---> %s\n", k, disk[k]);
                    }

                    disk_accesses++;
                    printf("disk accesses is %d so far\n", disk_accesses);
                }

                int frame = page_table[victim_page].frame;
                page_table[victim_page].valid = 0;
                page_table[victim_page].frame = -1;
                page_table[victim_page].dirty = 0;
                page_table[victim_page].referenced = 0;

                page_table[i].frame = frame;
                page_table[i].valid = 1;
                page_table[i].dirty = 0;
                page_table[i].referenced = 0;

                printf("Resume MMU process\n");
                printf("-------------------------\n");
                kill(page_table->mmu_pid, SIGCONT);
            }
            disk_accesses++;
        } else {
            zero_referenced++;
        }
    }
    if (zero_referenced == pages) {
        printf("%d disk accesses in total\n", disk_accesses);
        printf("Pager is terminated\n");

        munmap(page_table, sizeof(struct PTE) * pages);

        exit(EXIT_SUCCESS);
    }
}
