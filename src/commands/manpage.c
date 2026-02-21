#include "manpage.h"
const char* defn[] = {
    "definition",
    "does grep things"
    "touches things"
};
const char* cmd[] = {
    "command",
    "grep"
    "touch"
};

void request_manpage(char* query) {
    if (!query || strlen(query) == 0) {
        printf("\nHow to use the man function:\n");
        printf("\t\n\"man [command]\"\n\n");
        printf("Where the manual will return the manual based on the \"command\" given.\n");
        printf("Example: \"man grep\" will provide the manual for the \"grep\" command\n\n");
        return;
    }
    for (int i = 0; i < (sizeof(cmd) / sizeof(cmd[0])); i++) {
        if (strncmp(query, cmd[i], strlen(cmd[i])) == 0) {
            printf("%s\n", defn[i]);
            return;
        }
    }
    printf("man command \"%s\" unknown\n", query);
}

char* get_description(char* query){
    return NULL;
}

int add_description(char* query, char* description){
    return 0;
}

int remove_description(char* query){
    return 0;
}

// As I am currently unsure how to implement opening the gui, this will be in its place
//int open_man_gui();