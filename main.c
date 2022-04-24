#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#define lineSize 50
#define wordSize 25





int main(int argc, char* argv[]) {

    FILE *input;
    input = fopen(argv[1], "r");

    while(1) {
      char *line = (char*)calloc(lineSize, sizeof(char));
      char **word = (char**)calloc(wordSize, sizeof(char*));
      int wordCount = 0;
      if(!fgets(line, lineSize, input))
        break;
      if(strrchr(line, '\n'))
         *strrchr(line, '\n') = '\0';

      word[wordCount] = strtok(line, " ");
      while (word[wordCount]) {
        wordCount++;
        word[wordCount] = strtok(NULL, " ");
      }

      if(strcmp(word[0], "add_tasks") == 0) {

      }

      if(strcmp(word[0], "get_task") == 0) {

      }

      if(strcmp(word[0], "add_thread") == 0) {

      }

      if(strcmp(word[0], "print") == 0) {

      }

      if(strcmp(word[0], "run") == 0) {

      }

      if(strcmp(word[0], "finish") == 0) {

      }



    }





}
