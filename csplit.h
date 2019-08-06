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
 * @defgroup intern Internal
 * @brief Internal functions, not intended to be used by users
 * @defgroup set Setup
 * @brief Setup and Diagnostic functions used by csplit
 * @defgroup core csplit Core
 * @brief Core functions included in csplit
 */

/**
 * A single-header C library for string manipulation and splitting.
 * 
 * Author: Jakub Wlodek
 * Created: 02-Aug-2019
 */


// Include guard - avoid redefinition
#ifndef CSPLIT_H
#define CSPLIT_H

#ifdef __cplusplus
extern "C" {
#endif

// some basic includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _MSC_VER
# define _CSPLIT_FUNC static __inline
#elif !defined __STDC_VERSION__ || __STDC_VERSION__ < 199901L
# define _CSPLIT_FUNC static __inline__
#else
# define _CSPLIT_FUNC static inline
#endif

/**
 * Enum type for error codes for csplit
 * @ingroup set
 */
typedef enum CSPLIT_ERROR {
    CSPLIT_SUCCESS          = 0,     /**< No Error */
    CSPLIT_TOO_SHORT        = -1,    /**< Input string is too short */
    CSPLIT_NO_SUCH_INDEX    = -2,    /**< Index out of range */
    CSPLIT_UNIMPLEMENTED    = -3,    /**< Function unimplemented */
    CSPLIT_BUFF_EXCEEDED    = -4,    /**< Buffer size exceeded */
} CSplitError_t;


/**
 * Struct for an individual string fragment result from csplit
 * @ingroup intern
 */
typedef struct CSPLIT_FRAGMENT {
    char* text;                         /**< Text of the fragment. */
    struct CSPLIT_FRAGMENT* next;       /**< Next fragment in the linked list */
    struct CSPLIT_FRAGMENT* prev;       /**< Previous fragment in the linked list */
} CSplitFragment_t;


/**
 * Struct that stores the csplit linked list. Can be used as an arbitrary linked list
 * for strings, but is intended for use with csplit strtok replacement functions
 * @ingroup core
 */
typedef struct CSPLIT_LIST {
    int num_elems;              /**< Number of elements in the list */
    CSplitFragment_t* head;     /**< Head of the linked list (first element) */
    CSplitFragment_t* tail;     /**< Tail of the linked list (last element) */
} CSplitList_t;


/* Function Declarations */

_CSPLIT_FUNC
CSplitList_t* csplit_init_list();

_CSPLIT_FUNC
void csplit_clear_list(CSplitList_t* list);

_CSPLIT_FUNC
CSplitError_t csplit_push_to_list(CSplitList_t* list, CSplitFragment_t* fragment, size_t buff_size);

_CSPLIT_FUNC
void csplit_print_list_info(CSplitList_t* list, FILE* fp);

#ifdef CSPLIT_DEBUG
_CSPLIT_FUNC
void print_csplit_fragment_info(CSplitFragment_t* fragment, FILE* fp);
#endif

_CSPLIT_FUNC
char* csplit_get_fragment_at_index(CSplitList_t* list, int index);

_CSPLIT_FUNC
CSplitError_t csplit_reverse_list(CSplitList_t* list);

_CSPLIT_FUNC
char* csplit_strip(char* input_str);

_CSPLIT_FUNC
char* csplit_remove_whitespace(char* input_str);

_CSPLIT_FUNC
int csplit_startswith(char* input_str, char* starts_with);

_CSPLIT_FUNC
int csplit_endswith(char* input_str, char* ends_with);

_CSPLIT_FUNC
CSplitError_t csplit_rstr(CSplitList_t* list, char* input_str, char* token, int max_splits);

_CSPLIT_FUNC
CSplitError_t csplit_str(CSplitList_t* list, char* input_str, char* token, int max_splits);

_CSPLIT_FUNC
CSplitError_t csplit_lim(CSplitList_t* list, char* input_str, char* token, int max_splits);

_CSPLIT_FUNC
CSplitError_t csplit(CSplitList_t* list, char* input_str, char* token);

_CSPLIT_FUNC
CSplitError_t rcsplit(CSplitList_t* output_list, char* input_str, char* token);


/* Function Definitions */


/**
 * @brief Function for initializing a csplit list
 * @ingroup set
 * 
 * @params[in]: buff_size   -> user set buffer size. Make sure this is large enough for your largest fragment
 * @return: list            -> an allocated csplit list
 */
_CSPLIT_FUNC
CSplitList_t* csplit_init_list(){
    CSplitList_t* list = (CSplitList_t*) calloc(1, sizeof(CSplitList_t));
    list->num_elems = 0;
    return list;
}


/**
 * @brief Clears all memory for an allocated csplit list
 * @ingroup set
 * 
 * @params[in]: list    -> a previously allocated csplit list to be freed
 */
_CSPLIT_FUNC
void csplit_clear_list(CSplitList_t* list){
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        CSplitFragment_t* temp = current_fragment->next;
        free(current_fragment->text);
        free(current_fragment);
        current_fragment = temp;
    }
    free(list);
}


