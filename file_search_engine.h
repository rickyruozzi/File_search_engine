#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
/* #define FILENAME_MAX 100 - using std FILENAME_MAX */

typedef struct wordentry {
    char *word; // word
    struct fileNode *files; // file with the word
    struct wordentry *next; //next word
} wordentry;

typedef struct fileNode {
    char filename[260];
    int count; //word occurence in the file
    struct fileNode *next; //next fileNode
} fileNode;

typedef struct index {
    wordentry *head; //heads of the words list
} index;

index* init_index(void);
void scan_directory(index* index, const char* path);
void index_file(index* Index, const char *filename);
void normalize_word(char *word);
void add_word(index* Index, const char* word, const char *filename);
wordentry* search_word(index *Index, const char* word);
void printFile(index *Index, const char* word);
void free_index(index *Index);
int count_total_words(index* Index);
