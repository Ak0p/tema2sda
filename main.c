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

  short id;
  thread trd;
  u_int8_t prio;
  u_int total_time;
  u_int left_time;

} task;


u_int giveID(u_int **v) {
  int k = 0;
  int l = 31;
  for(; k < (1 << 10); k++) {
    for(l = 31; l >= 0; l--) {
      int temp = ((*v)[k] & (1 << l)) >> l;
      if(temp == 0) {
        (*v)[k] |= (1 << l);
        //printf("k=%d l=%d\n", k, l);
        return k*32 + (31-l+1);
      }
    }
  }
  return -1;
}

void takeID(u_int *v, u_short id) {
      u_short k = id / 32;
      u_short l = 31 - (id % 32) - 1;
      v[k] -= 1 << l;
}


// void *init(u_short cuanta, u_short cores) {
//   u_short N = 2 * cores;
//   // setup stiva
//   void *thStack =  InitS(sizeof(thread), N);
//   thread temp;
//   temp.id = N - 1;
//   while(temp.id >= 0) {
//     Push(thStack, &temp);
//     temp.id--;
//   }
//   void *waitingQ = InitQ(sizeof(task), N);
//   void *runningQ = InitQ(sizeof(task), N);
//   void *finishedQ = InitQ(sizeof(task), N);
//
// }

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

int cmpTaskeg(task *task1, task* task2) {
  if( task1->prio == task2->prio) {
    if (task1->left_time > task2->left_time) {
      if (task1->id > task2->id)
        return 1;
    }

  }
  return 0;
}

void sortQ(void **q, int maxTask) {

    void *aux = InitQ(sizeof(task), maxTask);
    task *min = (task*)calloc(1, sizeof(task));
    task *temp = (task*)calloc(1, sizeof(task));

    for(int i = 0; i < maxTask; i++) {

      ExtrQ(*q, min);
      if(VIDA(*q)) {
        IntrQ(*q, min);
        return;
      }

      for (int i = 0; i < maxTask && !VidaQ(*q); i++) {

        ExtrQ(*q, temp);
        if (cmpTask(min, temp)) {
          IntrQ(*q, temp);
        } else {
          task *schimb = (task*)calloc(1, sizeof(task));
          memcpy(schimb, min, sizeof(task));
          memcpy(min, temp, sizeof(task));
          memcpy(temp, schimb, sizeof(task));
          free(schimb);
          IntrQ(*q, temp);
        }
      }
      IntrQ(aux, min);
    }
    while(!VidaQ(aux)) {
      ExtrQ(aux, temp);
      IntrQ(*q, aux);
    }

}

void sortQu(void **q, int maxTask) {

    void *aux = InitQ(sizeof(task), maxTask);
    task *min = (task*)calloc(1, sizeof(task));
    task *temp = (task*)calloc(1, sizeof(task));
    void *final= InitQ(sizeof(task), maxTask);
    while (!VidaQ(*q)) {
    //  printf("Aci mare\n");
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
      //  printf("%d %d %"PRId8" %u %u\n", min->id, min->trd.id, min->prio, min->total_time, min->left_time);
        while (!VidaQ(aux)) {
      //    printf("Aci mic\n");
          ExtrQ(aux, temp);
        //  printf("%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);
          IntrQ(*q, temp);
      }
    }
    while (!VidaQ(final)) {
    //  printf("Aci final\n");
      ExtrQ(final, temp);
    //  printf("%d %d %"PRId8" %u %u\n", temp->id, temp->trd.id, temp->prio, temp->total_time, temp->left_time);
      IntrQ(*q, temp);
    }
  //  printf("E\n");
    free(temp);
    free(min);
  }


void *setupStiva(int a) {
  void *thStack =  InitS(sizeof(thread), a);
  int trdId = a - 1;
  while(trdId >= 0) {
    thread *temp = (thread*)calloc(1, sizeof(thread));
    temp->id = trdId;
    Push(thStack, temp);
    trdId--;
    // printf("Bfr %d id \n", temp->id);
    // Top(thStack, temp);
    // printf("After %d id \n", temp->id);
  }
    return thStack;
}


