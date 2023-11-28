#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <errno.h>

#define E_KEY 0x12
#define X_KEY 0x2d
#define C_KEY 0x2e
#define A_KEY 0x1e
#define P_KEY 0x19
#define R_KEY 0x13
#define B_KEY 0x30

int e_is_holding;
int c_is_holding;
int a_is_holding;
int p_is_holding;
int r_is_holding;

int main(int argc, char *argv[]) {
    struct input_event ev;
    ssize_t n;
    char *event_file = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    char *output_file = "ex1.txt";

    int fd = open(event_file, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Press E+X to exit\n");
    printf("Press P+E to get some information my exam\n");
    printf("Press C+A+P to get some cappuccino\n");
    printf("Press R+A to get some information about Ra\n");

    fprintf(out, "Press E+X to exit\n");
    fprintf(out, "Press P+E to get some information my exam\n");
    fprintf(out, "Press C+A+P to get some cappuccino\n");
    fprintf(out, "Press R+A to get some information about Ra\n");

    while (1) {
        n = read(fd, &ev, sizeof ev);

        if (n == (ssize_t) -1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else if (n != sizeof ev) {
            errno = EIO;
            break;
        }


        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2) {
            if (ev.value == 0) {
                fprintf(out, "RELEASED 0x%04x (%d)\n", ev.code, ev.code);
                printf("RELEASED 0x%04x (%d)\n", ev.code, ev.code);
            } else if (ev.value == 1) {
                fprintf(out, "PRESSED 0x%04x (%d)\n", ev.code, ev.code);
                printf("PRESSED 0x%04x (%d)\n", ev.code, ev.code);
            } else if (ev.value == 2) {
                fprintf(out, "REPEATED 0x%04x (%d)\n", ev.code, ev.code);
                printf("REPEATED 0x%04x (%d)\n", ev.code, ev.code);
            }
        }


        if (ev.code == E_KEY && (ev.value == 1 || ev.value == 2)) {
            e_is_holding = 1;
        } else if (ev.code == X_KEY && (ev.value == 1 || ev.value == 2) && e_is_holding == 1) {
            printf("EXIT\n");
            fprintf(out, "EXIT\n");
            break;
        } else if (ev.code == E_KEY && ev.value == 0) {
            e_is_holding = 0;
        }

        if (ev.code == C_KEY && (ev.value == 1 || ev.value == 2)) {
            c_is_holding = 1;
        } else if (ev.code == A_KEY && (ev.value == 1 || ev.value == 2) && c_is_holding == 1) {
            if (p_is_holding == 1) {
                printf("Get some cappuccino!\n");
                fprintf(out, "Get some cappuccino!\n");
                p_is_holding = 0;
            } else {
                p_is_holding = 1;
            }
        } else if (ev.code == C_KEY && ev.value == 0) {
            c_is_holding = 0;
        }

        if (ev.code == P_KEY && (ev.value == 1 || ev.value == 2)) {
            p_is_holding = 1;
        } else if (ev.code == E_KEY && (ev.value == 1 || ev.value == 2) && p_is_holding == 1) {
            printf("I passed the Exam!\n");
            fprintf(out, "I passed the Exam!\n");
        } else if (ev.code == P_KEY && ev.value == 0) {
            p_is_holding = 0;
        }        

        if (ev.code == R_KEY && (ev.value == 1 || ev.value == 2)) {
            r_is_holding = 1;
        } else if (ev.code == A_KEY && (ev.value == 1 || ev.value == 2) && r_is_holding == 1) {
            printf("Ra (also given as Re) is the sun god of ancient Egypt. He is one of the oldest deities in the Egyptian pantheon and was later merged with others such as Horus, becoming Ra-Horakhty (the morning sun), Amun (as noonday sun), and Atum (the evening sun) associated with primal life-giving energy.\n");
            fprintf(out, "Ra (also given as Re) is the sun god of ancient Egypt. He is one of the oldest deities in the Egyptian pantheon and was later merged with others such as Horus, becoming Ra-Horakhty (the morning sun), Amun (as noonday sun), and Atum (the evening sun) associated with primal life-giving energy.\n");
        } else if (ev.code == R_KEY && ev.value == 0) {
            r_is_holding = 0;
        }

    }

    fclose(out);
    close(fd);

    return 0;
}
