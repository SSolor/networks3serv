//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// intf for server functions, aka all the send and recieves
//(not done in main because threads)

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