/**
 * @brief Function that pushes a new CSplitFragment to the end of the list, and allocates memory for the text,
 * with size list->BUFF_SIZE
 * @ingroup intern
 * 
 * @params[out]: list       -> The list with fragment appended to the tail
 * @params[in]: fragment    -> fragment to append to the list. fragment->text will be allocated based on list->BUFF_SIZE
 */
_CSPLIT_FUNC
CSplitError_t csplit_push_to_list(CSplitList_t* list, CSplitFragment_t* fragment, size_t buff_size){
    // first make sure neither is null
    if(list == NULL || fragment == NULL){
        return CSPLIT_TOO_SHORT;
    }
    else{
        // then update the number of elements, and the pointers
        list->num_elems = list->num_elems + 1;
        if(list->head == NULL){
            list->head = fragment;
            list->tail = fragment;
        }
        else{
            list->tail->next = fragment;
            fragment->prev = list->tail;
            list->tail = fragment;
        }
        // allocate fragment text field
        fragment->text = (char*) calloc(1, buff_size);
    }
    return CSPLIT_SUCCESS;
}


/**
 * @brief Function that prints information about a csplit list
 * @ingroup set
 * 
 * @params[in]: list    -> list for which to print info
 * @params[in]: fp      -> file pointer to print into.
 */
_CSPLIT_FUNC
void csplit_print_list_info(CSplitList_t* list, FILE* fp){
    if(list == NULL || fp == NULL) return;
    fprintf(fp, "List contains %d elements\n", list->num_elems);
    fprintf(fp, "Supports indexes -%d to %d.\n", list->num_elems, list->num_elems -1);
    CSplitFragment_t* current_fragment = list->head;
    while(current_fragment != NULL){
        fprintf(fp, "--%s--\n", current_fragment->text);
        current_fragment = current_fragment->next;
    }
}


#ifdef CSPLIT_DEBUG
/**
 * @brief Function that prints information about a csplit fragment - intended for internal use only
 * @ingroup intern
 * 
 * @params[in]: fragment    -> fragment for which to print info
 * @params[in]: fp          -> file pointer to print into
 */
_CSPLIT_FUNC
void print_csplit_fragment_info(CSplitFragment_t* fragment, FILE* fp){
    if(fragment == NULL || fp == NULL) return;
    fprintf(fp, "Fragment has text %s\n", fragment->text);
    if(fragment->next != NULL) fprintf(fp, "Fragment has next with text %s\n", fragment->next->text);
    if(fragment->prev != NULL) fprintf(fp, "Fragment has prev with text %s\n", fragment->prev->text);    
}
#endif


/**
 * @brief Function that returns the string fragment at a certain index in the list
 * @ingroup core
 * 
 * @params[in]: list    -> list generated by csplit
 * @params[in]: index   -> index to search for (can be negative for getting at index from back of list)
 * @return: text        -> string at the given index or NULL if index out of range.
 */
_CSPLIT_FUNC
char* csplit_get_fragment_at_index(CSplitList_t* list, int index){
    // convert index into absolute index (if negative);
    int target_index;
    if(index < 0){
        target_index = index + list->num_elems;
    }
    else{
        target_index = index;
    }
    // if index is out of range return null
    if(list->num_elems <= target_index || target_index < 0){
        return NULL;
    }
    else{
        // iterate over list until index found
        int counter = 0;
        CSplitFragment_t* current_fragment = list->head;
        while(counter < target_index){
            current_fragment = current_fragment->next;
            counter++;
        }
        // return the text field
        return current_fragment->text;
    }
}


/**
 * @brief Function that reverses the list generated by csplit
 * @ingroup set
 * 
 * @params[out]: list   -> list to reverse
 * @return: err         -> error code if there is an error
 */
