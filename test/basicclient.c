//sebastian solorzano -- comp networks lab 3a -- nov 2025
// I AM A GENIUS WHO CAN WRITE NETWORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h> //needed for inet_addr()

#define INVALID_SOCKET ~0 //this is how windows does it idk man
#define SOCKET_ERROR -1

int main(void) {



	
	//socket
	int ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		printf("cout <<ERROR: Failed to create ServerSocket << std::endl;");
		return 0;
	}
	printf("succesful socket\n");
	//same as on server


	//client does not bind

	//connect
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(27000); //is this choosing its own port? or the one to connect to on the server? do they have to match?
	SvrAddr.sin_addr.s_addr =	inet_addr("127.0.0.1");
		//I know this is a computer's own IP when not connected, but does that mean this only works when going through localhost?
		//if you were trying this with 2 seperate computers (on the same network) would it still work?

	if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		//I was having some issues at this stage so I added this

		close(ClientSocket);
		printf("cout << ERROR: Connection attempted failed << std::endl;");
		return 0;
	}
	printf("found a connection!\n");
	
	//is there a way to prevent connecting automatically? for example, in games you usually get the choice
	//to browse lobbies and see what there is before choosing one to go to. 
	//the same applies to LAN multiplayer, such as on the Nintendo DS, which I assume works similarily to this?


		int ini;
		int outi=478;
		recv(ClientSocket, &ini, sizeof(ini), 0);
		send(ClientSocket,&outi,sizeof(outi),0);
		printf("%d",ini);

	printf("ending\n");

	//closesocket
	close(ClientSocket);
	//cleanup


	return 0;
}