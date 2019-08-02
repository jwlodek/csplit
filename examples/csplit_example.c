#include "csplit.h"
#include "stdio.h"


int main(int argc, char** argv){
    char* test_string = "Hello_how_are_you_doing?";
    CSplitList_t* list = (CSplitList_t*) calloc(1, sizeof(CSplitList_t));
    list->head = NULL;
    list->tail = NULL;
    list->num_elems = 0;
    CSplitError_t err = csplit_char(&list, test_string, '_');
    print_list_info(&list, stdout);
}