void get_task(void **waitingQ, short id, int8_t status, int maxTask) {
  void *aux = InitQ(sizeof(task), maxTask);
  task *temp = (task*)calloc(1, sizeof(task));
  int8_t cond = 0;
    while (!VidaQ(*waitingQ)) {
      ExtrQ(*waitingQ, temp);
    //  printf("Func id %d %d %"PRId8" time %u \n ",temp->id,temp->trd.id,temp->prio,temp->left_time);
      if (temp->id == id) {
        switch (status) {
          case 0:
            printf("Task %d is waiting (remaining_time = %u).\n"
            ,temp->id, temp->left_time);
            break;
          case 1:
            printf("Task %d is running (remaining_time = %u).\n"
            ,temp->id, temp->left_time);
            break;
          case 2:
            printf("Task %d is finished (executed_time = %u).\n"
            ,temp->id, temp->total_time);
            break;
        }
        cond = 1;
      }
      IntrQ(aux, temp);
    }
    if (!cond) {
      printf("Task %d not found.\n", id);
    }
    while (!VidaQ(aux)) {
      ExtrQ(aux, temp);
       //printf("Func jos %d %d %"PRId8" %u\n ",temp->id,temp->trd.id,temp->prio,temp->left_time);
      IntrQ(*waitingQ, temp);
    }
    DistrQ(&aux);
    free(temp);
  //  printf("Cv\n");

}

void get_thread(void **s, void **waitingQ, void **runningQ, u_short id, int N, int maxTask) {
  int8_t idle = 0;
  void *auxS = InitS(sizeof(thread), N);
  thread *tempTrd = (thread*)calloc(1, sizeof(thread));
  while(!VidaS(*s)) {
  //  printf("Da\n");
    Pop(*s, tempTrd);
//    printf("%d\n", tempTrd->id);
    if (tempTrd->id == id) {
      idle = 1;
      printf("Thread %hu is idle.\n", tempTrd->id);
    }
      Push(auxS, tempTrd);
  //    printf("%d\n", tempTrd->id);
  }

    while(!VidaS(auxS)) {
      Pop(auxS, tempTrd);
      Push(*s, tempTrd);
    }
    DistrS(&auxS);
    free(tempTrd);

  void *auxQ = InitQ(sizeof(task), maxTask);
  task *tempTask = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*waitingQ)) {
        ExtrQ(*waitingQ, tempTask);
        if (tempTask->trd.id == id) {
         printf("Thread %hu is idle.\n",tempTask->trd.id);
         idle = 1;
        }
        IntrQ(auxQ, tempTask);
      }
      while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, tempTask);
        IntrQ(*waitingQ, tempTask);
      }
      DistrQ(&auxQ);
      free(tempTask);
  if(idle)
    return;

      // while (!VidaQ(*runningQ)) {
      //     ExtrQ(*runningQ, tempTask);
      //     if (tempTask->trd.id == id) {
      //        printf("Thread %d is running task %d (remaining_time = %d).\n"
      //        ,tempTask->trd.id ,tempTask->id, tempTask->left_time);
      //     }
      //     IntrQ(auxQ, tempTask);
      //   }
      //   while (!VidaQ(auxQ)) {
      //     ExtrQ(auxQ, tempTask);
      //     IntrQ(*runningQ, tempTask);
      //   }
      //   DistrQ(&auxQ);
      //   free(tempTask);
}

void print(void **q, int8_t mode, int maxTask) {

if (mode == 0) {
  printf("====== Waiting queue ======\n");
  printf("[");
  void *auxQ = InitQ(sizeof(task*), maxTask);
  task *tempTask = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*q)) {
        ExtrQ(*q, tempTask);
        if (VidaQ(*q)) {
          printf("(%d: priority = %d, remaining_time = %d)"
          , tempTask->id, tempTask->prio, tempTask->left_time);
        } else {
        printf("(%d: priority = %d, remaining_time = %d),\n"
        , tempTask->id, tempTask->prio, tempTask->left_time);
        }
        IntrQ(auxQ, tempTask);
      }
  printf("]\n");
  while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, tempTask);
        IntrQ(*q, tempTask);
      }
  DistrQ(&auxQ);
  free(tempTask);


} else if (mode == 1) {

  printf("====== Running in parallel ======\n");
  printf("[");
  void *auxQ = InitQ(sizeof(task*), maxTask);
  task *tempTask = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*q)) {
        ExtrQ(*q, tempTask);
      if (VidaQ(*q)) {
        printf("(%d: priority = %d, remaining_time = %d, running_thread = %d)"
          ,tempTask->id, tempTask->prio, tempTask->left_time, tempTask->trd.id);
      } else {
      printf("(%d: priority = %d, remaining_time = %d, running_thread = %d),\n"
        ,tempTask->id, tempTask->prio, tempTask->left_time, tempTask->trd.id);
      }
        IntrQ(auxQ, tempTask);
      }

  printf("]\n");
  while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, tempTask);
        IntrQ(*q, tempTask);
      }
  DistrQ(&auxQ);
  free(tempTask);

} else if (mode == 2) {

  printf("====== Finished queue ======\n");
  printf("[");
  void *auxQ = InitQ(sizeof(task*), maxTask);
  task *tempTask = (task*)calloc(1, sizeof(task));
      while (!VidaQ(*q)) {
        ExtrQ(*q, tempTask);
        if (VidaQ(*q)) {
          printf("(%d: priority = %d, executed_time = %d)"
          , tempTask->id, tempTask->prio, tempTask->total_time);
        } else {
        printf("(%d: priority = %d, executed_time = %d),\n"
        , tempTask->id, tempTask->prio, tempTask->total_time);
        }
        IntrQ(auxQ, tempTask);
      }
  printf("]\n");
  while (!VidaQ(auxQ)) {
        ExtrQ(auxQ, tempTask);
        IntrQ(*q, tempTask);
      }
  DistrQ(&auxQ);
  free(tempTask);

}
}


