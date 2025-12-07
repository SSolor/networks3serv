//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// oh yeah baby we're making a 4chan server


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


int main(void) {
	//all the printfs are just status messages for debugging, basically
	//no one really sees the server, after all
	fprintf(stderr,"does this show up?\n");


//	int ConnectionSocket;

	//this linked list will be global, and as such we will make heavy use of mutexes
	PPOSTNODE posts=NULL;
	char welcome[TOPIC_AUTH_MAX]="Welcome to the discussion board!\n";
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
	if (listen(ServerSocket, 1) == SOCKET_ERROR) { 
		close(ServerSocket);

		fprintf(stderr,"ERROR: listen failed to configure ServerSocket\n");
		return 0;
	}
	fprintf(DEBUG,"listening...\n");


	//I believe we have to begin a loop starting here

	//accept
	int ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		close(ServerSocket);

		fprintf(stderr,"ERROR: could not accept connection\n");
		return 0;
	}
	fprintf(DEBUG,"client connected\n");

	//here now, I belive we create a thread, give it connection socket, and run the entire rest of the code as a function
	//run a thread detach or whatever?
	//and then add some sort of exit clause so the program can end without using ^C (letting us save)


	//------IN A THREAD -----

	char buf[POST_MAX]; //we use this a lot so it feels silly to declare it every time
	//should be unique to each thread though

	//first thing we're doing is sending a welcome message to the client
	//unsure what size they'll be accepting in, so i'm using the smallest commonly defined size we have
	send(ConnectionSocket,&welcome,sizeof(welcome),0);
	fprintf(DEBUG,"welcome sent to client\n");


	INPUTS choice =start;
	while(choice!=quit){
		fprintf(DEBUG,"client choosing...\n");
		//recieves client input on what they want to do
		recv(ConnectionSocket,&choice,sizeof(choice),0);


		//user wishes to write a post
		if(choice==wpost){
			fprintf(DEBUG,"client writing a post...\n");

			recv(ConnectionSocket,buf,sizeof(buf),0);//recieving into buf
			addPost(&posts,buf);
			fprintf(DEBUG,"post saved to linked list\n");
		}

		//user wishes to read all posts
		else if(choice==rposts){
			fprintf(DEBUG,"client wishes to read posts\n");

			//tells the client how many posts they will recieve
			//(made an assumption off the project requirements that they did not all have to be sent as a single string)
			//(as that could become extremely large, and rather inefficient)
			//(moreover, it would be difficult to communicate to the client how big a space they had to prepare)
			int toSend= countListLength(posts);

			send(ConnectionSocket,&toSend,sizeof(toSend),0);
			fprintf(DEBUG,"sent quantity of posts to client (%d)\n",toSend);

			//sending the client each post
			for(int i =0; i<toSend;i++){
				peekDepth(posts,i,buf); 
				//this gives us O(n^2) complexity but I can't think of how else to do it with a linked list
				//(without destroying it)

				send(ConnectionSocket,&buf,sizeof(buf),0);
				fprintf(DEBUG,"sent single post to client\n");
			}
		}

	}
	fprintf(DEBUG,"client quit\n");

	//i belive this can be either in or out of the loop?
	//but if we give the thread its own copy, and then detach it...
	//its probably better to do this in the loop just in case
	close(ConnectionSocket);


	//------IN A THREAD -----


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