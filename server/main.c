//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// oh yeah baby we're making a 4chan server


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "FilePost.h"
#include "server.h"

#define INVALID_SOCKET ~0 //this is how windows does it idk man
#define SOCKET_ERROR -1

//the common protocol stuff that we decided on as a group
#define PORT_NO 27000
#define PROTO IPPROTO_TCP




int main(void) {
	//all the printfs are just status messages for debugging, basically
	//no one really sees the server, after all
	fprintf(stderr,"does this show up?\n");


//	int ConnectionSocket;

	//this linked list will be global, and as such we will make heavy use of mutexes
	PPOSTNODE posts=NULL;
	//char welcome[TOPIC_AUTH_MAX]="Welcome to the discussion board!\n";
		//no point initializing this several times within the threads
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
	SvrAddr.sin_port = htons(27000); // <-- common protocol decided by entire group
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR){
		close(ServerSocket);

		fprintf(stderr,"ERROR: Failed to bind ServerSocket\n");
		return 0;
	}
	fprintf(DEBUG,"succesfull bind\n");


	//listen
	if (listen(ServerSocket, 5) == SOCKET_ERROR) { //letting this be higher because the server will wait for input to accept again
		close(ServerSocket);

		fprintf(stderr,"ERROR: listen failed to configure ServerSocket\n");
		return 0;
	}
	fprintf(DEBUG,"listening\n");


	//accept
	int ConnectionSocket;
	int open=1;

	//i believe the mutex should only exist once?
	pthread_mutex_t mutlock;
	pthread_mutex_init(&mutlock,NULL);

	while(open){
		fprintf(DEBUG,"waiting for connection...\n");

		//mallocing it because it was acting really funky
        int* ConnectionSocket = malloc(sizeof(int));
		*ConnectionSocket = SOCKET_ERROR;
		if ((*ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
			close(ServerSocket);

			fprintf(stderr,"ERROR: could not accept connection\n");
			return 0;
		}
		fprintf(DEBUG,"client connected\n");


		//create only accepts 1 arg, so we squish everything we need into a single struct
		SDAT dat;
		dat.posts=&posts;
		dat.mutlock=&mutlock; //we all must share the mutlock, otherwise it doesn't work
		dat.ConnectionSocket=ConnectionSocket;

		pthread_t thread;
		pthread_create(&thread,NULL,RunServer,&dat);
		pthread_detach(thread);


		//this is dumb, but I can't think of how else to both let
		//the server keep running, and let it end without using ^C
		//(it has to end naturally in order to save)
		printf("close server? (enter 0)\n");
		scanf("%d",&open);
	}
	//ending while a client is still running might cause problems for them, but its not like we can have a join...
	
	//it makes sense to close the mutex once everything's done, right?
	pthread_mutex_destroy(&mutlock);

	//saving
	fprintf(DEBUG,"saving to file...\n");
	savePostsToFile(&posts,"posts.dat");
		//this should have cleared the ll, but just in case
	//deleteList(&posts);
		//although its actually a problem if this does anything since it means we didn't save everyhting

	//closesocket
	close(ServerSocket);

	return 0;
}
//am I overdoing the debug messages? hmm...