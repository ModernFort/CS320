#ifndef MANPAGE_H
#define MANPAGE_H
#include <string.h>

#include "manpage.c"

/**
 * When called, this will check for a definition corresponding to query
 * and print the description to stdout.
 * 
 * -If the query is NULL, it will print how to use the command to stdout
 * 
 * -If the query is not found, it will print a message to stdout and return
 * 
 * @param query
 */
void request_manpage(char* query);

/**
 * Go into a file and grab the correct description based on the query.
 * 
 * -This is currently not being used due to the description being stored in the .c file
 * -Also not implemented
 * 
 * @param query
 */
char* get_description(char* query);

/** 
 * Adds a description into the manpage
 * 
 * -Not implemented
 * 
 * @param query
 * @param description
 */
int add_description(char* query, char* description);

/**
 * Removes a description from the manpage
 * 
 * -Not implemented
 * @param query
 */
int remove_description(char* query);

/**
 * As I am currently unsure how to implement opening the gui, this will be in its place
 * -Not implemented
 */
int open_man_gui();


#endif