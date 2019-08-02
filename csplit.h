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
 * 
 * A single-header C library for string splitting.
 * 
 * Author: Jakub Wlodek
 * Created: 02-Aug-2019
 */


#ifndef CSPLIT_H
#define CSPLIT_H

// some basic includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/* Enum type for error codes for csplit */
typedef enum CSPLIT_ERROR {
    CSPLIT_SUCCESS          = 0,    /* No Error */
    CSPLIT_TOO_SHORT        = -1,    /* Input string is too short */
    CSPLIT_NO_SUCH_INDEX    = -2,    /* Index out of range */
    CSPLIT_UNIMPLEMENTED    = -3,    /* Function unimplemented */
    CSPLIT_BUFF_EXCEEDED    = -4,    /* Buffer size exceeded */
} CSplitError_t;


/* Struct for an individual string fragment result from csplit */
typedef struct CSPLIT_FRAGMENT {
    char* text;                         /* actual text from split */
    struct CSPLIT_FRAGMENT* next;       /* next fragment in the linked list */
    struct CSPLIT_FRAGMENT* prev;       /* previous fragment in the linked list */
} CSplitFragment_t;


/* Struct that stores the csplit linked list */
typedef struct CSPLIT_LIST {
    int num_elems;              /* number of elements in the list */
    size_t BUFF_SIZE;           /* Buffer size given to each fragment parsed */
    CSplitFragment_t* head;     /* head of the linked list (first element) */
    CSplitFragment_t* tail;     /* tail of the linked list (last element) */
} CSplitList_t;


/**
 * Function for printing the csplit errpr cpde string. Only print for non-success code
 * 
 * @params[in]: err -> error code returned by csplit
 * @params[in]: fp  -> output file pointer (typically stderr or stdout)
 */
void print_csplit_error(CSplitError_t err, FILE* fp){
    if(err == CSPLIT_SUCCESS) return;
    const char* err_message;
    switch(err){
        case CSPLIT_TOO_SHORT:
            err_message = "Input string too short";
            break;
        case CSPLIT_NO_SUCH_INDEX:
            err_message = "Index out of range";
            break;
        case CSPLIT_UNIMPLEMENTED:
            err_message = "Unimplemented";
            break;
        case CSPLIT_BUFF_EXCEEDED:
            err_message = "Buffer size exceeded";
            break;
        default:
            err_message = "Unknown Error";
            break;
    }
    fprintf(fp, "**CSPLIT ERROR**: %s\n", err_message);
}


/**
 * Function for initializing a csplit list
 * 
 * @params[in]: buff_size   -> user set buffer size. Make sure this is large enough for your largest fragment
 * @return: list            -> an allocated csplit list
 */
CSplitList_t* csplit_init_list(size_t buff_size){
    CSplitList_t* list = (CSplitList_t*) calloc(1, sizeof(CSplitList_t));
    list->BUFF_SIZE = buff_size;
    list->num_elems = 0;
    return list;
}


/**
 * Clears all memory for an allocated csplit list
 * 
 * @params[in]: list    -> a previously allocated csplit list to be freed
 */
void csplit_clear_list(CSplitList_t* list){
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        CSplitFragment_t* temp = current_fragment->next;
        free(current_fragment);
        current_fragment = temp;
    }
    free(list);
}


/**
 * Function that prints information about a csplit list
 * 
 * @params[in]: list    -> list for which to print info
 * @params[in]: fp      -> file pointer to print into.
 */
void print_csplit_list_info(CSplitList_t* list, FILE* fp){
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        fprintf(fp, "%s\n", current_fragment->text);
        current_fragment = current_fragment->next;
    }
}


/**
 * Function that returns the string fragment at a certain index in the list
 * 
 * @params[in]: list    -> list generated by csplit
 * @params[in]: index   -> index to search for (can be negative for getting at index from back of list)
 * @return: text        -> string at the given index or NULL if index out of range.
 */
char* get_fragment_at_index(CSplitList_t* list, int index){
    int target_index;
    if(index < 0){
        target_index = index + list->num_elems;
    }
    else{
        target_index = index;
    }
    if(list->num_elems <= target_index && target_index < 0){
        return NULL;
    }
    else{
        int counter = 0;
        CSplitFragment_t* current_fragment = list->head;
        while(counter < target_index){
            current_fragment = current_fragment->next;
            counter++;
        }
        return current_fragment->text;
    }
}


/**
 * Function that converts a csplit list into an array if the user requires simpler
 * iteration
 * 
 * @params[in]: list    -> list resulting from a call to csplit
 * @return: output_arr  -> an array of char* (strings) pulled from list, or NULL if num_elems is 0
 */
char** csplit_list_to_array(CSplitList_t* list){
    if(list->num_elems < 1)
        return NULL;
    char** output_arr = (char**) malloc(sizeof(char*) * list->num_elems);
    int counter = 0;
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        output_arr[counter] = current_fragment->text;
        counter++;
        current_fragment = current_fragment->next;
    }
    return output_arr;
}


/**
 * Function that reverses the list generated by csplit
 * 
 * @params[out]: list   -> list to reverse
 * @return: err         -> error code if there is an error
 */
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


/**
 * Function that runs csplit on a particular character. Called if length of token string
 * is 1
 * 
 * @params[out]: list       -> split input string into this list structure
 * @params[in]: input_str   -> input string
 * @params[in]: token       -> character on which to split
 */
CSplitError_t csplit_char(CSplitList_t* list, char* input_str, char token){
    int counter = 0;
    int len = strlen(input_str);
    list->num_elems = 0;
    while(counter < len){
        CSplitFragment_t* fragment = (CSplitFragment_t*) calloc(1, sizeof(CSplitFragment_t));
        fragment->text = (char*) calloc(1, sizeof(list->BUFF_SIZE));
        int fragment_counter = 0;
        if(list->head == NULL){
            list->head = fragment;
            list->tail = fragment;
        }
        else{
            list->tail->next = fragment;
            list->tail = fragment;
        }
        while(input_str[counter] != token && counter < len && input_str[counter] != '\n'){
            fragment->text[fragment_counter] = input_str[counter];
            counter++;
            fragment_counter++;
            if(fragment_counter > list->BUFF_SIZE)
                return CSPLIT_BUFF_EXCEEDED;
        }
        counter++;
        list->num_elems = list->num_elems + 1;
    }
    return CSPLIT_SUCCESS;
}


/**
 * Top level csplit function call. Outputs a csplit list split on a string token. If token is single character,
 * calls csplit_char. If length is less than 1, then input too short error outputted.
 * 
 * @params[out]: output_list    -> output list splitting input str on string token
 * @params[in]: input_str       -> input string which will be split
 * @params[in]: token           -> string on which to split 
 * @return:     err             -> error code if there was a problem with csplitting.
 */
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


/**
 * Function that runs csplit and then reverses the output.
 * 
 * @params[out]: output_list    -> output list splitting input str on string token
 * @params[in]: input_str       -> input string which will be split
 * @params[in]: token           -> string on which to split 
 * @return:     err             -> error code if there was a problem with csplitting.
 */
CSplitError_t rcsplit(CSplitList_t* output_list, char* input_str, char* token){
    CSplitError_t err = csplit(output_list, input_str, token);
    if(err != CSPLIT_SUCCESS)
        return err;
    err = reverse_csplit_list(output_list);
    return err;
}

#endif