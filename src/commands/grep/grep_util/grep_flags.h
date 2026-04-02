#ifndef GREP_FLAGS_H
#define GREP_FLAGS_H

//A comprehensive array of flags that my grep clone will recognize. Contains short and long
//versions of each flag.
extern const char *VALID_FLAGS[];

extern const int VALID_FLAG_COUNT;


//Checks if an individual flag is present in the array of valid flags.
//Returns 1 if it exists, 0 if not.
int flag_valid(const char* param);

#endif