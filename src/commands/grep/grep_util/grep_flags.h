#ifndef GREP_FLAGS_H
#define GREP_FLAGS_H


//A comprehensive array of flags that my grep clone will recognize. Contains short and long
//versions of each flag.
extern const char *VALID_FLAGS[] = {
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

#endif