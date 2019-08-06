# csplit Usage Examples

To build csplit examples, enter the `examples` directory, and run:
```
make clean all
```
or
```
make clean debug
```
to have access to gdb debugging of the examples. This has been tested on linux, as well as on Windows with gcc and make included in MinGW.

#### Basic Example

Example demonstrating using csplit for splitting string on single character and getting split fragments based on indexes.
```C
// include csplit and stdio
#include "csplit.h"
#include <stdio.h>


int main(int argc, char** argv){
    // our test string
    char* test_string = "Hello how are you doing?";
    printf("Our demo string is: %s\n", test_string);

    // initialize our output list
    CSplitList_t* list = csplit_init_list();

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
```

#### .csv reading example

Example of using csplit to read .csv files.
```C
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

        // ignore lines that are blank
        if(strlen(buffer) > 1){
            // initialize the list, strip whitespace, and call csplit on commas
            CSplitList_t* list = csplit_init_list();
            char* temp = csplit_strip(buffer);
            CSplitError_t err = csplit(list, temp, ",");
            free(temp); /* Make sure to free stripped line */

            // print the split values
            print_csplit_list_info(list, stdout);

            // example iterating through resulting list and summing values read from .csv file
            CSplitFragment_t* current_fragment = list->head;
            int sum = 0;
            while(current_fragment != NULL){
                sum = sum + atoi(current_fragment->text);
                current_fragment = current_fragment->next;
            }

            // print sum of numbers in line, and free memory
            printf("The sum of the elements in the line = %d\n", sum);
            csplit_clear_list(list);

            printf("----------------------\n");
        }
    }
    fclose(csv_file);
    return 0;
}
```

### Basic string operations example

A simple example showing some additional string processing functions included with csplit

```C
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
```

### Custom configure example

An example of using csplit to read a custom configuration file

```C
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
```

