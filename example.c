#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "parapat.h"

int fib(int n){
  int  i, Fnew, Fold, temp,ans;

    Fnew = 1;  Fold = 0;
    for ( i = 2;
	  i <= n;          /* apsim_loop 1 0 */
	  i++ )
    {
      temp = Fnew;
      Fnew = Fnew + Fold;
      Fold = temp;
    }
    ans = Fnew;
  return ans;
}

int payload1(int x) {
  return (fib(900090000));
}

int payload2(int x) {
  return (fib(900090000));
}

int payload3(int x) {
  return (fib(900090000));
}

int main () {
  //Used to calculate the duration of the tests
  time_t start;
  time_t end;
  //Holds the result arrays from the parallel pattern implementations
  int *piperesults;
  int *farmresults;
  int res;
  int res2;
  int res3;
  //Holds the tasks passed to the parallel library
  int pipetasks[MAX_PIPE_TASKS];
  int farmtasks[MAX_FARM_TASKS];
  //Holds the function pointers for the stages in the pipeline
  int (*stages[MAX_STAGES])(int x);

  //Test the time a pipeline takes vs a regular for-loop to complete a number of tasks
  printf("Pipeline Test:\n");
  start = time(NULL);
  for (int i = 0; i < MAX_PIPE_TASKS; i++) {
    res = payload1(i);
    res2 = payload2(res);
    res3 = payload3(res2);
  }
  end = time(NULL);
  printf("Regular time elapsed is %ld seconds\n", (end- start));
  for (int i = 0; i < MAX_PIPE_TASKS; i++) {
    pipetasks[i] = i;
  }
  //Must set the functions for each stage
  stages[0] = payload1;
  stages[1] = payload2;
  stages[2] = payload3;
  start = time(NULL);
  piperesults = createpipe(stages, MAX_STAGES, pipetasks, MAX_PIPE_TASKS);
  end = time(NULL);
  printf("Pipelined time elapsed is %ld seconds\n\n", (end- start));

  //Test the time a farm takes vs a regular for-loop to complete a number of tasks
  printf("Farm Test:\n");
  start = time(NULL);
  for (int i = 0; i < MAX_FARM_TASKS; i++) {
    res = fib(i);
  }
  end = time(NULL);
  printf("Regular time elapsed is %ld seconds\n", (end - start));
  //Add all the tasks to be passed to the farm 
  for (int i = 0; i < MAX_FARM_TASKS; i++) {
    farmtasks[i] = i;
  }
  start = time(NULL);
  farmresults = createfarm(fib, MAX_WORKERS, farmtasks, MAX_FARM_TASKS);
  end = time(NULL);
  printf("Farm time elapsed is %ld seconds\n", (end - start));
}
