#include "unity.h"
#include "../src/commands/grep.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Black box unit tests:
*/

//Test that contains_text can correctly detect full word substrings with spaces,
//at the beginning and end of a string
void test_contains_word(){
    TEST_ASSERT_TRUE(contains_text("hello world", "hello")); 
    TEST_ASSERT_TRUE(contains_text("hello world", "world"));
}

//Test that contains text returns false if the substring isn't present
void test_no_match(){
    TEST_ASSERT_FALSE(contains_text("hello world", "bananaa"));
}

//Test flag_valid to ensure it correctly recognizes known flags
void test_flag_valid(){
    TEST_ASSERT_TRUE(flag_valid("-E"));
    TEST_ASSERT_TRUE(flag_valid("--ignore-case"));
}

//Test that flag_valid rejects invalid flags
void test_flag_valid_rejects(){
    TEST_ASSERT_FALSE(flag_valid("-Ze"));
}

//Test that my line to lowercase function correctly converts lines to lowercase
void test_lower_line(){
    char* lowered = lower_line("HELLO WORLD");
    TEST_ASSERT_TRUE(strcmp(lowered, "hello world") == 0);
    free(lowered);
}

/*
    White box unit tests: Cover the conditional branches in get_mode, ensuring
    each flag is mapped to the corresponding mode correctly, and that each invalid
    flag is simply returned as an invalid instance of the match_type enum.
*/

void test_get_mode_basic_short(){
    TEST_ASSERT_EQUAL(BASIC_REGEX, get_mode("-G"));
}

void test_get_mode_basic_long(){
    TEST_ASSERT_EQUAL(BASIC_REGEX, get_mode("--basic-regexp"));
}

void test_get_mode_extended_short(){
    TEST_ASSERT_EQUAL(EXTENDED_REGEX, get_mode("-E"));
}

void test_get_mode_extended_long(){
    TEST_ASSERT_EQUAL(FIXED_STRING, get_mode("--extended-regexp"));
}

void test_get_mode_fixed_short(){
    TEST_ASSERT_EQUAL(FIXED_STRING, get_mode("-F"));
}

void test_get_mode_fixed_long(){
    TEST_ASSERT_EQUAL(FIXED_STRING, get_mode("--fixed-strings"));
}

void test_get_mode_invalid(){
    TEST_ASSERT_EQUAL(INVALID_MODE, get_mode("-B"));
}

//Test that get_mode is case sensitive, returning invalid if -e is
//recieved instead of -E, since the lowercased -e flag is used else
//where in command parsing.
void test_get_mode_invalid_case_sensitive(){
    TEST_ASSERT_EQUAL(INVALID_MODE, get_mode("-e"));
}

/*
    Integration tests
*/

//Test open_file util + contains_text + match_text pipeline
void test_match_text_detects_pattern(){

    //Create and write words to a file to test
    FILE* f = fopen("test_file1.txt", "w");
    fprintf(f, "apple\nbanana\ncarrot\n");
    fclose(f);

    FILE* fp = open_file("test_file1.txt");

    int result = match_text(fp, "banana");

    TEST_ASSERT_TRUE(result);

    fclose(fp);
    remove("test_file1.txt");
}

void test_match_text_rejects_pattern(){

    FILE* f = fopen("test_file2.txt", "w");
    fprintf(f, "apple\nbanana\ncarrot\n");
    fclose(f);

    FILE* fp = open_file("test_file2.txt");

    int result = match_text(fp, "blah");

    TEST_ASSERT_TRUE(result);

    fclose(fp);
    remove("test_file2.txt");
}


void setUp(void){}
void tearDown(void){}

int main(void){

    UNITY_BEGIN();

    //Run black box tests
    RUN_TEST(test_contains_word);
    RUN_TEST(test_no_match);
    RUN_TEST(test_flag_valid);
    RUN_TEST(test_flag_valid_rejects);
    RUN_TEST(test_lower_line);

    //Run white box tests
    RUN_TEST(test_get_mode_basic_short);
    RUN_TEST(test_get_mode_basic_long);
    RUN_TEST(test_get_mode_extended_short);
    RUN_TEST(test_get_mode_extended_long);
    RUN_TEST(test_get_mode_fixed_short);
    RUN_TEST(test_get_mode_fixed_long);
    RUN_TEST(test_get_mode_invalid);
    RUN_TEST(test_get_mode_invalid_case_sensitive);

    //run integration tests
    RUN_TEST(test_match_text_detects_pattern);
    RUN_TEST(test_match_text_rejects_pattern);

    return UNITY_END();
}