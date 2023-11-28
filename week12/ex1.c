#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <errno.h>

// key codes
#define E_KEY 0x12
#define X_KEY 0x2d
#define C_KEY 0x2e
#define A_KEY 0x1e
#define P_KEY 0x19
#define R_KEY 0x13
#define B_KEY 0x30


// globals for holding keys
int e_is_holding;
int c_is_holding;
int a_is_holding;
int p_is_holding;
int r_is_holding;

int main(int argc, char *argv[]) {
    // input event structure
    struct input_event ev;
    ssize_t n;

    // event file and output file
    char *event_file = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    char *output_file = "ex1.txt";

    // open event file
    int fd = open(event_file, O_RDONLY);

    // check if open was successful, if not exit with error
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // open output file
    FILE *out = fopen(output_file, "w");

    // check if open was successful, if not exit with error
    if (out == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // print instructions
    printf("Press E+X to exit\n");
    printf("Press P+E to get some information my exam\n");
    printf("Press C+A+P to get some cappuccino\n");
    printf("Press R+A to get some information about Ra\n");

    // save instructions to file
    fprintf(out, "Press E+X to exit\n");
    fprintf(out, "Press P+E to get some information my exam\n");
    fprintf(out, "Press C+A+P to get some cappuccino\n");
    fprintf(out, "Press R+A to get some information about Ra\n");

    while (1) {
        // read event
        n = read(fd, &ev, sizeof ev);

        /*
        If n is -1, it checks if the error is EINTR (interrupted system call),
        in which case it continues the loop. For any other error, or if n is
        not equal to the size of ev, it sets errno to EIO (input/output error)
        and breaks the loop.
        */
        if (n == (ssize_t) -1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else if (n != sizeof ev) {
            errno = EIO;
            break;
        }

        // if event is a key event and the value is 0, 1 or 2
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

        // check if E is holding
        if (ev.code == E_KEY && (ev.value == 1 || ev.value == 2)) {
            e_is_holding = 1;
        } else if (ev.code == X_KEY && (ev.value == 1 || ev.value == 2) && e_is_holding == 1) {
            // if E is holding and X is pressed, exit
            printf("EXIT\n");
            fprintf(out, "EXIT\n");
            break;
        } else if (ev.code == E_KEY && ev.value == 0) {
            e_is_holding = 0;
        }

        // check if C is holding
        if (ev.code == C_KEY && (ev.value == 1 || ev.value == 2)) {
            c_is_holding = 1;
        } else if (ev.code == A_KEY && (ev.value == 1 || ev.value == 2) && c_is_holding == 1) {
            // if C is holding and A is pressed, then check if P is holding
            if (p_is_holding == 1) {
                // if P is holding, print the cappuccino message and reset P
                printf("Get some cappuccino!\n");
                fprintf(out, "Get some cappuccino!\n");
                p_is_holding = 0;
            } else {
                p_is_holding = 1;
            }
        } else if (ev.code == C_KEY && ev.value == 0) {
            c_is_holding = 0;
        }

        // check if P is holding
        if (ev.code == P_KEY && (ev.value == 1 || ev.value == 2)) {
            p_is_holding = 1;
        } else if (ev.code == E_KEY && (ev.value == 1 || ev.value == 2) && p_is_holding == 1) {
            // if P is holding and E is pressed, print message
            printf("I passed the Exam!\n");
            fprintf(out, "I passed the Exam!\n");
        } else if (ev.code == P_KEY && ev.value == 0) {
            p_is_holding = 0;
        }        

        // check if R is holding
        if (ev.code == R_KEY && (ev.value == 1 || ev.value == 2)) {
            r_is_holding = 1;
        } else if (ev.code == A_KEY && (ev.value == 1 || ev.value == 2) && r_is_holding == 1) {
            // if R is holding and A is pressed, print the info about Ra
            printf("Ra (also given as Re) is the sun god of ancient Egypt. He is one of the oldest deities in the Egyptian pantheon and was later merged with others such as Horus, becoming Ra-Horakhty (the morning sun), Amun (as noonday sun), and Atum (the evening sun) associated with primal life-giving energy.\n");
            fprintf(out, "Ra (also given as Re) is the sun god of ancient Egypt. He is one of the oldest deities in the Egyptian pantheon and was later merged with others such as Horus, becoming Ra-Horakhty (the morning sun), Amun (as noonday sun), and Atum (the evening sun) associated with primal life-giving energy.\n");
        } else if (ev.code == R_KEY && ev.value == 0) {
            r_is_holding = 0;
        }

    }

    // close files
    fclose(out);
    close(fd);

    return 0;
}
