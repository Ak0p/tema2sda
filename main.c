#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include "TCoada-V.h"
#include "TStiva-V.h"
#define lineSize 50
#define wordSize 25
#define min(a,b) (a <= b ? a : b)


typedef struct {

  u_short id;

} thread;

typedef struct {

  u_short id;
  thread trd;
  u_int8_t prio;
  int total_time;
  int left_time;

} task;


u_short giveID(u_int **v) {
  int k = 0;
  int l = 31;
  for(; k < (1 << 10); k++) {
    for(l = 31; l >= 0; l--) {
      int temp = ((*v)[k] & (1 << l)) >> l;
      if(temp == 0) {
        (*v)[k] |= (1 << l);
      //  printf("Give k=%d l=%d\n", k, l);
        return k*32 + (31-l+1);
      }
    }
  }
  return -1;
}

void takeID(u_int **v, u_short id) {
      int k = id / 32;
      int l = 31 - id % 32;
    //  printf("Take id = %d k = %d l = %d\n", id, k, l);
      (*v)[k] &= (1 << l);
}

int cmpTask(task *task1, task* task2) {
  if( task1->prio < task2->prio)
    return 1;
  else if (task1->prio > task2->prio)
    return 0;
  else {
    if (task1->left_time > task2->left_time)
      return 1;
    else if (task1->left_time < task2->left_time)
      return 0;
    else {
      if (task1->id > task2->id)
        return 1;
      else
        return 0;
    }
  }
}

void sortQ(void **q, int maxTask) {

    if (VidaQ(*q))
      return;
    void *aux = InitQ(sizeof(task), maxTask);
    task *min = (task*)calloc(1, sizeof(task));
    task *temp = (task*)calloc(1, sizeof(task));
    void *final= InitQ(sizeof(task), maxTask);
    while (!VidaQ(*q)) {
    //  printf( "Aci mare\n");
      ExtrQ(*q, min);
      while (!VidaQ(*q)) {
        ExtrQ(*q, temp);
        if (cmpTask(min, temp) == 1) {
          task *schimb = (task*)calloc(1, sizeof(task));
          memcpy(schimb, min, sizeof(task));
          memcpy(min, temp, sizeof(task));
          memcpy(temp, schimb, sizeof(task));
          free(schimb);
        }
        IntrQ(aux, temp);
      }
        IntrQ(final, min);
      //  printf( "%d %d %"PRId8" %u %u\n", min->id, min->trd.id, min->prio, min->total_time, min->left_time);
        while (!VidaQ(aux)) {
      //    printf( "Aci mic\n");
          ExtrQ(aux, temp);
        //  printf( "%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);
          IntrQ(*q, temp);
      }
    }
    while (!VidaQ(final)) {
    //  printf( "Aci final\n");
      ExtrQ(final, temp);
    //  printf( "%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);
      IntrQ(*q, temp);
    }
  //  printf( "E\n");
    free(temp);
    free(min);
  }

  void sortS(void **q, int a) {
      if (VidaS(*q))
        return;
      // printf("SortS\n");
      void *aux = InitS(sizeof(thread), a);
      thread *minim = (thread*)calloc(1, sizeof(thread));
      thread *temp = (thread*)calloc(1, sizeof(thread));
      void *final= InitS(sizeof(thread), a);
      while (!VidaS(*q)) {

      //  printf( "Aci mare\n");
        Pop(*q, minim );
        while (!VidaS(*q)) {
          Pop(*q, temp);
          if (temp->id < minim->id) {
            thread *schimb = (thread*)calloc(1, sizeof(thread));
            memcpy(schimb, minim, sizeof(thread));
            memcpy(minim , temp, sizeof(thread));
            memcpy(temp, schimb, sizeof(thread));
            free(schimb);
          }
          Push(aux, temp);
        }
          Push(final, minim);
        //  printf( "%d %d %"PRId8" %u %u\n", min->id, min->trd.id, min->prio, min->total_time, min->left_time);
          while (!VidaS(aux)) {
        //    printf( "Aci mic\n");
            Pop(aux, temp);
          //  printf( "%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);
            Push(*q, temp);
        }
      }
      while (!VidaS(final)) {
      //  printf( "Aci final\n");
        Pop(final, temp);
      //  printf( "%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);
        Push(*q, temp);
      }
    //  printf( "E\n");
      free(temp);
      free(minim);
    }


