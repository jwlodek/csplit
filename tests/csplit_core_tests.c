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


// Test list
CSplitList_t* list;


/* Setup function - initializes the list, and enters 5 fragments into the list */
void setup(void){
    list = csplit_init_list(256);
    int i;
    for(i=0; i< 5; i++){
        CSplitFragment_t* current_fragment =  (CSplitFragment_t*) calloc(1, sizeof(CSplitFragment_t));
        csplit_push_to_list(list, current_fragment);
        sprintf(current_fragment->text, "Number%d", i);
    }
}


/* Frees up memory */
void teardown(void){
    csplit_clear_list(list);
}


/* Test for getting at index */
Test(asserts, get_at_index_test, .init=setup, .fini=teardown){
    char* temp = get_fragment_at_index(list, 2);
    cr_assert(strcmp(temp, "Number2") == 0, "String at index not correct.");
}


/* Test for getting at index from reverse */
Test(asserts, get_at_rindex_test, .init=setup, .fini=teardown){
    char* temp = get_fragment_at_index(list, -1);
    cr_assert(strcmp(temp, "Number4") == 0, "String at reverse index not correct.");
}


/* Test for getting at invalid index */
Test(asserts, get_index_oob_test, .init=setup, .fini=teardown){
    char* temp = get_fragment_at_index(list, 7);
    cr_assert(temp == NULL, "String at index not correct.");
}


/* Test for getting at invalid index from reverse */
Test(asserts, get_rindex_oob_test, .init=setup, .fini=teardown){
    char* temp = get_fragment_at_index(list, -7);
    cr_assert(temp == NULL, "String at index not correct.");
}

