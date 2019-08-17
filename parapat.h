#ifndef PARAFILE_H
#define PARAFILE_H
#define MAX_PIPE_TASKS 5        //Controls the number of tasks passed to the pipeline
#define MAX_FARM_TASKS 100000   //Controls the number of tasks passed to the farm
#define MAX_WORKERS 16          //Controls the number of worker threads deployed by the farm
#define MAX_STAGES 3            //Controls the number of stages in the pipeline
int *createfarm(int(*func)(int x),int n,int *input, int size);
int *createpipe(int (**stages)(int x),int stagenum,int *input,int size);
#endif
