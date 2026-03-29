#include "grep_flags.h"
#include "stdio.h"

int flag_valid(const char* param){
    if(param == NULL) return 0;
    //Iterate over the array of valid flags, if a match is found return 1.
    for(int i = 0; i < VALID_FLAG_COUNT; i++) {
        if(strcmp(param, VALID_FLAGS[i]) == 0) return 1;
    }
    //If the entire array has been searched and no match was found, return 0.
    return 0;
}