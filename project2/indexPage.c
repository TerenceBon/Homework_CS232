/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ALPHABETS 26 // Size of alphabets
#define MAX_CHARACTERS 30000 //Max characters allowed

/* TODO: structure definitions */

struct trieNode{
 
  struct trieNode *children[MAX_ALPHABETS];
  int count;
  int Ending;

};

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
struct trieNode* indexPage(const char* url);

int addWordOccurrence(const char* word, const int wordLength, 
struct trieNode* node, int counter);

void printTrieContents(struct trieNode *node, char * words, int link);

int freeTrieMemory(struct trieNode* node);

int getText(const char* srcAddr, char* buffer, const int bufSize);

struct trieNode* createNode();

int main(int argc, char** argv){

  char tempString[MAX_CHARACTERS];

  if(argc < 2){
    fprintf(stderr, "USAGE: %s Url argument", argv[0]);
    return 1;
  }  
  struct trieNode * root = indexPage(argv[1]);

  if(root == NULL){
    return 2;
  }
  
  printTrieContents(root, tempString, 0);
  freeTrieMemory(root);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
struct trieNode* indexPage(const char* url)
{

  char buffer[MAX_CHARACTERS];
  int length = 0;
  int j = 0;
  int wordS = 0;
  int wordE = 0;
  char word[MAX_CHARACTERS];

  if(getText(url, buffer, MAX_CHARACTERS) == 0){

    return NULL;

  }

  length = strlen(buffer);

  struct trieNode* node = createNode();

  if(node == NULL){

    return NULL;

  }

  printf("%s\n", url);

  for(int i = 0; i < length; i++){
    if((buffer[i] != ' ') || (buffer[i] != ',') || (buffer[i] != '\n') ||
    (buffer[i] != '\r') || (buffer[i] != '\t') || (buffer[i] != '\'')){

      if(isalpha(buffer[i])){

       word[j] = tolower(buffer[i]);
       wordS = 1;

      }else if(wordS == 1){

        word[j] = '\0';
        wordE = 1;

      }
    }
    else if(wordS == 1){

      word[j] = '\0';
      wordE = 1;

    }
    if(wordE == 0 && wordS == 1){

      j++;

    }
    else if(wordE == 1 && wordS == 1){

        addWordOccurrence(word, strlen(word), node, 0);
        printf("\t%s\n", word);

        wordE = 0;
        wordS = 0;

        word[0] = '\0';
        j = 0;

    }

  }

  return node;
}

struct trieNode* createNode(){
  
  struct trieNode* node = malloc(sizeof(struct trieNode));

  if(node == NULL){

    fprintf(stderr, "ERROR: could not allocate memory\n");

    return NULL;
  }

  for(int i = 0 ; i < MAX_ALPHABETS; i++){

    node -> children[i] = NULL;
    node -> Ending = 1;
    node -> count = 0;

  }
  return node;
}


int addWordOccurrence(const char* word, const int wordLength, 
struct trieNode* node, int counter)
{

  int indexOfChar;

  if(counter < wordLength){
    indexOfChar = word[counter] - 'a';

    if(node -> children[indexOfChar] == NULL){
      node -> children[indexOfChar] = createNode();

      if(node -> children[indexOfChar] == NULL){
        return 3;

      }
    }

    addWordOccurrence(word, wordLength, node->children[indexOfChar], counter+1);

  }else{

    node -> Ending = 0;
    node -> count++;

  }

  return 0;

}

void printTrieContents(struct trieNode *node, char *temp, int link)
{

  int countOccurance = 0;
  int done = 0;

  if(node == NULL){

    return;

  }

  if(node -> Ending == 0 || node -> children[link]){

    temp[link] = '\0';
    countOccurance = node -> count;
    done = 1;

  }

  if(node -> Ending == 0 && done == 1){

    printf("%s: %d\n", temp, countOccurance);

  }

  for(int i = 0; i < MAX_ALPHABETS; i++){
    if(node -> children[i] != NULL){

      temp[link] = i+'a';
      printTrieContents(node -> children[i], temp, link+1);

    }
  }
  
}


int freeTrieMemory(struct trieNode* t)
{
  for (int i = 0; i < MAX_ALPHABETS; i++){
    if(t->children[i] != NULL){
    freeTrieMemory(t->children[i]);
    }
  
  }
  free(t);
  return 0;
}


int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}




