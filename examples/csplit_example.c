#include "csplit.h"
#include "stdio.h"


int main(int argc, char** argv){
    char* test_string = "Hello-how-are-you-doing?";

    CSplitList_t* list = csplit_init_list(256);
    CSplitError_t err = csplit(list, test_string, "-");
    print_list_info(list, stdout);
    printf("----------------------------\n");

    char* test = get_fragment_at_index(list, -1);
    printf("%s\n", test);
    csplit_clear_list(list);
}