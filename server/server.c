//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// oh yeah baby we're making a 4chan server


#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define INVALID_SOCKET ~0 //this is how windows does it idk man
#define SOCKET_ERROR -1

//the common protocol stuff that we decided on as a group
#define PORT_NO 27000
#define PROTO IPPROTO_TCP
	//char limits
#define TOPIC_AUTH_MAX 64
#define BODY_MAX 512


//i didnt wanna make more files so all the fxns are up here


//the reqs said all posts must be loaded into memory on startup, even if 
//I think it might be better to only load them when at the moment we need them
//as such, i'll be making a linked list here to store it all
typedef struct post{
	char topic[TOPIC_AUTH_MAX];
	char author[TOPIC_AUTH_MAX];
	char body[BODY_MAX];
}POST, *PPOST;

typedef struct postlist{
	POST content;
	struct postlist* next;
}POSTNODE, *PPOSTNODE;
//man writing all the linked list functions is such a pain

//posts must be this length, so constraining it like this should be fine?
void parsePost(char* msgIn, char topicOut[TOPIC_AUTH_MAX], char authorOut[TOPIC_AUTH_MAX], char bodyOut[BODY_MAX]){
	strncpy(topicOut,strtok(msgIn,","),TOPIC_AUTH_MAX);
		//strcmp for "", " "... , "\n"
			//figure out what actually gets sent when someone just hits enter
	strncpy(authorOut,strtok(NULL,","),TOPIC_AUTH_MAX);
		//strcmp ...
	strncpy(bodyOut,strtok(NULL,","),BODY_MAX);
	
	//strtok is not a trustworthy function (i think), but I forget how to use sscanf
	return;
}
//certified works



int main(void) {

	//LOAD UP THE FILE INTO THE LL AT THE START HERE
	//----------------------

	//----------------------


	//SOCKET CONNECTION JUNK
	//----------------------

	//socket
	int ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //common protocol decided by entire group
	if (ServerSocket == INVALID_SOCKET) {
		printf("cout << ERROR: Failed to create ServerSocket << std::endl;");
		return 0;
	}
	printf("succesful socket creation\n");

	
	//bind
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000); // <-- common protocol decided by entire group
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		close(ServerSocket);
		printf("cout << ERROR: Failed to bind ServerSocket << std::endl;");
		return 0;
	}
	printf("succesfull bind\n");


	//listen
	if (listen(ServerSocket, 1) == SOCKET_ERROR) { 
		close(ServerSocket);
		printf("cout << ERROR: listen failed to configure ServerSocket << std::endl;");
		return 0;
	}
	printf("listening...\n");


	//accept
	int ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		close(ServerSocket);
		return 0;
	}
	printf("match found!\n");
	//----------------------

	//CLIENT-SERVER STUFF (THINK ABOUT THREADS LATER)
	//----------------------

	//the first thing to do is probably send a greeting to the client
	//so they know we're connected

	//then, they should choose whether to make a post or look at them
	//which needs to be confirmed between the both of us

	//if they choose to read, we should index our LL, read and send the number of posts n,
	// then loop n times to send every message in the LL.

	//if they choose to write, we wait on recieving for their post, then return to the original choice
	//(this fulfills the 'accepting multiple posts' (at least as I understood it))

	//messages
	char ini[640];
	char topic[64];
	char auth[64];
	char body[512];
	int outi = 382;
	//char* outi = "hello client";
	//char ini[30];


	printf("wating for transmision...\n");

	recv(ConnectionSocket, &ini, sizeof(ini), 0);
	send(ConnectionSocket, &outi, sizeof(outi), 0);

	parsePost(ini,topic,auth,body);
	printf("topic: %s\n author: %s\n body:\n%s\n",topic,auth,body);

	//printf("recieved: %d\n", ini);
	printf("finished\n");


	//----------------------


	//closesocket
	close(ConnectionSocket);
	close(ServerSocket);

	//save to file, destroy everything you've mallocd

	return 0;
}
