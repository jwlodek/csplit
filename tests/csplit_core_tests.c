/********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2019 Jakub Wlodek
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

/**
 * Unit test file for core csplit functionality
 * 
 * Author: Jakub Wlodek
 */


#include "csplit.h"

#include "criterion/assert.h"
#include "criterion/criterion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------
// ------ Some values and variables used by tests ---------
// --------------------------------------------------------

#define BUFF_SIZE 256

// Test list
CSplitList_t* list;

char* input_test_string = "Hello how are you doing?";
char* input_test_string_w_whitespace = "\n\n Hello how are you doing?\t\n\n";
//char* output_test_string;


// --------------------------------------------------------
// ------------- Setup and Teardown Functions -------------
// --------------------------------------------------------

/* Setup function - initializes the list, and enters 5 fragments into the list */
void setup(void){
    list = csplit_init_list(BUFF_SIZE);
    int i;
    for(i=0; i< 5; i++){
        CSplitFragment_t* current_fragment =  (CSplitFragment_t*) calloc(1, sizeof(CSplitFragment_t));
        csplit_push_to_list(list, current_fragment);
        sprintf(current_fragment->text, "Number%d", i);
    }
}

void setup_strings(void){
    //output_test_string = (char*) calloc(1, BUFF_SIZE);
}

/* Frees up memory */
void teardown(void){
    csplit_clear_list(list);
}


void teardown_strings(void){
    if(list != NULL){
        csplit_clear_list(list);
    }
}


// --------------------------------------------------------
// ------------- Tests for getting at index ---------------
// --------------------------------------------------------


/* Test for getting at index */
Test(asserts, get_at_index_test, .init=setup, .fini=teardown){
    char* temp = csplit_get_fragment_at_index(list, 2);
    cr_assert(strcmp(temp, "Number2") == 0, "String at index not correct.");
}


/* Test for getting at index from reverse */
Test(asserts, get_at_rindex_test, .init=setup, .fini=teardown){
    char* temp = csplit_get_fragment_at_index(list, -1);
    cr_assert(strcmp(temp, "Number4") == 0, "String at reverse index not correct.");
}


/* Test for getting at invalid index */
Test(asserts, get_index_oob_test, .init=setup, .fini=teardown){
    char* temp = csplit_get_fragment_at_index(list, 7);
    cr_assert(temp == NULL, "String at index not correct.");
}


/* Test for getting at invalid index from reverse */
Test(asserts, get_rindex_oob_test, .init=setup, .fini=teardown){
    char* temp = csplit_get_fragment_at_index(list, -7);
    cr_assert(temp == NULL, "String at index not correct.");
}


// --------------------------------------------------------
// ------------- Test for reversing a list ----------------
// --------------------------------------------------------


/* Test for reversing a CSplitList */
Test(asserts, reverse_list_test, .init=setup, .fini=teardown){
    CSplitError_t err = csplit_reverse_list(list);
    cr_assert(err == CSPLIT_SUCCESS, "Reverse list failed.");
    int counter = 4;
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        char temp[256];
        sprintf(temp, "Number%d", counter);
        cr_assert(strcmp(current_fragment->text, temp) == 0, "List not reversed correctly.");
        current_fragment = current_fragment->next;
        counter--;
    }
}


// --------------------------------------------------------
// ------------- Tests for 'strip' function ---------------
// --------------------------------------------------------


/* Test for csplit_strip function */
Test(asserts, csplit_strip_test, .init=setup_strings, .fini=teardown_strings){
    char* output_str = csplit_strip(input_test_string_w_whitespace);
    cr_assert(strcmp(output_str, input_test_string) == 0, "Output string doesn't match expected.");
    free(output_str);
}


/* Test for csplit_strip function with invalid arguments */
Test(asserts, csplit_strip_failed_test, .init=setup_strings, .fini=teardown_strings){
    char* temp = NULL;
    char* err = csplit_strip(temp);
    cr_assert(err == NULL, "Unexpected error code");
}


// --------------------------------------------------------
// -------- Tests for 'remove whitespace' function --------
// --------------------------------------------------------


/* Test for csplit_strip function */
Test(asserts, csplit_remove_whitespace_test, .init=setup_strings, .fini=teardown_strings){
    char* output_str = csplit_remove_whitespace(input_test_string_w_whitespace);
    cr_assert(strcmp(output_str, "Hellohowareyoudoing?") == 0, "Output string doesn't match expected.");
    free(output_str);
}


/* Test for csplit_strip function with invalid arguments */
Test(asserts, csplit_remove_whitespace_failed_test, .init=setup_strings, .fini=teardown_strings){
    char* temp = NULL;
    char* err = csplit_remove_whitespace(temp);
    cr_assert(err == NULL, "Unexpected error code");
}


// --------------------------------------------------------
// -------- Tests for 'starts/ends with' functions --------
// --------------------------------------------------------