_CSPLIT_FUNC
CSplitError_t csplit_reverse_list(CSplitList_t* list){
    int i;
    // iterate over list and swap next and previous fields
    CSplitFragment_t* temp = list->head;
    for(i = 0; i < list->num_elems; i++){
        CSplitFragment_t* temp2 = temp->next;
        temp->next = temp->prev;
        temp->prev = temp2;
        temp = temp2;
    }
    // swap head and tail
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
    return CSPLIT_SUCCESS;
}


/**
 * @brief Function that strips a given string into an output string. Will remove whitespace character:
 * \n, \r, \t, space will be removed from the start and end of each string.
 * @ingroup core
 * 
 * @params[in]: input_str   -> the input string to strip
 * @return: output_str      -> the string with whitespace removed from the ends. Must be freed.
 */
_CSPLIT_FUNC
char* csplit_strip(char* input_str){
    char* output_str;
    // check if input is null
    if(input_str == NULL)
        output_str = NULL;
    else{
        int len = strlen(input_str);
        char* end = input_str + len - 1;
        char* start = input_str;
        while(isspace(*start)){
            if(*start == '\0' || start == end) return NULL;
            else start++;
        }
        while(isspace(*end))
            end--;

        size_t buff_size = end - start + 1;
        output_str = (char*) calloc(1, buff_size + 1);
        strncpy(output_str, start, buff_size);
    }
    return output_str;
}


/**
 * @breifFunction that removes all whitespace characters of a given string into an output string.
 * Note that resulting char* must be free'd after it is no longer used
 * @ingroup core
 * 
 * @params[in]: input_str   -> the input string to strip
 * @return: output_str      -> the string with whitespace removed.
 */
_CSPLIT_FUNC
char* csplit_remove_whitespace(char* input_str){
    char* output_str;
    if(input_str == NULL)
        output_str = NULL;
    else{
        int len = strlen(input_str);
        output_str = (char*) calloc(1, len);
        int counter = 0;
        int output_counter = 0;
        // read through but don't copy whitespace
        while(counter < len){
            if(!isspace(input_str[counter])){
                output_str[output_counter] = input_str[counter];
                output_counter++;
            }
            counter++;
        }
    }
    return output_str;
}


/**
 * @brief Function that checks if a given string starts with another given string.
 * @ingroup core
 * 
 * @params[in]: input_str       -> string to check against
 * @params[in]: starts_with     -> string to try to match with start of input string
 * @return:     int             -> -2 if input is invalid, -1 if doesn't start with given string, or 0 if it does
 */
_CSPLIT_FUNC
int csplit_startswith(char* input_str, char* starts_with){
    if(input_str == NULL || starts_with == NULL) return -2;
    else if(strstr(input_str, starts_with) == input_str) return 0;
    else return -1;
}


/**
 * @brief Function that checks if a given string ends with another given string.
 * @ingroup core
 * 
 * @params[in]: input_str       -> string to check against
 * @params[in]: ends_with       -> string to try to match with end of input string
 * @return:     int             -> -2 if input is invalid, -1 if doesn't end with given string, or 0 if it does
 */
_CSPLIT_FUNC
int csplit_endswith(char* input_str, char* ends_with){
    if(input_str == NULL || ends_with == NULL){
        return -2;
    }
    else{
        int ewith_len = strlen(ends_with);
        int istr_len = strlen(input_str);
        if(ewith_len > istr_len) return -1;
        else if(strcmp(input_str + istr_len - ewith_len, ends_with) == 0) return 0;
        else return -1;
    }
}


/**
 * @brief Function that runs csplit on a particular string from the end of the input. Called if max_splits < 0
 * @ingroup intern
 * 
 * @params[out]: list       -> split input string into this list structure
 * @params[in]: input_str   -> input string
 * @params[in]: token       -> character on which to split
 * @params[in]: max_splits  -> maximum number of splits. If negative will split from end of string
 * @return:     err             -> error code if there was a problem with csplitting.
 */
_CSPLIT_FUNC
CSplitError_t csplit_rstr(CSplitList_t* list, char* input_str, char* token, int max_splits){
    CSplitError_t err = CSPLIT_SUCCESS;
    int in_len = strlen(input_str);
    int counter = 0;
    int num_splits = 0;
    int token_len = strlen(token);
    char* current_location = input_str;
    char* arr[in_len];
    while((arr[counter] = strstr(current_location, token)) != NULL){
        current_location = arr[counter] + 1;
        counter++;
    }
    char* last_location = input_str + in_len;
    while(counter >= 0 && num_splits >= max_splits){
        current_location = arr[counter - 1] + token_len;
        CSplitFragment_t* fragment = (CSplitFragment_t*) calloc(1, sizeof(CSplitFragment_t));
        size_t req_buff_size;
        if(num_splits == max_splits || counter == 0){
            req_buff_size = in_len - strlen(last_location);
            current_location = input_str;
        }
        else
            req_buff_size = last_location - current_location;
        err = csplit_push_to_list(list, fragment, req_buff_size);
        strncpy(fragment->text, current_location, req_buff_size);
        last_location = current_location - token_len;
        num_splits--;
        counter--;
    }
    err = csplit_reverse_list(list);
    return err;
}


