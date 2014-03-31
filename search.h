#ifndef SEARCH_H
#define SEARCH_H
#include "uthash.h"

typedef struct {
   char *word;
   char **files;
   size_t wordcount;
   UT_hash_handle hh;
} KeywordTable;

typedef struct {
   char *file;
   size_t wordcount;
   UT_hash_handle hh;
} ResultTable;

int addword(KeywordTable **keywords,char *word,char *name);
char **sosearch(KeywordTable *keywords,char **terms,size_t wordcount);
char **sasearch(KeywordTable *keywords,char **terms,size_t wordcount);

#endif // SEARCH_H
