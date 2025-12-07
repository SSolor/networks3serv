//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// impl for server functions, aka all the send and recieves
//(not done in main because threads)

#include "server.h"
#include "FilePost.h"
#include <stdio.h>
#include <stdlib.h>

//idk if I need all these tbh but things werent working, so
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void * RunServer (void* args){

    //thread functions must accept void * so we have to do this
    SDAT* data= (SDAT*) args;

    //the connectionsocket was acting really funky, so we're copying it over and deleting the original
    int ConnectionSocket = *data->ConnectionSocket;
    free(data->ConnectionSocket);
    printf("our connection sock number is %d\n",ConnectionSocket);

    char buf[POST_MAX]; //we use this a lot so it feels silly to declare it every time

    //i'd like to make this global but it just made things weird
    char welcome[TOPIC_AUTH_MAX]="Welcome to the discussion board!\n";

	//first thing we're doing is sending a welcome message to the client
	//unsure what size they'll be accepting in, so i'm using the smallest commonly defined size we have
	send(ConnectionSocket,&welcome,sizeof(welcome),0);
	//ok so i figured this out through dealing with a really annoying bug, but buffer sizes between the client server
	//have to be the same (recieving can hopefully be bigger?). therefore an assumption on size is made with nearly every
	//send/recv
	fprintf(DEBUG,"welcome sent to client %d\n",ConnectionSocket);


	INPUTS choice =start;
	while(choice!=quit){
		fprintf(DEBUG,"client %d choosing...\n",ConnectionSocket);
		//recieves client input on what they want to do
		recv(ConnectionSocket,&choice,sizeof(choice),0);


		//user wishes to write a post
		if(choice==wpost){
			fprintf(DEBUG,"client %d wishes to write posts...\n",ConnectionSocket);

			//I figured that to satisfy the 'collection of posts' req,
			//we'd be doing the same thing i'm doing for reading
			int torecieve;

			recv(ConnectionSocket,&torecieve,sizeof(torecieve),0);
			fprintf(DEBUG,"recieved quantity client %d is sending (%d)\n",ConnectionSocket,torecieve);

			for(int i=0;i<torecieve;i++){
				recv(ConnectionSocket,buf,sizeof(buf),0);//recieving into buf

                //mutexes on everything, i'm taking no risks
                pthread_mutex_lock(data->mutlock);
				addPost(data->posts,buf);
                pthread_mutex_unlock(data->mutlock);

				//fprintf(DEBUG,"post saved to linked list\n");
			}
			//if we have to recieve everything as a single mega string (which is really dumb and a terrible idea)
			//then instead we can do:
				//char recieving[post_max];//unsure if they'd be using a full post or just a 'topic' portion
					//since we have to do *everything* as strings (also dumb)
				//recv(ConnectionSocket,&recieving,sizeof(recieving),0);
				//int torecieve = atoi(recieving);

				//char recvbuf[POST_MAX * torecieve];//if this doesn't work just use malloc
				//recv(ConnectionSocket,recvbuf,sizeof(recvbuf),0);
				//then write some function to split those up
				//we'd have to agree on some delimiter between seperate posts (different from the category delimiter)
					//we didn't think we'd be doing this, so we didn't decide on anything
		}

		//user wishes to read all posts
		else if(choice==rposts){
			fprintf(DEBUG,"client %d wishes to read posts\n",ConnectionSocket);

			//tells the client how many posts they will recieve
			//(made an assumption off the project requirements that they did not all have to be sent as a single string)
			//(as that could become extremely large, and rather inefficient)

            //its only a read, so maybe it doesn't need it, but it would get messed up if anyone was editing
            pthread_mutex_lock(data->mutlock);
			int toSend= countListLength(*data->posts);
            pthread_mutex_unlock(data->mutlock);

			send(ConnectionSocket,&toSend,sizeof(toSend),0);
			fprintf(DEBUG,"sent quantity of posts to client %d (%d)\n",ConnectionSocket,toSend);

			//sending the client each post
			for(int i =0; i<toSend;i++){

                //its only a read, so maybe it doesn't need it, but it would get messed up if anyone was editing
                pthread_mutex_lock(data->mutlock);
				peekDepth(*data->posts,i,buf); 
                pthread_mutex_unlock(data->mutlock);
				//this gives us O(n^2) complexity but I can't think of how else to do it with a linked list
				//(without destroying it)

				send(ConnectionSocket,&buf,sizeof(buf),0);
				//fprintf(DEBUG,"sent single post to client %d\n",ConnectionSocket);
			}
			//if we have to recieve everything as a single mega string (which is really dumb and a terrible idea)
			//then instead we can do:
				//int toSend=countListLength(posts);
				//char sending[TOPIC_AUTH_MAX];//using the smallest agreed-upon size
				//itoa? sprintf?

				//send(ConnectionSocket,&sending,sizeof(sending),0);

				//char sendbuf[POST_MAX * sending];//if this doesn't work just use malloc
					//make some loop of strcat (sendbuf,peekdepth) //does that work properly?

				//send(ConnectionSocket,sendbuf,sizeof(sendbuf),0);
				//again, we'd have to agree on some delimiter between seperate posts (different from the category delimiter)
					//we didn't think we'd be doing this, so we didn't choose anything
		}

	}
	fprintf(DEBUG,"client %d quit\n",ConnectionSocket);

	close(ConnectionSocket);
    //do I need to end the thread or something? not sure
    //dont clear posts since its used by everyone
}