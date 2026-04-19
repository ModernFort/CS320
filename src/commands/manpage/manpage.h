#ifndef MANPAGE_H
#define MANPAGE_H

#include <string.h>
#include <stdio.h>




/**
 * Searches and returns the correct description based on the query.
 * 
 * @param query
 */
FILE* get_manpage_file(char* query);

/** 
 * Adds a description into the manpage file
 * 
 * -Not implemented
 * 
 * @param query
 * @param description
 */
int create_new_description(char* query, char* description);

/**
 * Finds the manpage, and pulls the description from the manpage out 
 * as a char array. It will then print the array to stdout.
 * 
 * @param query
 */
int print_description(char* query);

/**
 * Function for main, which will check for a definition corresponding to query
 * and print the description to stdout.
 * 
 * -If the query is NULL, it will print how to use the command to stdout and return 1
 * 
 * -If the query is not found, it will print a message to stdout and return 0
 * 
 * @param query
 */
int request_manpage(char* query);

/**
 * Removes a description from the manpage
 * 
 * -Not implemented
 * @param query
 */
int delete_description(char* query);



#endif
