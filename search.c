#include <stdio.h>
#include "search.h"

int addword(KeywordTable **keywords,char *word,char *name)
{
   char *check;
   KeywordTable *temp;
   KeywordTable *table;
   check=strdup(word);
   temp=NULL;
   table=NULL;
   HASH_FIND_STR(*keywords,word,temp);
   if(temp)
   {
       if(!(temp->files=realloc(temp->files,(temp->wordcount+1)*sizeof(char*))))
       {
           perror("Error! Memory wasnt't allocated.");
           return 1;
       }
       temp->files[temp->wordcount]=strdup(name);
       temp->wordcount++;
   }
   else
   {
       table=NULL;
       if (!(table=calloc(1,sizeof(KeywordTable))))
       {
           perror("Error! Memory wasnt't allocated.");
           return 1;
       }
       table->word=check;
       table->files=malloc(sizeof(char **));
       *(table->files)=strdup(name);
       table->wordcount=1;
       HASH_ADD_KEYPTR(hh,*keywords,table->word,strlen(table->word),table);
   }
   return 0;
}
/*must contain all of the files*/
char **saSearch(KeywordTable *keywords,char **terms,size_t wordcount)
{
   ResultTable *result;
   ResultTable *temp;
   ResultTable *temp2;
   KeywordTable *table;
   int i;
   int j;
   if (!keywords||!terms)
   {
       return NULL;
   }
   HASH_FIND_STR(keywords,*terms,table);
   if (!table)
   {
       return NULL;
   }
   char **files=table->files;
   for (i=0; i<table->wordcount; i++)
   {
       temp=calloc(1, sizeof(ResultTable));
       temp->file=strdup(files[i]);
       temp->wordcount=1;
       HASH_ADD_KEYPTR(hh,result,temp->file,strlen(temp->file),temp);
   }

   for (i=1; i<wordcount; i++)
   {
       char *check=terms[i];
       HASH_FIND_STR(keywords,check,table);
       for (j=0; j<table->wordcount; j++)
       {
           HASH_FIND_STR(result,table->files[j],temp);
           if (temp)
           {
               temp->wordcount++;
           }
       }
   }

   files=calloc(HASH_COUNT(result)+1,sizeof(char*));
   i=0;
   for (temp2=result; temp2!=NULL; temp2=temp2->hh.next)
   {
       if (temp2->wordcount==wordcount)
       {
           files[i++]=strdup(temp2->file);
       }
       HASH_DEL(result,temp2);
       free(temp2->file);
       free(temp2);
   }
   return files;
}
/*must contain 1 of the terms*/
char **sosearch(KeywordTable *keywords,char **terms,size_t wordcount)
{
   ResultTable *result;
   ResultTable *temp2;
   ResultTable *temp;
   KeywordTable *table;
   int i;
   int j;
   if (!keywords||!terms)
   {
       return NULL;
   }
   for(i=0; i<wordcount; i++)
   {
       char *check=terms[i];
       HASH_FIND_STR(keywords,check,table);
       if (!table)
       {
           continue;
       }
       char **files=table->files;
       for(j=0; j<table->wordcount; j++)
       {
           char *fin=files[j];
           HASH_FIND_STR(result,fin,temp);
           if(!temp)
           {
               temp=calloc(1, sizeof(ResultTable));
               temp->file=fin;
               HASH_ADD_KEYPTR(hh,result,temp->file,strlen(temp->file),temp);
           }
           else
           {
               temp->wordcount++;
           }
       }
   }
   char **files=calloc(HASH_COUNT(result)+1,sizeof(char*));
   i=0;
   for (temp2=result; temp2!=NULL; temp2=temp2->hh.next,i++)
   {
       files[i]=strdup(temp2->file);
       HASH_DEL(result, temp2);
       free(temp2);
   }
   return files;
}