void *setupStiva(int a) {
  void *thStack =  InitS(sizeof(thread), a);
  int trdId = a - 1;
  while(trdId >= 0) {
    thread *temp = (thread*)calloc(1, sizeof(thread));
    temp->id = trdId;
    Push(thStack, temp);
    trdId--;
    // printf( "Bfr %d id \n", temp->id);
    // Top(thStack, temp);
    // printf( "After %d id \n", temp->id);
  }
    return thStack;
}


void get_task(void **waitingQ, void **runningQ, void **finishedQ, short id, int maxTask, FILE *output) {
  void *aux = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));
  int8_t cond = 0;

//printf( "Func call id %d.\n", id);
  while (!VidaQ(*runningQ)) {
    printf( "roning %d.\n", id);
    ExtrQ(*runningQ, temp);
    if (temp->id == id) {
     printf( "Task %d is running (remaining_time = %u).\n"
      ,temp->id, temp->left_time);
      cond = 1;
    }
    IntrQ(aux, temp);
  }
  while (!VidaQ(aux)) {
    ExtrQ(aux, temp);
     //printf( "Func jos %d %d %"PRId8" %u\n ",temp->id,temp->trd.id,temp->prio,temp->left_time);
    IntrQ(*runningQ, temp);
  }
  if (cond == 1)
    return;

  while (!VidaQ(*waitingQ)) {
  //  printf( "waiting %d.\n", id);
      ExtrQ(*waitingQ, temp);
    //  printf( "Func waiting ||%d|| |%"PRId8"| %d  %u\n",temp->id, temp->prio, temp->trd.id,temp->left_time);
      if (temp->id == id) {
        printf( "Task %d is waiting (remaining_time = %u).\n"
        ,temp->id, temp->left_time);
        cond = 2;
      }
      IntrQ(aux, temp);
    }
  while(!VidaQ(aux)) {
      ExtrQ(aux, temp);
    //  printf( "Func jos %d %d %"PRId8" %u\n ",temp->id,temp->trd.id,temp->prio,temp->left_time);
      IntrQ(*waitingQ, temp);
  }
    if (cond == 2)
      return;

    while (!VidaQ(*finishedQ)) {
    //  printf( "roning %d.\n", id);
        ExtrQ(*finishedQ, temp);
        if (temp->id == id) {
      printf( "Task %d is finished (executed_time = %u).\n"
        ,temp->id, temp->total_time);
          cond = 3;
        }
        IntrQ(aux, temp);
      }
      while (!VidaQ(aux)) {
        ExtrQ(aux, temp);
         //printf( "Func jos %d %d %"PRId8" %u\n ",temp->id,temp->trd.id,temp->prio,temp->left_time);
        IntrQ(*runningQ, temp);
      }
      if (cond == 3)
        return;

    if (cond == 0)
      printf( "Task %d not found.\n", id);


}

void get_thread(void **runningQ, u_short id, int maxTask, int N, FILE *output) {
  int8_t idle = 1;
  void *auxQ = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));

  while (!VidaQ(*runningQ)) {
      ExtrQ(*runningQ, temp);
      if (temp->trd.id == id) {
         printf( "Thread %d is running task %d (remaining_time = %d).\n"
         ,temp->trd.id ,temp->id, temp->left_time);
         idle = 0;
      }
      IntrQ(auxQ, temp);
    }
    while (!VidaQ(auxQ)) {
      ExtrQ(auxQ, temp);
      IntrQ(*runningQ, temp);
    }
    DistrQ(&auxQ);
    free(temp);

    if(idle && id < N) {
      printf( "Thread %hu is idle.\n",id);
      return;
    }

}

