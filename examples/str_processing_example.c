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
 * Example demonstrating basic string processing csplit calls
 * 
 * 
 * Author: Jakub Wlodek
 * Created: 05-Aug-2019
 * 
 */

// include csplit and stdio
#include "csplit.h"
#include <stdio.h>

int main(int argc, char** argv){
    // init test string
    char* test_string = "Hello how are you doing?\n\n";
    printf("Our demo string is: --%s--\n", test_string);

    // strip outermost whitespace (2 newlines)
    char* stripped_str = csplit_strip(test_string);
    printf("The stripped string is --%s--\n", stripped_str);
    printf("Note the disappeared newline characters.\n");

    // startswith command
    int temp = csplit_startswith(test_string, "Hello");
    if(temp == 0){
        printf("The input started with 'Hello'\n");
    }

    // endswith - will fail here because of newlines, but succeed on the stripped one.
    temp = csplit_endswith(test_string, "doing?");
    if(temp == 0){
        printf("The input string ended with 'doing?'\n");
    }
    temp = csplit_endswith(stripped_str, "doing?");
    if(temp == 0){
        printf("After stripping away newlines, the string ends with 'doing?'\n");
    }

    // remove all whitespace and print
    char* no_whitespace = csplit_remove_whitespace(test_string);
    printf("The input string without any whitespace: --%s--\n", no_whitespace);

    // free memory.
    free(stripped_str);
    free(no_whitespace);
}