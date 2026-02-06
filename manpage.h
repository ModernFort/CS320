#ifndef MANPAGE_H
#define MANPAGE_H

// Main file will call request_manpage() and this will automatically 
// print the description to stdout
void request_manpage(char* message);

// Go into a file and grab the correct description based on the query
char* get_description(char* query);

// Adds a description into the manpage
int add_description(char* query, char* description);

// Removes a description from the manpage
int remove_description(char* query);

// As I am currently unsure how to implement opening the gui, this will be in its place
int open_man_gui();


#endif