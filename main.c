#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdio.h>
#include "uthash.h"
#include "search.h"


const char *start = "<list> ";

/* 0 for fail; 1 for success*/
static int tokenize(char *inputStream, char ***toks, int *numtok)
{
   char *tok;
   char *ptr;
   char *ch;
   for(tok=strtok_r(inputStream," \n",&ptr); tok; tok=strtok_r(NULL," \n",&ptr))
   {
       ch=tok;
       while(*ch)
       {
           if(isalnum((int)*ch))
           {
               ch++;
           }
           else
           {
               printf("Bad character.");
               return 0;
           }
       }
       (*numtok)++;
       /*char times numtok*/
       *toks=realloc(*toks,sizeof(char *) * *numtok);
       (*toks)[*numtok-1]=strdup(tok);
   }
   return 1;
}

/*0 for fail; 1 for success*/
int loadword(FILE *file, KeywordTable **keywords)
{
   char *line;
   size_t buffer;
   char *word;
   char *name;
   char *ptr;
   buffer=0;
   while(getline(&line,&buffer,file) > 0)
   {
       if(!strncmp(line, start, 7))
       {
           word=malloc(sizeof(char)*(strlen(line)-6));
           strcpy(word,line+7);
           /*make null character*/
           word[strlen(word)-1]='\0';
           if ((getline(&line,&buffer,file)>0))
           {
               for(name=strtok_r(line," \n",&ptr); name; name=strtok_r(NULL," \n",&ptr))
               {
                   /*add words*/
                   addword(keywords,word,name);
                   name=strtok_r(NULL," \n",&ptr);
               }
           }
           else
           {
               printf("Bad file");
               free(line);
               return 0;
           }
       }
   }
   free(line);
   return 1;
}

int main(int argc, char **argv)
{
   FILE *file;
   KeywordTable *keywords=NULL;
   int num;
   int i;
   char *input;
   size_t buffer;

   /*check inputs*/
   if(argc != 2)
   {
       perror("Error! Incorrect number of arguments.");
       return 1;
   }
   /*puts file in memory*/
   if((file=fopen(argv[1], "r")))
   {
       if (loadword(file,&keywords))
       {
           printf("Failed to load file");
       }
       else
       {
           printf("Succesfully loaded file: ");
       }
   }
   else
   {
       printf("Could not open file");
       return -1;
   }

   buffer=0;
   /*run searches using user input*/
   while(printf("Enter query: "),getline(&input,&buffer,stdin))
   {
       char **files = NULL;
       char **terms = NULL;
       num=0;
       if(!strcmp(input,"q\n"))
       {
           printf("You will now exit the progarm.\n");
           break;
       }
       if (!strncmp(input,"sa ",3))
       {
           printf("Logical AND Query: \n");
           if(tokenize(input+3,&terms,&num))
           {
               printf("Invalid terms!\n");
               continue;
           }
           files=sasearch(keywords,terms,num);
       }
       else if (!strncmp(input,"so ",3))
       {
           printf("Logical OR Query: \n");
           if(tokenize(input+3,&terms,&num))
           {
               printf("Invalid terms! \n");
               continue;
           }
           files=sosearch(keywords,terms,num);
       }
       else
       {
           printf("Invalid input entered! \n");
       }
       if (files)
       {
           for(i=0; files[i]!=NULL; i++)
           {
               printf("%s\n", files[i]);
           }
       }
   }
   free(input);
   fclose(fopen(".page", "w"));
   return 0;
}
