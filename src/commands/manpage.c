#include "manpage.h"
const char* defn[] = {
    "definition 1"
};
const char* cmd[] = {
    "command 1"
};

void request_manpage(char* query) {
    if (!query) {
        printf("\tman [command]\n");
        return;
    }
    for (int i = 0; i < (sizeof(cmd) / sizeof(cmd[0])); i++) {
        if (strncmp(query, cmd[i], strlen(cmd[i])) == 0) {
            printf("%s\n", defn[i]);
            return;
        }
    }
    printf("man command %s unknown\n", query);
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