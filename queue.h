#ifndef QUEUEFILE_H
#define QUEUEFILE_H
typedef int Value;

typedef struct Node *Node;

struct Node {
  Value value;
  Node next;
};

typedef struct Queue *Queue;

struct Queue {
  Node first;
};

typedef Queue tq;

typedef struct Parameters *Parameters;

//Used to hold the parameters passed to the pipeline stages
struct Parameters {
  int tasknum;      //The total number of tasks
  tq input;         //The input queue
  tq output;        //The output queue
  int (*f)(int x);  //The user specified function for the stage
};

tq newtq();
Node createNode(int v);
Node gettask(tq tq);
void puttask(tq tq,int v);
#endif
