# csplit

A single header C library that can be used to easily handle splitting strings in C.

### Usage

Because csplit is a single-header library, all you need to do is copy the csplit.h file into your project, and
include it at the top of your source file in which you would like to use it.

### Examples

#### Basic Example

Example demonstrating using csplit for splitting string on single character and getting split fragments based on indexes.
```C
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
    print_list_info(list, stdout);

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
#include "stdio.h"


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
```

### Why does it exist?

Because let's be honest - strtok is a huge pain.
