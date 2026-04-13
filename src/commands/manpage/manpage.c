#include "manpage.h"

const char* defn[] = {
    "Function \"grep\" searches through files\n",
    "touches things"
};
const char* cmd[] = {
    "grep",
    "touch"
};



FILE* get_manpage_file(char* query){
    /*
    for (int i = 0; i < (sizeof(cmd) / sizeof(cmd[0])); i++) {
        if (strncmp(query, cmd[i], strlen(cmd[i])) == 0) {
            return defn[i];
        }
    }
        */
    char fileloc[strlen(query)+strlen("descriptions/")+1];
    strcpy(fileloc, "descriptions/");
    strcat(fileloc, query);

    return fopen(fileloc, "r");
}

int create_new_description(char* query, char* description){
    return 0;
}


int print_description(char* query) {

    // Obtaining the file
    FILE* description = get_manpage_file(query);
    if (!description) {
        printf("man command \"%s\" unknown\n", query);
        return 0;
    }

    // Printing the file out
    int bufSize = 200;
    char buffer[bufSize];
    printf("--------------------------------------------\n");
    while (fgets(buffer, bufSize-1, description)) {
        printf("%s", buffer);
    }
    printf("\n--------------------------------------------\n");

    // Leaving
    fclose(description);
    return 0;
}

int request_manpage(char* query) {
    // For when there was no query present, it will print how to use the function
    if (!query || strlen(query) == 0) {
        printf("\nHow to use the man function:\n");
        printf("\t\n\"man [command]\"\n\n");
        printf("Where the manual will return the manual based on the \"command\" given.\n");
        printf("Example: \"man grep\" will provide the manual for the \"grep\" command\n\n");
        return 1;
    }
    print_description(query);
    return 0;
}

int delete_description(char* query){
    return 0;
}

// As I am currently unsure how to implement opening the gui, this will be in its place
//int open_man_gui();
