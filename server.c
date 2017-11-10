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
      
      pthread_mutex_lock(&mutex);

      while(count == 0){
           //printf("Head: %d, Tail: %d\n", head, tail);
           pthread_cond_wait(&empty, &mutex); 
	printf("Count: %d, Buffer: %d\n", count, nbuffer);
           //sleep(1);
           
      //grab lock
      }
      //printf("out of while loop\n");

      //dequeue
      int connfd = queue[head];
      queue[head] = -1;
      //printf("Debug1\n");
      head++;
      count--;
      if(head >= nbuffer)
          head = 0;
      //printf("Debug2\n");
      pthread_cond_signal(&fill);
      //printf("client fd: %d\n", *client);
      
      //printf("Debug3\n");
      
      //printf("Debug4\n");
      pthread_mutex_unlock(&mutex);
      requestHandle(connfd);
      close(connfd);
    }
}

void queue_put(int value){
    pthread_mutex_lock(&mutex);
    
    while(count >= nbuffer){
      
      pthread_cond_wait(&fill, &mutex);
    }
    queue[tail] = value;
    tail++;
    count++;
    if(tail >= nbuffer)
         tail = 0;
    //printf("Updating the queue, Tail: %d\n", tail);
    //signal to wake up the thread
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
}
int main(int argc, char *argv[])
{
    int listenfd, port, clientlen, nthreads, connfd;
    struct sockaddr_in clientaddr;
    getargs(&port, &nthreads, argc, argv);
    pthread_t *tids = (pthread_t*)malloc(sizeof(pthread_t)*nthreads);
    count = 0;
    head = 0;
    tail = 0;
    queue = (int*)malloc(sizeof(int)*nbuffer);
    listenfd = Open_listenfd(port);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&fill, NULL);
    //printf("NBUFFER: %d", nbuffer);
    for(int i = 0; i < nthreads; i++){
        pthread_create(&tids[i], NULL, thread_handle, NULL);
    }
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
  printf("Client %d\n", connfd);
        queue_put(connfd);
  //Getting a queue in c:
  //Array[10] First in First out. 
  //Use circular arrray to make it efficient
  //Make front ptr and back ptr
  //Front = Where queue out (ptr that has smt)
  //Back = Where queeue is added
  //Protect this queue with lock!

	
    }

}


    


 
