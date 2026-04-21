#include "manpage.h"
#include <errno.h>

FILE* get_manpage_file(char* query){
    char fileloc[strlen(query)+strlen("descriptions/")+1];
    strcpy(fileloc, "descriptions/");
    strcat(fileloc, query);

    return fopen(fileloc, "r");
}

int create_new_description(char* query, char* description){
    FILE* existing_file = get_manpage_file(query);
    if (existing_file) return -1;
    
    char new_file[strlen(query)+strlen("descriptions/")+1];
    strcpy(new_file, "descriptions/");
    strcat(new_file, query);
    FILE* fileloc = fopen(new_file, "w");
    fwrite(description, sizeof(char), strlen(description), fileloc);
    fclose(fileloc);
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

/*
int delete_description(char* query){
    FILE* del_file = get_manpage_file(query);
    printf("1\n");
    // Checking if the file exists first
    if (!del_file) return -1; 
    printf("2\n");

    // Getting the file destination
    printf("3\n");

    char filedest[strlen(query)+strlen("descriptions/deleted/")+1];
    strcpy(filedest, "descriptions/deleted/");
    strcat(filedest, query);
    printf("4\n");

    // Duplicate the file into deleted and delete the current file
    FILE* del_file_binloc = fopen(filedest, "w");
    int bufferSize = 5;
    char buffer[bufferSize];
    printf("5\n");

    while ((fread(buffer, sizeof(char), bufferSize-1, del_file) > 0)) {
        fwrite(buffer, sizeof(char), bufferSize-1, del_file_binloc);
    }

    printf("6\n");
    fclose(del_file);
    remove(del_file);
    fclose(del_file_binloc);
    printf("7\n");

    return 0;
}
    */
