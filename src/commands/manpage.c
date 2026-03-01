#include <stdio.h>
#include "manpage.h"

const char* defn[] = {
    "--------------------------------\nFunction \"grep\" searches through files \n--------------------------------\n",
    "touches things"
};
const char* cmd[] = {
    "grep",
    "touch"
};



const char* get_description(char* query){
    for (int i = 0; i < (sizeof(cmd) / sizeof(cmd[0])); i++) {
        if (strncmp(query, cmd[i], strlen(cmd[i])) == 0) {
            return defn[i];
        }
    }
    return NULL;
}

int add_description(char* query, char* description){
    return 0;
}


int read_description(char* query) {
    const char* description = get_description(query);
    if (!description) {
        printf("man command \"%s\" unknown\n", query);
        return 0;
    }
    printf("%s", description);
    return 0;
}

void request_manpage(char* query) {
    // For when there was no query present, it will print how to use the function
    if (!query || strlen(query) == 0) {
        printf("\nHow to use the man function:\n");
        printf("\t\n\"man [command]\"\n\n");
        printf("Where the manual will return the manual based on the \"command\" given.\n");
        printf("Example: \"man grep\" will provide the manual for the \"grep\" command\n\n");
        return;
    }
    read_description(query);
}

int remove_description(char* query){
    return 0;
}

// As I am currently unsure how to implement opening the gui, this will be in its place
//int open_man_gui();