int main(int argc, char* argv[]) {

    FILE *input;
    input = fopen(argv[1], "r");
    u_short Q = 0;
    u_short C = 0;
    u_short N = 0;
    fscanf(input, "%hu\n", &Q);
    fscanf(input, "%hu\n", &C);
    N = 2 * C;
    // setup stiva
    int8_t status = 0;
    u_short maxTask = 1<<15;
    void *thrdS = setupStiva(N);
    void *waitingQ = InitQ(sizeof(task), maxTask);
    void *runningQ = InitQ(sizeof(task), maxTask);
    void *finishedQ = InitQ(sizeof(task), maxTask);
    u_int *vectID = (u_int*)calloc(1<<10, sizeof(int));


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
        task *u = (task*)calloc(1, sizeof(task));
        thread *tempTrd = (thread*)calloc(1, sizeof(thread));
        for(int i = 0; i < nrTaskuri; i++) {
          Pop(thrdS, tempTrd);
      //    printf("id %d",tempTrd->id);
          u->trd.id = tempTrd->id;
          u->prio = prio;
          u->total_time = time;
          u->left_time = time;
          u->id = giveID(&vectID);
          IntrQ(waitingQ, u);
        //  printf("id %d %d %"PRId8" time %u\n ",u->id,u->trd.id,u->prio, u->left_time);
          printf("Task created successfully : ID %hu.\n", u->id);
        }
        free(u);
        free(tempTrd);

      //  printf("%hu %u %"PRId8"\n", nrTaskuri, time, prio);
       sortQu(&waitingQ, maxTask);
      }

      if(strcmp(word[0], "get_task") == 0) {

            short id = atoi(word[1]);
            get_task(&waitingQ, id, status, maxTask);

      }

      if(strcmp(word[0], "get_thread") == 0) {
        u_short id = atoi(word[1]);
        get_thread(&thrdS, &waitingQ, &runningQ, id, N, maxTask);

      }

      if(strcmp(word[0], "print") == 0) {
        int8_t mode = -1;
        if(strcmp(word[1], "waiting") == 0) {
          mode = 0;
          print(&waitingQ, mode, N);

        } else if(strcmp(word[1], "running") == 0) {
          mode = 1;
          print(&runningQ, mode, N);

        } else if(strcmp(word[1], "finished") == 0) {
          mode = 2;
          print(&finishedQ, mode, N);

        }
      }

      if(strcmp(word[0], "run") == 0) {
        u_int time = atoi(word[1]);

        printf("Running tasks for %u ms...\n", time);
        int  i = time;
        while(!PLINA(runningQ) && !VIDA(thrdS)) {


          for(int j = 0; j < N; j++) {
            task *tempTsk = (task*)calloc(1, sizeof(task));
            thread *tempTrd = (thread*)calloc(1, sizeof(thread));
            ExtrQ(waitingQ, tempTsk);
            Pop(thrdS, tempTrd);
            tempTsk->trd.id = tempTrd->id;
            tempTsk->left_time -= min(Q, i);
            // if()
        //    Push(runningS, tempTrd);
            IntrQ(runningQ, tempTsk);
          }
          i -= min(time, Q);
        }
        // for(int i = time; i = 0 || i - Q < 0 ; i - min(i,Q) ) {
        //     for(int j = 0; j < N; j++) {
        //       task *tempTsk = (task*)calloc(1, sizeof(task));
        //       thread *tempTrd = (thread*)calloc(1, sizeof(thread));
        //       ExtrQ(waitingQ, tempTsk);
        //       Pop(thrdS, tempTrd);
        //       tempTsk->trd.id = tempTrd->id;
        //
        //       Push(runningS, tempTrd);
        //       IntrQ(runningQ, tempTsk);
        //     }
        // }

      }

      if(strcmp(word[0], "finish") == 0) {

      }



    }





}
