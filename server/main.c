//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// oh yeah baby we're making a 4chan server



#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define INVALID_SOCKET ~0 //this is how windows does it idk man
#define SOCKET_ERROR -1



int main(void) {

	//socket

	int ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //common protocol decided by entire group
	if (ServerSocket == INVALID_SOCKET) {
		printf("cout << ERROR: Failed to create ServerSocket << std::endl;");
		return 0;
	}
	printf("succseful socket creation\n");


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
	if (listen(ServerSocket, 1) == SOCKET_ERROR) { //what does the 1 here represent?
		close(ServerSocket);
		printf("cout << ERROR: listen failed to configure ServerSocket << std::endl;");
		return 0;
	}
	printf("listening...\n");


	//accept
	int ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		//why are the 2nd and 3rd parameters on accept NULL ?
		close(ServerSocket);
		return 0;
	}
	printf("match found!\n");



	//storage for client info
//	struct sockaddr_in CliAddr;
	//socklen_t CadrLen = sizeof(CliAddr);//apparently recvfrom needs a special type or something


	//messages
	int ini;
	int outi = 382;
	//char* outi = "hello client";
	//char ini[30];


	printf("wating for transmision...\n");

	send(ConnectionSocket, &outi, sizeof(outi), 0);

	recv(ConnectionSocket, &ini, sizeof(ini), 0);

	printf("recieved: %d\n", ini);

	//casting stuff is needed and also I don't want to take chances with strings

	printf("finished\n");

	//closesocket
	close(ServerSocket);


	return 0;
}