void print(void **q, int8_t mode, int maxTask, FILE *output) {

if (mode == 0) {
  printf( "====== Waiting queue =======\n");
  printf( "[");
  void *auxQ = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*q)) {
        ExtrQ(*q, temp);
        if (VidaQ(*q)) {
          printf( "(%d: priority = %d, remaining_time = %d)"
          , temp->id, temp->prio, temp->left_time);
        } else {
        printf( "(%d: priority = %d, remaining_time = %d),\n"
        , temp->id, temp->prio, temp->left_time);
        }
        IntrQ(auxQ, temp);
      }
  printf( "]\n");
  while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, temp);
    //    printf( "Print ||%d|| |%"PRId8"| %d  %u\n ",temp->id, temp->prio,temp->trd.id,temp->left_time);
        IntrQ(*q, temp);
      }
  DistrQ(&auxQ);
  free(temp);

} else if (mode == 1) {

  printf( "====== Running in parallel =======\n");
  printf( "[");
  void *auxQ = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*q)) {
        ExtrQ(*q, temp);
      if (VidaQ(*q)) {
        printf( "(%d: priority = %d, remaining_time = %d, running_thread = %d)"
          ,temp->id, temp->prio, temp->left_time, temp->trd.id);
      } else {
      printf( "(%d: priority = %d, remaining_time = %d, running_thread = %d),\n"
        ,temp->id, temp->prio, temp->left_time, temp->trd.id);
      }
        IntrQ(auxQ, temp);
      }

  printf( "]\n");
  while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, temp);
        IntrQ(*q, temp);
      }
  DistrQ(&auxQ);
  free(temp);

} else if (mode == 2) {

  printf( "====== Finished queue =======\n");
  printf( "[");
  void *auxQ = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*q)) {
        ExtrQ(*q, temp);
        if (VidaQ(*q)) {
          printf( "(%d: priority = %d, executed_time = %d)"
          , temp->id, temp->prio, temp->total_time);
        } else {
        printf( "(%d: priority = %d, executed_time = %d),\n"
        , temp->id, temp->prio, temp->total_time);
        }
        IntrQ(auxQ, temp);
      }
  printf( "]\n");
  while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, temp);
        IntrQ(*q, temp);
      }
  DistrQ(&auxQ);
  free(temp);

}
}

int run(int timp, u_short Q, int maxTask, int N, u_int **vectID, void **waitingQ, void **runningQ, void **finishedQ, void **s, FILE *output) {
  void *auxQ = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));
  int leftThreads = N;
  int execTimp = 0;
  int maxTimp = -1;

  if (VidaQ(*waitingQ))
    return execTimp;

  while (!VidaQ(*waitingQ) && leftThreads > 0) {
    ExtrQ(*waitingQ, temp);
    IntrQ(*runningQ, temp);
    leftThreads--;
  }

  printf("Start\n");
  // subtract
  while (timp > 0) {
    while (!VidaQ(*runningQ)) {
      ExtrQ(*runningQ, temp);
  //    printf("Current id = %hd prio = %"PRId8" \n", temp->id, temp->prio);
      if (maxTimp < temp->left_time) {
        maxTimp = temp->left_time;
      }
      temp->left_time -= min(timp, Q);
      if (temp->left_time < 0) {
        temp->left_time = 0;
      }
      IntrQ(auxQ, temp);

    }
    while (!VidaQ(auxQ)) {
      ExtrQ(auxQ, temp);
      IntrQ(*runningQ, temp);
      leftThreads--;
    }

  //  printf("Check -----%d\n\n", VidaQ(*runningQ));
    // PrimQ(*runningQ, temp);
    //printf("Testare %d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);

    // check

    while (!VidaQ(*runningQ)) {
      ExtrQ(*runningQ, temp);
      if (temp->left_time == 0) {
        thread *tempThread = (thread*)calloc(1, sizeof(thread));
        tempThread->id = temp->trd.id;
      //  printf("Func Take id = %hd \n", temp->id);
        Push(*s, tempThread);
      //  printf("Pushed \n");
        sortS(s, N);
        free(tempThread);
        IntrQ(*finishedQ, temp);
        takeID(vectID, temp->id);
        leftThreads++;
      //  printf("?\n");
    } else
        IntrQ(auxQ, temp);
    }
    while (!VidaQ(auxQ)) {
      ExtrQ(auxQ, temp);
      //printf("%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);

      IntrQ(*runningQ, temp);
    }


    timp -= min(timp, Q);
    printf("TIMP %d", timp);
    for (int i = 0; i < leftThreads && !VidaQ(*waitingQ); i++) {
      ExtrQ(*waitingQ, temp);
      IntrQ(*runningQ, temp);
    }
    sortQ(runningQ, maxTask);
    }


    printf("Aci\n");
    sortQ(runningQ, maxTask);
    sortQ(finishedQ, maxTask);



      execTimp = min(min(timp, Q), maxTimp);
      printf("Final\n");
      return execTimp;

  }


