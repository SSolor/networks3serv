//sebastian solorzano -- comp networks lab 3a -- nov 2025
// I AM A GENIUS WHO CAN WRITE NETWORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define INVALID_SOCKET ~0 //this is how windows does it idk man
#define SOCKET_ERROR -1

//defines for stuff that was in the code from the port and isnt' defined here
//#define AF_INET 2
//#define SOCK_STREAM 1
//#define IPPROTO_TCP 6

int main(void) {

	//startup
	/* //i believe this is unesecary in nix
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { //what does MAKEWORD(2,2) accomplish? what is its purpose?
		printf("cout << ERROR: Failed to start WSA << std::endl;");
		return 0;
	}
	printf("succesfull startup\n");

*/

	//socket
	
	int ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		printf("cout << ERROR: Failed to create ServerSocket << std::endl;");
		return 0;
	}
	printf("succseful socket creation\n");


	//bind
	struct sockaddr_in SvrAddr; 
		//I am not sure I get how this works/what it does exactly. what's its relation to SOCKET ?
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY; 
	SvrAddr.sin_port = htons(27000); //what should one do if the port is taken? are there other options besides hardcoding a port?
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

	int ConnectionSocket;

    while(1){
        printf("waiting for someone...\n");
	//accept
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) { 
			//why are the 2nd and 3rd parameters on accept NULL ?
		close(ServerSocket);
		return 0;
	}
	printf("match found!\n");


	
	//messed around a bit with send and recieve here, though they both line up
	//here I'm just preventing infinite whiles


	
		int outi = 321;
		int ini;
		send(ConnectionSocket, &outi, sizeof(outi),0);
		//send(ConnectionSocket, &opp, sizeof(opp), 0);
		recv(ConnectionSocket,&ini,sizeof(ini),0);
		printf("%d",ini);

		
		//send(ConnectionSocket, &kill, sizeof(kill), 0);

		//I made it simple here so the client doesn't send anything but it wouldn't be any differnt really

		//recv(ConnectionSocket, &msg, sizeof(msg), 0);

	printf("finished\n");

	//closesocket
    printf("didn't close the socket\n");
}
	close(ConnectionSocket);
	close(ServerSocket);

	//cleanup


	return 0;
}