/* Test for startwith function */
Test(asserts, csplit_startswith_test, .init=setup_strings, .fini=teardown_strings){
    char* startT1 = NULL;
    char* startT2 = "Hello";
    char* startT3 = "Hello Hello how are you doing?";
    char* startT4 = "doing?";
    int res = csplit_startswith(input_test_string, startT1);
    cr_assert(res == -2, "Unexpected error result");
    res = csplit_startswith(input_test_string, startT2);
    cr_assert(res == 0, "Unexpected incorrect result");
    res = csplit_startswith(input_test_string, startT3);
    cr_assert(res == -1, "Unexpected error result");
    res = csplit_startswith(input_test_string, startT4);
    cr_assert(res == -1, "Unexpected error result");
}


/* Test for endswith function */
Test(asserts, csplit_endswith_test, .init=setup_strings, .fini=teardown_strings){
    char* endT1 = NULL;
    char* endT2 = "doing?";
    char* endT3 = "Hello Hello how are you doing?";
    char* endT4 = "Hello";
    int res = csplit_endswith(input_test_string, endT1);
    cr_assert(res == -2, "Unexpected error result");
    res = csplit_endswith(input_test_string, endT2);
    cr_assert(res == 0, "Unexpected incorrect result");
    res = csplit_endswith(input_test_string, endT3);
    cr_assert(res == -1, "Unexpected error result");
    res = csplit_endswith(input_test_string, endT4);
    cr_assert(res == -1, "Unexpected error result");
}


// --------------------------------------------------------
// ---------------- Tests for split functions -------------
// --------------------------------------------------------

/* Test for splitting on max number of splits */
Test(asserts, csplit_char_max_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "Hello Cool World!";
    CSplitError_t err = csplit_char(list, short_test_str, ' ', strlen(short_test_str));
    cr_assert(list->num_elems == 3, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "Hello") == 0, "First string not as expected");
    cr_assert(strcmp(list->head->next->text, "Cool") == 0, "Second string not as expected");
    cr_assert(strcmp(list->tail->text, "World!") == 0, "Third string not as expected");
}

/* Test reverse split on max number of splits */
Test(asserts, csplit_rchar_max_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "Hello Cool World!";
    int target = (int) -1 * strlen(short_test_str);
    CSplitError_t err = csplit_rchar(list, short_test_str, ' ', target);
    cr_assert(list->num_elems == 3, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "Hello") == 0, "First string not as expected");
    cr_assert(strcmp(list->head->next->text, "Cool") == 0, "Second string not as expected");
    cr_assert(strcmp(list->tail->text, "World!") == 0, "Third string not as expected");
}

/* Test split on single split */
Test(asserts, csplit_char_single_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "Hello Cool World!";
    CSplitError_t err = csplit_char(list, short_test_str, ' ', 1);
    cr_assert(list->num_elems == 2, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "Hello") == 0, "First string not as expected");
    cr_assert(strcmp(list->head->next->text, "Cool World!") == 0, "Second string not as expected");
}

/* Test reverse split on single split */
Test(asserts, csplit_char_rsingle_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "Hello Cool World!";
    CSplitError_t err = csplit_rchar(list, short_test_str, ' ', -1);
    cr_assert(list->num_elems == 2, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "Hello Cool") == 0, "First string not as expected");
    cr_assert(strcmp(list->tail->text, "World!") == 0, "Second string not as expected");
}


/* Test for splitting on max number of splits on string*/
Test(asserts, csplit_string_max_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "HelloCoolWoorld!";
    CSplitError_t err = csplit_str(list, short_test_str, "oo", strlen(short_test_str));
    cr_assert(list->num_elems == 3, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "HelloC") == 0, "First string not as expected");
    cr_assert(strcmp(list->head->next->text, "lW") == 0, "Second string not as expected");
    cr_assert(strcmp(list->tail->text, "rld!") == 0, "Third string not as expected");
}

/* Test for splitting on max number of splits on string*/
Test(asserts, csplit_string_lim_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "HelloCoolWoorld!";
    CSplitError_t err = csplit_str(list, short_test_str, "oo", 1);
    cr_assert(list->num_elems == 2, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "HelloC") == 0, "First string not as expected");
    cr_assert(strcmp(list->tail->text, "lWoorld!") == 0, "Second string not as expected");
}


/* Test for splitting on max number of splits on string*/
Test(asserts, csplit_rstring_max_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "HelloCoolWoorld!";
    int target = (int) -1 * strlen(short_test_str);
    CSplitError_t err = csplit_rstr(list, short_test_str, "oo", target);
    cr_assert(list->num_elems == 3, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "HelloC") == 0, "First string not as expected");
    cr_assert(strcmp(list->head->next->text, "lW") == 0, "Second string not as expected");
    cr_assert(strcmp(list->tail->text, "rld!") == 0, "Third string not as expected");
}


/* Test for splitting on max number of splits on string*/
Test(asserts, csplit_rstring_lim_test, .init=setup_strings, .fini=teardown_strings){
    list = csplit_init_list(256);
    char* short_test_str = "HelloCoolWoorld!";
    CSplitError_t err = csplit_rstr(list, short_test_str, "oo", -1);
    cr_assert(list->num_elems == 2, "Number of fragments parsed is not as expected");
    cr_assert(strcmp(list->head->text, "HelloCoolW") == 0, "First string not as expected");
    cr_assert(strcmp(list->tail->text, "rld!") == 0, "Second string not as expected");
}
