#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 256


typedef enum CSPLIT_ERROR {
    CSPLIT_SUCCESS          = 0,
    CSPLIT_TOO_SHORT        = 1,
    CSPLIT_NO_SUCH_INDEX    = 2,
    CSPLIT_UNIMPLEMENTED    = 3,
    CSPLIT_BUFF_EXCEEDED    = 4,
} CSplitError_t;


typedef struct CSPLIT_FRAGMENT {
    char* text;
    struct CSPLIT_FRAGMENT* next;
    struct CSPLIT_FRAGMENT* prev;
} CSplitFragment_t;


typedef struct CSPLIT_LIST {
    int num_elems;
    CSplitFragment_t* head;
    CSplitFragment_t* tail;
} CSplitList_t;


void print_list_info(CSplitList_t* list, FILE* fp){
    int counter = 0;
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        fprintf(fp, "%s\n", current_fragment->text);
        current_fragment = current_fragment->next;
    }
}


char* get_fragment_at_index(CSplitList_t* list, int index){
    if(list->num_elems <= index){
        return NULL;
    }
    else{
        int counter = 0;
        CSplitFragment_t* current_fragment = list->head;
        while(counter < index){
            current_fragment = current_fragment->next;
            counter++;
        }
        return current_fragment->text;
    }
}

CSplitError_t reverse_csplit_list(CSplitList_t* list){
    int i;
    CSplitFragment_t* temp = list->head;
    for(i = 0; i < list->num_elems; i++){
        CSplitFragment_t* temp2 = temp->next;
        temp->next = temp->prev;
        temp->prev = temp2;
    }
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
    return CSPLIT_SUCCESS;
}


CSplitError_t csplit_char(CSplitList_t* list, char* input_str, char token){
    int counter = 0;
    int len = strlen(input_str);
    printf("%d\n", len);
    list->num_elems = 0;
    while(counter < len){
        CSplitFragment_t* fragment = (CSplitFragment_t*) calloc(1, sizeof(CSplitFragment_t));
        fragment->text = (char*) malloc(sizeof(BUFF_SIZE));
        int fragment_counter = 0;
        if(list->head == NULL){
            list->head = fragment;
            list->tail = fragment;
        }
        else{
            list->tail->next = fragment;
            list->tail = fragment;
        }
        while(input_str[counter] != token && counter < len){
            printf("Input: %c, token %c\n", input_str[counter], token);
            counter = counter++;
            fragment->text[fragment_counter] = input_str[counter];
            fragment_counter++;
            if(fragment_counter > BUFF_SIZE)
                return CSPLIT_BUFF_EXCEEDED;
        }
        counter = counter++;
        list->num_elems = list->num_elems + 1;
    }
    return CSPLIT_SUCCESS;
}


CSplitError_t csplit(CSplitList_t* output_list, char* input_str, char* token){
    CSplitError_t err = CSPLIT_SUCCESS;
    if(strlen(input_str) < 1){
        err = CSPLIT_TOO_SHORT;
    }
    else if(strlen(token) == 1){
        err = csplit_char(output_list, input_str, token[0]);
    }
    else{
        err = CSPLIT_UNIMPLEMENTED;
    }
    return err;
}


CSplitError_t rcsplit(CSplitList_t* output_list, char* input_str, char* token){
    CSplitError_t err = csplit(output_list, input_str, token);
    if(err != CSPLIT_SUCCESS)
        return err;
    err = reverse_csplit_list(output_list);
    return err;
}