#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void animate() {
    char *f1 = "       \n   .   \n       ";
    char *f2 = "       \n  .-.  \n       ";
    char *f3 = "  ...  \n  | |  \n  '''  ";
    char *f4 = "  .-.  \n  | |  \n  '-'  ";
    char *f5 = "  /-\\  \n | O | \n  \\-/  ";
    char *f6 = "  .-.  \n  | |  \n  '-'  ";
    char *f7 = "  ...  \n  | |  \n  '''  ";
    char *f8 = "       \n  .-.  \n       ";
    char *f9 = "       \n   .   \n       ";
    char *frames[] = {f1, f2, f3, f4, f5, f6, f7, f8, f9};

    system("clear");
    printf("\033[?25l"); // clears curs
    for (int i = 0; i < 90; i++) {
        printf("%s", frames[i % 9]);
        fflush(stdout);
        usleep(200000);
        printf("\033[H"); // resets curs to top of screen
    }
    printf("\n");
    printf("\033[?25h"); // puts curs back    
}


