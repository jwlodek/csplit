<p align="center">
    <img src="docs/assets/csplit_logo.png">
</p>

# csplit ![](https://travis-ci.org/jwlodek/csplit.svg?branch=master)

A single header C library that can be used to easily handle splitting strings and other string operations in C.

### Installation

Because csplit is a single-header library, all you need to do is copy the `csplit.h` file into your project, and
include it at the top of your source file in which you would like to use it. Clone this repository with:
```
git clone https://github.com/jwlodek/csplit
```
Then copy `csplit.h` into your repository, add the
```C
#include "csplit.h"
```
line at the top of your code, and, if necessary, update your project's include path.

### Usage

The core functions that csplit provides are the following:
```C
CSplitError_t csplit(CSplitList_t* list, char* input_str, char* token);
```
This will split the input string based on the token, as many times as possible. You may also limit the number of splits with:
```C
CSplitError_t csplit_lim(CSplitList_t* list, char* input_str, char* token, int max_splits);
```
where max_splits is the maximum number of splits allowed. If max_splits is negative, csplit will perform the splits from the end of the string first, instead of from the front.

There are also some utility functions included as well:
```C
char* csplit_get_fragment_at_index(CSplitList_t* list, int index); /* Will return text at given index in list */
char* csplit_strip(char* input_str); /* Strips whitespace from front and rear of string */
int csplit_startswith(char* input_str, char* starts_with); /* Checks if string starts with given token. */
```
and several other string utility/manipulation functions. Full documentation can be seen [here.](https://jwlodek.github.io/csplit-docs/)

### Running Unit Tests

Unit testing for `csplit` is done with the help of the [Criterion](https://github.com/Snaipe/Criterion) library. To simplify setup, scripts have been added to the `tests/` directory that setup this libarary, and run the tests. Simply run:
```
cd tests
./initUnitTests.sh
```
to initialize the [Criterion](https://github.com/Snaipe/Criterion) Library, and then
```
./runUnitTests.sh
```
to run the tests. Note that this only works on linux systems, and on windows you must set up the unit testing environment manually.

### Examples

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

    // initialize our output list.
    CSplitList_t* list = csplit_init_list();

    // split on the " " (space) character
    CSplitError_t err = csplit(list, test_string, " ");

    // print the list of split fragments to stdout
    csplit_print_list_info(list, stdout);

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

#### More examples are available in the `examples` directory of this repository.

### Why does it exist?

I wrote csplit because I found string manipulation in C to be very difficult, particularly the `strtok` built in function. I have successfully used csplit in several projects, primarily for parsing input files, and I hope it proves to be of use to you.

### License

MIT (c) 2019 - Jakub Wlodek
