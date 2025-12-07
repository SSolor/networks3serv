//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// oh yeah baby we're making a 4chan server (now with threads)


#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "FilePost.h"

#define INVALID_SOCKET ~0 //this is how windows does it idk man
#define SOCKET_ERROR -1

//the common protocol stuff that we decided on as a group
#define PORT_NO 27000
#define PROTO IPPROTO_TCP

typedef enum inputchoices{
    start=-1,
    quit,
    wpost,
    rposts
}INPUTS;


//this linked list will be global, and as such we will make heavy use of mutexes
PPOSTNODE posts=NULL;
pthread_mutex_t posts_mutex = PTHREAD_MUTEX_INITIALIZER;


//------THREAD FUNCTION------
//here now, we create a thread, give it connection socket, 
//and run the entire rest of the code as a function
void* clientHandler(void* arg){
    int ConnectionSocket = *(int*)arg;
    free(arg); //free malloc'd socket pointer

    char buf[POST_MAX]; //should be unique to each thread

    //first thing we're doing is sending a welcome message to the client
    send(ConnectionSocket,&"Welcome to the discussion board!\n",TOPIC_AUTH_MAX,0);
    fprintf(DEBUG,"welcome sent to client\n");

    INPUTS choice =start;
    while(choice!=quit){
        fprintf(DEBUG,"client choosing...\n");
        recv(ConnectionSocket,&choice,sizeof(choice),0);

        //user wishes to write a post
        if(choice==wpost){
            fprintf(DEBUG,"client writing a post...\n");
            recv(ConnectionSocket,buf,sizeof(buf),0);//recieving into buf

            //mutex lock/unlock around global linked list
            pthread_mutex_lock(&posts_mutex);
            addPost(&posts,buf);
            pthread_mutex_unlock(&posts_mutex);

            fprintf(DEBUG,"post saved to linked list\n");
        }

        //user wishes to read all posts
        else if(choice==rposts){
            fprintf(DEBUG,"client wishes to read posts\n");

            pthread_mutex_lock(&posts_mutex);
            int toSend= countListLength(posts);
            pthread_mutex_unlock(&posts_mutex);

            send(ConnectionSocket,&toSend,sizeof(toSend),0);
            fprintf(DEBUG,"sent quantity of posts to client (%d)\n",toSend);

            for(int i =0; i<toSend;i++){
                pthread_mutex_lock(&posts_mutex);
                peekDepth(posts,i,buf); 
                pthread_mutex_unlock(&posts_mutex);

                send(ConnectionSocket,&buf,sizeof(buf),0);
                fprintf(DEBUG,"sent single post to client\n");
            }
        }
    }
    fprintf(DEBUG,"client quit\n");

    //close connection socket
    close(ConnectionSocket);
    return NULL;
}
//------THREAD FUNCTION------


int main(void) {
    //all the printfs are just status messages for debugging, basically
    //no one really sees the server, after all
    fprintf(stderr,"does this show up?\n");

    //wanted to make this a #define, but i never take chances with strings
    fprintf(DEBUG,"loading from file...\n");
    loadPostsFromFile(&posts,"posts.dat");

    //socket
    int ServerSocket;
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //common protocol decided by entire group
    if (ServerSocket == INVALID_SOCKET) {
        fprintf(stderr,"ERROR: failed to create ServerSocket\n");
        return 0;
    }
    fprintf(DEBUG,"succesful socket creation\n");

    //bind
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(PORT_NO); // <-- common protocol decided by entire group
    if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR){
        close(ServerSocket);
        fprintf(stderr,"ERROR: Failed to bind ServerSocket\n");
        return 0;
    }
    fprintf(DEBUG,"succesfull bind\n");

    //listen
    if (listen(ServerSocket, 5) == SOCKET_ERROR) { 
        close(ServerSocket);
        fprintf(stderr,"ERROR: listen failed to configure ServerSocket\n");
        return 0;
    }
    fprintf(DEBUG,"listening...\n");

    //I believe we have to begin a loop starting here
    while(1){
        //accept
        int* ConnectionSocket = malloc(sizeof(int));
        *ConnectionSocket = accept(ServerSocket, NULL, NULL);
        if (*ConnectionSocket == SOCKET_ERROR) {
            free(ConnectionSocket);
            fprintf(stderr,"ERROR: could not accept connection\n");
            continue;
        }
        fprintf(DEBUG,"client connected\n");

        //thread creation
        pthread_t tid;
        pthread_create(&tid, NULL, clientHandler, ConnectionSocket);
        pthread_detach(tid); //no need to join, thread cleans itself up
    }

    //saving (though realistically ^C will kill before here)
    fprintf(DEBUG,"saving to file...\n");
    pthread_mutex_lock(&posts_mutex);
    savePostsToFile(&posts,"posts.dat");
    pthread_mutex_unlock(&posts_mutex);

    //closesocket
    close(ServerSocket);

    return 0;
}
//am I overdoing the debug messages? hmm...