int main(int argc, char* argv[]) {

    FILE *input;
    input = fopen(argv[1], "r");
    FILE *output;
    output = fopen(argv[2], "w");
    u_short Q = 0;
    u_short C = 0;
    u_short N = 0;
    int execTimp = 0;
    fscanf(input, "%hu\n", &Q);
    fscanf(input, "%hu\n", &C);
    N = 2 * C;
    // setup stiva
    u_short maxTask = 1<<15;
    void *thrdS = setupStiva(N);
    void *waitingQ = InitQ(sizeof(task), maxTask);
    void *runningQ = InitQ(sizeof(task), maxTask);
    void *finishedQ = InitQ(sizeof(task), maxTask);
    u_int *vectID = (u_int*)calloc(1<<10, sizeof(int));


  //  printf( "%hu %hu\n", cuanta, cores);
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
        task *u = (task*)calloc(1, sizeof(task));
        thread *tempTrd = (thread*)calloc(1, sizeof(thread));
        for(int i = 0; i < nrTaskuri; i++) {
          if (!VidaS(thrdS))
            Pop(thrdS, tempTrd);
          else
            tempTrd->id = -1;
      //    printf( "id %d",tempTrd->id);
          u->trd.id = tempTrd->id;
          u->prio = prio;
          u->total_time = time;
          u->left_time = time;
          u->id = giveID(&vectID);
          //printf("Given id %hd\n", u->id);
          IntrQ(waitingQ, u);
          //printf( "Cv\n");
        //  printf( "id %d %d %"PRId8" time %u\n ",u->id,u->trd.id,u->prio, u->left_time);
          printf( "Task created successfully : ID %hu.\n", u->id);
        }
        free(u);
        free(tempTrd);

      //  printf( "%hu %u %"PRId8"\n", nrTaskuri, time, prio);
       sortQ(&waitingQ, maxTask);
      }

      if(strcmp(word[0], "get_task") == 0) {

            short id = atoi(word[1]);
            get_task(&waitingQ, &runningQ, &finishedQ, id, maxTask, output);

      }

      if(strcmp(word[0], "get_thread") == 0) {
        u_short id = atoi(word[1]);
        get_thread(&runningQ, id, maxTask, N, output);

      }

      if(strcmp(word[0], "print") == 0) {
        int8_t mode = -1;
        if(strcmp(word[1], "waiting") == 0) {
          mode = 0;
          print(&waitingQ, mode, maxTask, output);

        } else if(strcmp(word[1], "running") == 0) {
          mode = 1;
          print(&runningQ, mode, maxTask, output);

        } else if(strcmp(word[1], "finished") == 0) {
          mode = 2;
          print(&finishedQ, mode, maxTask, output);

        }
      }

      if(strcmp(word[0], "run") == 0) {
        int timp = atoi(word[1]);

        printf( "Running tasks for %u ms...\n", timp);
        execTimp += run(timp, Q, maxTask, N, &vectID, &waitingQ, &runningQ, &finishedQ, &thrdS, output);
        printf("Iesit\n");

      }

      if(strcmp(word[0], "finish") == 0) {
        while (!VidaQ(runningQ)) {
          execTimp += run(Q, Q, maxTask, N, &vectID, &waitingQ, &runningQ, &finishedQ, &thrdS, output);
        }
        printf( "Total time: %d\n", execTimp);
      }



    }

    fclose(input);
    fclose(output);
}