/**
 * @brief Function that splits a given input string based on another string.
 * @ingroup intern
 * 
 * @params[out]: list           -> output list splitting input str on string token
 * @params[in]: input_str       -> input string which will be split
 * @params[in]: token           -> string on which to split
 * @params[in]: max_splits      -> max number of splits to perform. Negative if starting from end of string.
 * @return:     err             -> error code if there was a problem with csplitting.
 */
_CSPLIT_FUNC
CSplitError_t csplit_str(CSplitList_t* list, char* input_str, char* token, int max_splits){
    if(max_splits < 0)
        return csplit_rstr(list, input_str, token, max_splits);
    CSplitError_t err = CSPLIT_SUCCESS;
    int in_len = strlen(input_str);
    int num_splits = 0;
    int token_len = strlen(token);
    char* current_location = input_str;
    char* next_location;

    while(current_location != NULL && num_splits <= max_splits){
        CSplitFragment_t* fragment = (CSplitFragment_t*) calloc(1, sizeof(CSplitFragment_t));
        next_location = strstr(current_location, token);
        size_t req_buff_size;
        if(next_location == NULL || (num_splits == max_splits))
            req_buff_size = in_len - (current_location - input_str);
        else if(next_location != NULL)
            req_buff_size = next_location - current_location;
        err = csplit_push_to_list(list, fragment, req_buff_size);
        strncpy(fragment->text, current_location, req_buff_size);
        if(next_location != NULL)
            current_location = next_location + token_len;
        else{

            current_location = next_location;
        }
        num_splits++;
    }
    return err;
}



/**
 * @brief Function that allows user to split based on a limited number of splits, either forward
 * or in reverse. A max_splits >= len(input_str) will guarantee all possible splits
 * @ingroup core
 * 
 * @params[out]: list           -> output list splitting input str on string token
 * @params[in]: input_str       -> input string which will be split
 * @params[in]: token           -> string on which to split
 * @params[in]: max_splits      -> max number of splits to perform. Negative if starting from end of string.
 * @return:     err             -> error code if there was a problem with csplitting.
 */
_CSPLIT_FUNC
CSplitError_t csplit_lim(CSplitList_t* list, char* input_str, char* token, int max_splits){
    CSplitError_t err = CSPLIT_SUCCESS;
    // first check to see if input string is valid and token is valid
    if(strlen(input_str) < 1 || strlen(token) < 1){
        err = CSPLIT_TOO_SHORT;
    }
    else{
        // otherwise we split on the given string
        err = csplit_str(list, input_str, token, max_splits);
    }
    return err;
}


/**
 * @brief Top level csplit function call. Outputs a csplit list split on a string token. Calls
 * csplit_lim with max_splits = len(input_str), ensuring that all possible splits will be made.
 * @ingroup core
 * 
 * @params[out]: list           -> output list splitting input str on string token
 * @params[in]: input_str       -> input string which will be split
 * @params[in]: token           -> string on which to split 
 * @return:     err             -> error code if there was a problem with csplitting.
 */
_CSPLIT_FUNC
CSplitError_t csplit(CSplitList_t* list, char* input_str, char* token){
    return csplit_lim(list, input_str, token, (int) strlen(input_str));
}


/**
 * @brief Function that runs csplit and then reverses the output.
 * @ingroup core
 * 
 * @params[out]: output_list    -> output list splitting input str on string token
 * @params[in]: input_str       -> input string which will be split
 * @params[in]: token           -> string on which to split 
 * @return:     err             -> error code if there was a problem with csplitting.
 */
_CSPLIT_FUNC
CSplitError_t rcsplit(CSplitList_t* output_list, char* input_str, char* token){
    CSplitError_t err = csplit(output_list, input_str, token);
    if(err != CSPLIT_SUCCESS)
        return err;
    err = csplit_reverse_list(output_list);
    return err;
}

#ifdef __cplusplus
}
#endif

#endif