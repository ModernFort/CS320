#include "grep.h"

//All uppercase ascii chars are 32 less than their lowercase equivalants, so I define this macro
//for easy conversion.
#define TO_LOWER(x) (x + 32)

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

//Explicit definition of the base to be used for functions like strtol
#define DECIMAL_BASE 10

//A comprehensive array of flags that my grep clone will recognize. Contains short and long
//versions of each flag.
static const char *VALID_FLAGS[] = {
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

//Compute the length of the valid flags array one time and store as a static global.
//Find the length by dividing the size of the array by the size of an entry to get total 
//amount of entries.
static const int VALID_FLAG_COUNT = 
    sizeof(VALID_FLAGS) / sizeof(VALID_FLAGS[0]);

match_type get_mode(char* mode_flag){
    if(strcmp(mode_flag, "-G") == 0 || strcmp(mode_flag, "--basic-regexp") == 0) return BASIC_REGEX;
    if(strcmp(mode_flag, "-E") == 0 || strcmp(mode_flag, "--extended-regexp") == 0) return EXTENDED_REGEX;
    if(strcmp(mode_flag, "-F") == 0 || strcmp(mode_flag, "--fixed-strings") == 0) return FIXED_STRING;

    //If none of the valid modes are detected, return INVALID_MODE so the calling process can exit/throw error
    return INVALID_MODE;
} 

_pattern_info get_patterns(char** args, int start_idx){
    //Initialize a new pattern info struct as well as the fields it will store.
    _pattern_info info = {0};
    int pattern_idx = 0;
    int curr_idx = start_idx;

    //Loop over the arguments until a flag is detected, adding each pattern to the
    //patterns array and incrementing pattern count/the current idx.
    while(args[curr_idx] && !flag_valid(args[curr_idx])){
        info.pattern_count++;
        //Return early if the pattern count goes above max patterns, to be checked by
        //the calling function. This prevents overflow of the max patterns array.
        if(info.pattern_count > MAX_PATTERNS) return info;
        info.patterns[pattern_idx++] = args[curr_idx++];
        curr_idx++;
    }

    return info;
}

_file_info get_files(char** args, int start_idx){
    //Initialize a new pattern info struct as well as the fields it will store.
    _file_info info = {0};
    int file_idx = 0;
    int curr_idx = start_idx;

    //Loop over the arguments until a flag is detected, adding each pattern to the
    //patterns array and incrementing pattern count/the current idx.
    while(args[curr_idx] && !flag_valid(args[curr_idx])){
        info.file_count++;
        //Return early if the pattern count goes above max patterns, to be checked by
        //the calling function. This prevents overflow of the max patterns array.
        if(info.file_count > MAX_PATTERNS) return info;
        info.file_paths[file_idx++] = args[curr_idx++];
        curr_idx++;
    }

    return info;
}

int flag_valid(const char* param){
    if(param == NULL) return 0;
    //Iterate over the array of valid flags, if a match is found return 1.
    for(int i = 0; i < VALID_FLAG_COUNT; i++) {
        if(strcmp(param, VALID_FLAGS[i]) == 0) return 1;
    }
    //If the entire array has been searched and no match was found, return 0.
    return 0;
}

int validate_params(int paramc, char** params){
    //Iterate over each param
    for(int i = 0; i < paramc; i++){
        //Find the current param and look for a dash at the start indicating it is a flag.
        //If it is, check to see if the flag is valid. If it isn't return 0.
        char* curr = params[i];
        if(curr[0] == '-'){
            if(!flag_valid(curr)) {
                fprintf(stderr, "Error: Parameter \"%s\" is not a valid flag!\n", curr);
                return 0;
            }
        }
    }
    //All detected flags were valid, return success.
    return 1;
}

grep_state init_state(int grep_argc, char** args){
    //This version of grep must minimally contain a mode (regex or plain text) flag, 
    //followed by match flags (if any), followed by pattern/text, and finally 
    //files to search. The mode is required to be the first argument.
    if(grep_argc < 4 || !flag_valid(args[0])){
        fprintf(stderr, "Usage: grep MODE <MATCH FLAGS> PATTERN -f FILE <FILE2>...\n");
        exit(EXIT_FAILURE);
    }

    //Initialize a 0/null filled state struct, as well as arrays to store patterns/files.
    grep_state state = {0};
    char* patterns[MAX_PATTERNS];
    char* files[MAX_FILES];

    //Determine the mode based off of the first flag, set the match mode of the grep state.
    char* mode_flag = args[0];
    match_type mode = get_mode(mode_flag);
    if(mode == INVALID_MODE){
        fprintf(stderr, "Error: Please enter a valid mode\n");
        fprintf(stderr, "Usage: grep MODE <MATCH FLAGS> PATTERN -f FILE <FILE2>...\n");
        exit(EXIT_FAILURE);       
    }

    state.mode = mode;

    int idx = 1;

    //Loop over the array until no flags remain
    while(flag_valid(args[idx])){
        char* flag = args[idx];
        //Multiple pattern flag is indicated, flip the boolean flag
        if((strcmp(flag, "-e") == 0 || strcmp(flag, "--regexp=") == 0)){
            //Call the helper to get the pattern info (patterns and pattern count)
            _pattern_info info = get_patterns(args, idx + 1);
            //If the count is greater than max patterns, no patterns were detected, or the terminating -f flag isn't the next flag,
            //throw usage errors and exit.
            if(info.pattern_count > MAX_PATTERNS){
                fprintf(stderr, "Error: A maximum of 256 patterns are allowed\n");
                exit(EXIT_FAILURE);
            }

            if(info.pattern_count == 0){
                fprintf(stderr, "Usage: grep MODE <FLAGS> -e PATTERNS -f FILES\n");
                fprintf(stderr, "Error: the -e flag must be followed by at least one pattern!\n");
                exit(EXIT_FAILURE);
            }
            //Update pattern info and add the pattern count to the idx to update the array pointer to point
            //to the next flag, past the patterns.
            state.pattern_info = info;
            idx += 1 + state.pattern_info.pattern_count;

            if((args[idx] == NULL) || (strcmp(args[idx], "-f") != 0)){
                fprintf(stderr, "Usage: grep MODE <FLAGS> -e PATTERNS -f FILES\n");
                fprintf(stderr, "Error: Pattern arguments must be terminated by file (-f or --file=) flag!\n");
                exit(EXIT_FAILURE);
            }

            continue;
        }

        //File flag detected, call the helper and add the struct to the state, very similarly to the patterns struct
        if((strcmp(flag, "-f") == 0 || strcmp(flag, "--file=") == 0)){
            _file_info info = get_files(args, idx + 1);
            if(info.file_count > MAX_FILES){
                fprintf(stderr, "Error: A maximum of 256 files are allowed\n");
                exit(EXIT_FAILURE);
            }

            if(info.file_count == 0){
                fprintf(stderr, "Usage: grep MODE <FLAGS> -e PATTERNS -f FILES\n");
                fprintf(stderr, "Error: the -f flag must be followed by at least one file!\n");
                exit(EXIT_FAILURE);
            }

            state.file_info = info;
        }

        //Match/output control flags are handled here, each flag is checked for
        //individually, and on detection the corresponding state in the state
        //struct is updated.
        if((strcmp(flag, "-i") == 0 || strcmp(flag, "--ignore-case") == 0)){
            state.ignore_case = 1;
        }
        
        if((strcmp(flag, "-v") == 0 || strcmp(flag, "--invert-match") == 0)){
            state.invert_match = 1;
        }        

        if((strcmp(flag, "-c") == 0 || strcmp(flag, "--count") == 0)){
            state.count_only = 1;
        }
        
        if((strcmp(flag, "-L") == 0 || strcmp(flag, "--files-without-match") == 0)){
            state.files_without_matches = 1;
        }

        if((strcmp(flag, "-l") == 0 || strcmp(flag, "--files-with-matches") == 0)){
            state.files_with_matches = 1;
        }

        //A max match count is explicitly defined by the user with the max count flag
        if((strcmp(flag, "-m") == 0 || strcmp(flag, "--max-count=") == 0)){
            //The max match integer must immediately follow the flag in the next index
            int count_arg_idx = idx + 1;
            const char* count_arg = args[count_arg_idx];
            char* end;
            //Reset errno to reliably error check strtol
            errno = 0;

            //Convert the max_count arg to a long, if no int was entered print usage error.
            //If errno is not 0, strtol failed so throw an error there too.
            long max_count = strtol(count_arg, &end, DECIMAL_BASE);
            if(end == count_arg){
                fprintf(stderr, "Error: Max count requires a valid integer\n");
                fprintf(stderr, "Usage: grep MODE -m INTEGER PATTERN -f FILE\n");
                fprintf(stderr, "Or: grep MODE --max-count= INTEGER PATTERN -f FILE\n");
                exit(EXIT_FAILURE);
            }

            if(errno != 0){
                perror("Error converting max count arg to long:");
                exit(EXIT_FAILURE);
            }

            //If the end pointer is not the null terminator, the full string was not converted.
            if(*end != '\0'){
                fprintf(stderr, "Error: Full conversion of max count to long was unsuccessful!\n");
                exit(EXIT_FAILURE);
            }

            //Update the max count state in the struct
            state.max_count = max_count;

        }

        if((strcmp(flag, "-o") == 0 || strcmp(flag, "--only-matching") == 0)){
            state.only_matching = 1;
        }

        if((strcmp(flag, "-s") == 0 || strcmp(flag, "--no-messages") == 0)){
            state.suppress_errs = 1;
        }

        idx++;
    }


}

char* lower_line(const char* line){
    //Find the length of the line and allocate a new char*
    //with this length to store the lowercase version
    int len = strlen(line);
    char* lowered = malloc((len + 1) * sizeof(char));

    //Loop thru the line. If a char is uppercase, convert to lower
    //and add it. Otherwise, add it as is.
    for(int i = 0; i < len; i++){
        char c = line[i];
        if(isupper(c)){
            lowered[i] = TO_LOWER(c);
        } else {
            lowered[i] = c;
        }
    }
    
    return lowered;
}

//Open the desired file and error check, returning the file pointer on success.
FILE* open_file(char* user_file){
    FILE* fp;
    fp = fopen(user_file, "r");
    if(fp == NULL){
        fprintf(stderr, "Could not open file: %s\n", user_file);
        exit(EXIT_FAILURE);
    }

    return fp;
}

int match_text(FILE* fp, const char* str_to_match){
    //Create a buffer for fgets to store the line of the file
    char buf[MAX_LINE_LEN];

    int found_match = 0;

    //Iterate over each line of the file until EOF is reached
    while(fgets(buf, MAX_LINE_LEN, fp) != NULL){
        int len = strlen(buf);
        //If the text is contained, print the entire line (grep's default functionality)
        if(contains_text(buf, str_to_match)){
            found_match = 1;
            printf("%s", buf);
            //If the line in the file somehow didn't contain a newline, manually add one.
            if(strlen(buf) != 0 && buf[len - 1] != '\n') putchar('\n');
        }
    }

    return found_match;
}

int match_pattern(FILE* fp, const char* pattern_to_match){

}

int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}