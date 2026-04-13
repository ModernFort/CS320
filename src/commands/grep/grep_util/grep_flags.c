#include "grep_flags.h"

 const char *VALID_FLAGS[] = {
    //Pattern matching flags
    "-E", "--extended-regexp", //Flags to indicate extended regex matching
    "-F", "--fixed-strings", //Indicates plain text matching
    "-G", "--basic-regexp", //Basic regex matching
    
    //Matching control flags
    "-e", "--regexp=", //Searches for multiple patterns
    "-f", "--file=", //Matches a specific file
    "-i", "--ignore-case", //Ignore case of pattern/input data
    "-v", "--invert-match", //Select only non-matching lines

    //Output control flags
    "-c", "--count", //Print a count of matching lines rather than normal output
    "-L", "--files-without-match", //Print the name of input files without matching text
    "-l", "--files-with-matches", //Print files that do contain matching text
    "-m", "--max-count=", //Stop reading a file after --max-count= n matching lines
    "-o", "--only-matching", //Print only the matching parts of a line seperate from the actual line
    "-s", "--no-messages" //Suppress error messages about unreadable/nonexistent files 
};

const int VALID_FLAG_COUNT = sizeof(VALID_FLAGS) / sizeof(VALID_FLAGS[0]);

int flag_valid(const char* param){
    if(param == NULL) return 0;
    //Iterate over the array of valid flags, if a match is found return 1.
    for(int i = 0; i < VALID_FLAG_COUNT; i++) {
        if(strcmp(param, VALID_FLAGS[i]) == 0) return 1;
    }
    //If the entire array has been searched and no match was found, return 0.
    return 0;
}
