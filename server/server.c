//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// oh yeah baby we're making a 4chan server


#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

	PPOSTNODE posts=NULL;
	loadPostsFromFile(&posts);


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
		printf("cout << ERROR: Failed to bind ServerSocket << std::endl;\n");
		return 0;
	}
	printf("succesfull bind\n");


	//listen
	if (listen(ServerSocket, 1) == SOCKET_ERROR) { 
		close(ServerSocket);
		printf("cout << ERROR: listen failed to configure ServerSocket << std::endl;\n");
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




	//decided to go with title size, its probably fine?
	char welcome[TOPIC_AUTH_MAX]="Welcome to the discussion board!\n";
	send(ConnectionSocket,&welcome,sizeof(welcome),0);


	INPUTS choice =start;

	while(choice!=quit){
		//recieves client input on what they want to do
		printf("user choosing...\n");
		recv(ConnectionSocket,&choice,sizeof(choice),0);

		if(choice==wpost){
			printf("user wrote a post\n");
			char buf[POST_MAX];
			recv(ConnectionSocket,buf,sizeof(buf),0);
			printf("%s",buf);
			addPost(&posts,buf);
		}
		else if(choice==rposts){
			printf("user wishes to read posts\n");
				//if they choose to read, we should index our LL, read and send the number of posts n,
	// then loop n times to send every message in the LL.
			int toSend=postsLength(posts);
			send(ConnectionSocket,&toSend,sizeof(toSend),0);

			for(int i =0; i<toSend;i++){
				char buf[POST_MAX];
				pop(&posts,buf);
				send(ConnectionSocket,&buf,sizeof(buf),0);
			}
			//embarasingly, this empties the linked list, so we're going to secretly refill it here
			loadPostsFromFile(&posts);
		}

	}
	printf("user quit\n");



	//saving
	FILE* fp = fopen("posts.dat","w");
	char buf[POST_MAX];
	printf("saving...\n");
	while (posts!=NULL){
		printf("saved once\n");
		pop(&posts,buf);
		fprintf(fp,"%s",buf);
	}
	fclose(fp);

	//closesocket
	close(ConnectionSocket);
	close(ServerSocket);

	//save to file, destroy everything you've mallocd

	return 0;
}
