
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include "parapat.h"
#include "queue.h"

//The input and output queues for the farm
tq tqinput;
tq tqoutput;
//Mutexes for locking the queues
pthread_mutex_t inputmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outputmutex = PTHREAD_MUTEX_INITIALIZER;
//Integer arrays to hold the results from the output queues
int *piperesults;
int *farmresults;

//Create a new node to be added to the queue
Node createNode(int v) {
  Node temp = (Node)malloc(sizeof(struct Node));
  temp->value = v;
  temp->next = NULL;
  return temp;
}

//Return the node at the front of the queue
Node gettask(tq tq) {
  if (tq->first == NULL) {
    return NULL;
  }
  Node temp = tq->first;
  tq->first = tq->first->next;
  return temp;
}

//Put a node into the front of the queue
void puttask(tq tq,int v) {
  Node temp = createNode(v);
  if (tq->first == NULL) {
    tq->first = temp;
  } else {
    temp->next = tq->first;
    tq->first = temp;
  }
}

//A worker to be threaded and carry out the user specified function
void *worker(int (*func)(int x)) {
  Node task;
  int result;
  while (1) {
    pthread_mutex_lock(&inputmutex);
    task = gettask(tqinput);
    pthread_mutex_unlock(&inputmutex);
    //If there are no more tasks break out of the execution loop
    if (task == NULL) {
      break;
    }
    result = func(task->value);
    pthread_mutex_lock(&outputmutex);
    puttask(tqoutput, result);
    pthread_mutex_unlock(&outputmutex);
  }
}

//Called by user to create and execute the farm on the given tasks
int *createfarm(int (*func)(int x),int n,int *input, int size) {
  farmresults = malloc(size * sizeof(int));
  pthread_t workers[n];
  tqinput = newtq();
  tqoutput = newtq();

  //Populate the input queue with the user defined tasks
  for (int i = 0; i < size; i++) {
    puttask(tqinput, input[i]);
  }

  //Create all the worker threads
  for (int i = 0; i < n; i++) {
    pthread_create(&workers[i],NULL,worker,func);
  }

  //Wait till the threads have all completed
  for (int i = 0; i < n; i++) {
    pthread_join(workers[i], NULL);
  }

  //Record the results to be returned to the user
  for (int i = 0; i < size; i++) {
    farmresults[i] = gettask(tqoutput)->value;
  }

  return farmresults;
}

//A threaded stage in the pipeline
void *stage(Parameters params) {
  Node task;
  int result;
  int count = 0;
  while (1) {
    pthread_mutex_lock(&inputmutex);
    task = gettask(params->input);
    pthread_mutex_unlock(&inputmutex);
    //If there are no task may need to wait for the previous stage to complete their computation
    if (task == NULL) {
      continue;
    } else {
      result = params->f(task->value);
      pthread_mutex_lock(&outputmutex);
      puttask(params->output, result);
      pthread_mutex_unlock(&outputmutex);
      count++;
    }
    //If all the tasks have been completed break out of the execution
    if (count == params->tasknum) {
      break;
    }
  }
}

//Creates and executes a pipeline with the specified stages and tasks
int *createpipe(int (**stages)(int x),int stagenum,int *input,int size) {
  piperesults = malloc(size * sizeof(int));
  pthread_t threads[stagenum];
  //Need one more queue because there is an output and input queue
  tq queues[stagenum + 1];
  Parameters parameters[stagenum];

  //Malloc the required memory for the arrays
  for (int i = 0; i < stagenum + 1; i++) {
    if (i != stagenum) {
      parameters[i] = malloc(sizeof(struct Parameters));
    }
    queues[i] = newtq();
  }

  //Populate the input queue with the user specified tasks
  for (int i = 0; i < size; i++) {
    puttask(queues[0], input[i]);
  }

  //Set the parameters for each stage
  for (int i = 0; i < stagenum; i++) {
    parameters[i]->tasknum = size;
    parameters[i]->input = queues[i];
    parameters[i]->output = queues[i + 1];
    parameters[i]->f = stages[i];
    pthread_create(&threads[i],NULL,stage,parameters[i]);
  }

  //Wait till the stages have completed
  for (int i = 0; i < stagenum; i++) {
    pthread_join(threads[i], NULL);
  }

  //Record the results
  for (int i = 0; i < size; i++) {
    piperesults[i] = gettask(queues[stagenum])->value;
  }
  return piperesults;
}

Queue newQueue () {
  Queue q = (Queue)malloc(sizeof(struct Queue));
  q->first = NULL;
  return q;
}

tq newtq() {
  tq tq = newQueue();
  return(tq);
}
