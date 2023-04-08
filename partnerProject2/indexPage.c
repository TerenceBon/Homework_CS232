/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_WORD_LEN 50



/* TODO: structure definitions */

typedef struct TrieNode{
  int count;
  struct TrieNode* children[26];
}TrieNode;

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
void indexPage(const char* url);

int addWordOccurrence(const char* word, const int wordLength, TrieNode* root);

void printTrieContents(TrieNode* root);

int freeTrieMemory(TrieNode* root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv){
  // TODO: write the (simple) main function

  // argv[1] will be the URL to index, if argc > 1

//   const char* url = argc > 1 ? argv[1] : "http://www.example.com/";

//   /* Index the page */
//     indexPage(url);

//   return 0;
// }
  if (argc <= 1) {
        printf("Error: please provide a URL to index.\n");
        return 1;
      }
    indexPage(argv[1]);
    return 0;
}


/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
void indexPage(const char* url){
  char textBuffer[1024];
  TrieNode* root = (TrieNode*) calloc(1, sizeof(TrieNode));
  int bytesRead = getText(url, textBuffer, sizeof(textBuffer));

  /*parsing the text to count the occurrence*/
  for(int i = 0; i < bytesRead; i++){
    int wordStart = i;
    int wordLen = 0;

    /*finding the end of current word*/
    while(i < bytesRead && textBuffer[i] != ' ' && textBuffer[i] != '\n'){
      i++;
      wordLen++;
    }

    /*Adding the current word to index*/
    if(wordLen > 0){
      char word[MAX_WORD_LEN];
    strncpy(word, &textBuffer[wordStart], wordLen);
    word[wordLen] = '\0';
    addWordOccurrence(word, wordLen, root);
    }
  }

  printTrieContents(root);
  freeTrieMemory(root);
}

int addWordOccurrence(const char* word, const int wordLength, TrieNode* root){
  TrieNode* node = root;

  // Traversing

  for(int i = 0; i < wordLength; i++){
    int c = word[i] - 'a';

    if(!node -> children[c]){

      node -> children[c] = (TrieNode*) calloc(1, sizeof(TrieNode));

    }

    node = node -> children[c];
  }

  node -> count++;

  return node -> count;

}

void printTrieContents(TrieNode* root){

  if(!root){
    return;
  }

  for(int i = 0; i < 26; i++){
    if(root -> children[i]){
      printf("%c - %d\n", 'a' + i, root -> children[i] -> count);
      printTrieContents(root -> children[i]);
    }

  }

}


int freeTrieMemory(TrieNode* root){

  if(!root){
    return 0;
  }

  int totalCount = root -> count;

  for(int i = 0; i < 26; i++){
    if(root -> children[i]){
      totalCount += freeTrieMemory(root -> children[i]);
    }
  }

  free(root);
  return totalCount;

}


/* You should not need to modify this function */
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