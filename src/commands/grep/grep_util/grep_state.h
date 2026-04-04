#ifndef GREP_STATE_H
#define GREP_STATE_H

//Maximum amount of patterns/files that can be passed as arguments. Arbitrary for now
#define MAX_PATTERNS 256
#define MAX_FILES 256

//Enum ensuring types of matches remain mutually exclusive, so multiple can't be
//selected at once.
typedef enum {
    BASIC_REGEX,
    EXTENDED_REGEX,
    FIXED_STRING,
    INVALID_MODE //If no valid mode flag, this will be set as the state struct's mode for error handling
} match_type;

//Structs that store an array of patterns alongside count and an array of file paths alongside count.
//Defining these as structs allows for easy offloading of pattern/file parsing to helpers, since they
//need to store and return info about multiple variables.
typedef struct {
    char* patterns[MAX_PATTERNS];
    int pattern_count;
} _pattern_info;

typedef struct {
    char* file_paths[MAX_FILES];
    int file_count;
} _file_info;

//Struct that stores the running state of grep, including a running total of matches and files with/without matches
typedef struct {
    long total_matches;
    char* _files_with_matches[MAX_FILES];
    char* _files_without_matches[MAX_FILES];
} running_totals;

//Struct that stores information about the current state of the grep command, like the type
//of match, selected flags, match count, etc.
typedef struct {
    //Can be either extended/basic regex or plain text, but not multiple
    match_type mode;

    //Boolean flags representing output options of grep
    int ignore_case;
    int invert_match;
    int suppress_errs;
    int only_matching;
    int count_only;
    int files_with_matches;
    int files_without_matches;

    //Pattern/file arguments passed at runtime alongside their counts
    _pattern_info pattern_info;
    _file_info file_info;

    //If max count flag is selected, this stores the maximum matches that can be counted before stopping
    long max_count;

    //Reference to a struct storing the running totals as the command executes
    running_totals totals;

} grep_state;

//Takes a flag as an argument and returns the corresponding mode enum, or INVALID_MODE if none is detected.
match_type get_mode(char* mode_flag);

//Helpers that take the array of user arguments and return an array of patterns/files with the count of each.
_pattern_info get_patterns(char** args, int start_idx);
_file_info get_files(char** args, int start_idx);

//Checks if an individual flag is present in the array of valid flags.
//Returns 1 if it exists, 0 if not.
int flag_valid(const char* param);

//Takes an array of strings that represent the parameters the user used when using grep. Returns 0 if invalid params are
//given, 1 if all params are valid. The amount of params must also be passed (in similar fashion to main) so the param
//array can be accurately iterated over.
int validate_params(int paramc, char** params);

//Takes an array of strings representing arguments and flags the user passed when calling grep (not including the grep
//command itself) and creates a new grep flags struct based on these args. Validates that flags contain arguments if 
//required, and throws errors if not.
grep_state init_state(int grep_argc, char** args);

#endif