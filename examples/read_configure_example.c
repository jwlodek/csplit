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
 * Example demonstrating reading a custom configure file using csplit
 * 
 * The example file is available in "exampleFiles/CONFIGURE"
 * 
 * Author: Jakub Wlodek
 * Created: 05-Aug-2019
 * 
 */

// include csplit and stdio
#include "csplit.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
    // open the file
    FILE* fp = fopen("exampleFiles/CONFIGURE", "r");

    if(fp == NULL){
        printf("Couldn't open file.\n");
        return -1;
    }
    
    // char arrays where we will store resulting config
    char install_path[32];
    char build_utests[32];
    char build_examples[32];

    char line_buff[256];
    while(fgets(line_buff, 256, fp)){
        
        // use csplit and string functions to remove comments and empty lines
        if(csplit_startswith(line_buff, "#") != 0 && strlen(line_buff) > 1){
            CSplitList_t* list = csplit_init_list();
            
            // initialize list, strip of whitespace, and split on '=' character.
            char* temp = csplit_strip(line_buff);
            CSplitError_t err = csplit(list, temp, "=");
            
            // free up the stripped string
            free(temp);
            
            // print some info
            printf("Found config line:\n");
            csplit_print_list_info(list, stdout);
            
            // if we match one of our targets, copy the value (index 1) into the appropriate array
            if(csplit_startswith(csplit_get_fragment_at_index(list, 0), "INSTALL_PATH") == 0){
                strncpy(install_path, csplit_get_fragment_at_index(list, 1), sizeof(install_path));
            }
            else if(csplit_startswith(csplit_get_fragment_at_index(list, 0), "INSTALL_UTESTS") == 0){
                strncpy(build_utests, csplit_get_fragment_at_index(list, 1), sizeof(build_utests));
            }
            else if(csplit_startswith(csplit_get_fragment_at_index(list, 0), "BUILD_EXAMPLES") == 0){
                strncpy(build_examples, csplit_get_fragment_at_index(list, 1), sizeof(build_examples));
            }
            
            // free list memory
            csplit_clear_list(list);
        }
    }
    
    // close the file, print our results.
    fclose(fp);
    printf("Configuration read from CONFIGURE file is:\n");
    printf("INSTALL_LOCATION: %s, INSTALL_UTESTS: %s, BUILD_EXAMPLES: %s\n", install_path, build_utests, build_examples);
    return 0;
}
