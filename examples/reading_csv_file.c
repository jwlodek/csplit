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
 * Example usage of csplit for parsing .csv files
 * 
 * Author: Jakub Wlodek
 * Created: 02-Aug-2019
 * 
 */

// include csplit and stdio
#include "csplit.h"
#include <stdio.h>


int main(int argc, char** argv){
    // open the file, return if NULL
    FILE* csv_file = fopen("exampleFiles/test.csv", "r");
    if(csv_file == NULL){
        printf("Failed to open file, exiting.\n");
        return -1;
    }

    // buffer for reading from file
    char buffer[256];
    while(fgets(buffer, 256, csv_file)) {
        // initialize the list, and call csplit on commas (buffsize=256)
        CSplitList_t* list = csplit_init_list(256);
        CSplitError_t err = csplit(list, buffer, ",");

        // print the split values
        print_csplit_list_info(list, stdout);
        printf("----------------------\n");

        // example iterating through resulting list and summing values read from .csv file
        CSplitFragment_t* current_fragment = list->head;
        int sum = 0;
        while(current_fragment != NULL){
            sum = sum + atoi(current_fragment->text);
            current_fragment = current_fragment->next;
        }
        
        // print some and free memory
        printf("The sum of the elements in the line = %d\n", sum);
        csplit_clear_list(list);
    }
    return 0;
}