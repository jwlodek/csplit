<p align="center">
    <img src="assets/csplit_logo.png">
</p>

# csplit ![](https://travis-ci.org/jwlodek/csplit.svg?branch=master)

A single header C library that can be used to easily handle splitting strings and other string operations in C.

### Why does it exist?

I wrote csplit because I found string manipulation in C to be very difficult, particularly the `strtok` built in function. I have successfully used csplit in several projects, primarily for parsing input files, and I hope it proves to be of use to you.

### What is it?

`csplit` is intended to be a C string manipulation library that attempts to have a similar syntax as pythonic string functions. 

For example, passing a negative index into a csplit function will simply perform whatever operation was selected starting from the rearmost index. The primary usage for csplit is as a replacement for `strtok`, though it also includes several other string manipulation functions.

For my use cases, I have used csplit for parsing `.csv` files, custom `CONFIGURE` files I write, and other things.

`csplit` was written as a single header C library to simplify it's inclusion into other projects I have written. This may not be the most efficient way of writing this, seeing as csplit does contain quite a few functions, but I found that this implementation made using it for the small projects I work on that require some string manipulation to be much easier.

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

In order to use these core functions, we must first initialize a `CSplitList_t` struct:
```C
CSplitList_t* list = csplit_init_list();
```
This will create a doubly linked list data structure that will be used to internally store the split string fragments. Once this struct is allocated, we may call one of the `csplit` core functions listed above. This will populate the linked list with `CSplitFragment_t` structures that contain the string fragments found by splitting.

In order to access these fragments, we can use the
```C
char* csplit_get_fragment_at_index(CSplitList_t* list, int index);
```
function. Once again, as is the case in python, passing a negative number will net the string fragment starting from the rear of the list. For example, an index of -1 would be the last fragment, -2 would be the second to last fragment, etc.

Once we are done with the split fragments and our list, we must free up the memory associated with it:
```C
void csplit_clear_list(CSplitList_t* list);
```

For more detailed sample code using the `csplit` library, make sure to take a look at the Examples tab of this documentation.