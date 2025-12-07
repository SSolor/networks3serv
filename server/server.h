
#include "FilePost.h"
#include <pthread.h>

//thread functions can only accept 1 argument so we do this
typedef struct serverdata{
    int* ConnectionSocket;
    PPOSTNODE* posts;
    pthread_mutex_t* mutlock;
}SDAT;

typedef enum inputchoices{
	start=-1,
	quit,
	wpost,
	rposts
}INPUTS;


//this would just be the main if we weren't running threads
void * RunServer (void* args);