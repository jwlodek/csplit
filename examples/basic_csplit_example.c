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
 * The most basic use case example for the csplit library.
 * 
 * Splits an input string on a single character.
 * 
 * Author: Jakub Wlodek
 * Created: 02-Aug-2019
 * 
 */

// include csplit and stdio
#include "csplit.h"
#include "stdio.h"


int main(int argc, char** argv){
    // our test string
    char* test_string = "Hello how are you doing?";
    printf("Our demo string is: %s\n", test_string);

    // initialize our output list. We pass 256 as our buffer size, meaning that our fragments cannot 
    // be longer than 256 characters in length.
    CSplitList_t* list = csplit_init_list(256);

    // split on the " " (space) character
    CSplitError_t err = csplit(list, test_string, " ");

    // print the list of split fragments to stdout
    print_csplit_list_info(list, stdout);

    // print a separator
    printf("----------------------------\n");

    // demo of getting fragment string at an index, 3 index will give us "you"
    char* test_get_index = get_fragment_at_index(list, 3);

    // demo of getting fragment string using reverse index, -1 will give us the last
    // fragment, in this case "doing?"
    char* test_get_r_index = get_fragment_at_index(list, -1);

    // print results
    printf("Get index: %s\n", test_get_index);
    printf("Get reverse index: %s\n", test_get_r_index);

    // free memory
    csplit_clear_list(list);
}