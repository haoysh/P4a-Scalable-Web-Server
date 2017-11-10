#include "cs537.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// CS537: Parse the new arguments too

pthread_cond_t empty, fill;
pthread_mutex_t mutex;
int head, tail;
int *queue;
int nbuffer;
int connfd;
int count;

void getargs(int *port, int *nthreads, int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *nthreads = atoi(argv[2]);
    nbuffer = atoi(argv[3]);

}

void* thread_handle(void* port){
    //while loop
    //int *client = (int*) connfd;
    //printf("client fd: %d\n", client);
    while(1){
      while(head == tail){
           //printf("Head: %d, Tail: %d\n", head, tail);
           pthread_cond_wait(&empty, &mutex); 
           //sleep(1);
           
      //grab lock
      }
      //printf("out of while loop\n");
 	    //pthread_mutex_lock(&mutex);
      //dequeue
      queue[head] = -1;
      //printf("Debug1\n");
      head++;
      count--;
      if(head > nbuffer)
          head = 0;
      //printf("Debug2\n");
      pthread_cond_signal(&fill);
      //printf("client fd: %d\n", *client);
      requestHandle(connfd);
      //printf("Debug3\n");
      close(connfd);
      //printf("Debug4\n");
      //pthread_mutex_unlock(&mutex);
    }
}

void queue_put(int ptr, int value){
    
    pthread_mutex_lock(&mutex);
    count++;
    
    while(count > nbuffer){
      pthread_cond_wait(&fill, &mutex);
    }
    queue[ptr] = value;
    tail++;
    if(tail > nbuffer)
         tail = 0;
    //printf("Updating the queue, Tail: %d\n", tail);
    //signal to wake up the thread
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
}
int main(int argc, char *argv[])
{
    int listenfd, port, clientlen, nthreads;
    struct sockaddr_in clientaddr;
    getargs(&port, &nthreads, argc, argv);
    pthread_t *tids = (pthread_t*)malloc(sizeof(pthread_t)*nthreads);
    for(int i = 0; i < nthreads; i++){
        pthread_create(&tids[i], NULL, thread_handle, (void*) port);
    }
    pthread_mutex_init(&mutex, NULL);
    queue = (int*)malloc(sizeof(int)*nbuffer);
    listenfd = Open_listenfd(port);
    head = 0;
    tail = 0;
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
  printf("Client %d\n", connfd);
        queue_put(tail, connfd);
  //Getting a queue in c:
  //Array[10] First in First out. 
  //Use circular arrray to make it efficient
  //Make front ptr and back ptr
  //Front = Where queue out (ptr that has smt)
  //Back = Where queeue is added
  //Protect this queue with lock!

	
    }

}


    


 
