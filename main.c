#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include "TCoada-L.h"
#include "TStiva-L.h"
#define lineSize 50
#define wordSize 25


typedef struct {

  u_short id;

} thread;

typedef struct {

  u_short id;
  thread trd;
  u_int8_t prio;
  u_int time;

} task;


void *init(u_short cuanta, u_short cores) {
  u_short tCount = 2 * cores;
  void *tStack =  InitS(sizeof(thread), tCount);
  thread temp;
  temp.id = 0;
  while(temp.id < tCount) {
    Push(tStack, &temp);
    temp.id++;
  }
}


int main(int argc, char* argv[]) {

    FILE *input;
    input = fopen(argv[1], "r");
    u_short cuanta = 0;
    u_short cores = 0;
    fscanf(input, "%hu\n", &cuanta);
    fscanf(input, "%hu\n", &cores);
    if (!cuanta && !cores) {

    }
  //  printf("%hu %hu\n", cuanta, cores);
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
        u_short nrTaskuri = atoi(word[1]);
        u_int time = atoi(word[2]);
        u_int8_t prio = atoi(word[3]);
      //  printf("%hu %u %"PRId8"\n", nrTaskuri, time, prio);
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
