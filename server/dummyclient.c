//sebastian solorzano, Ryan Hackbart -- compnet assgmt3 group 13 -- CSCN71020 25F 
// I just copied the lab3 code for this so we can make sure our stuff works


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
	SvrAddr.sin_port = htons(27000); //we agreed on this port
	SvrAddr.sin_addr.s_addr =	inet_addr("127.0.0.1"); //assignment said so

	if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		close(ClientSocket);
		printf("cout << ERROR: Connection attempted failed << std::endl;");
		return 0;
	}
	printf("found a connection!\n");
	

    char h[640] = "testings tims and seasons,deeplightning,did you know that in terms"
    "of all the lightouses on earth\7 the brightest one exists in the circumpolar night"
    "of the antarctic wastes? the reson for htis is quite curious\7 and in fact\7 no one seems to recall when it was built.\n";
    
    //this is where the magic happens
    char welcome[64];
    int decision =1;

    //recieves welcom
    
//RECV A: SMALL STRING
    recv(ClientSocket,&welcome,sizeof(welcome),0);
    printf("recieved: %s",welcome);

    //sends decision (write)

//SEND B: INT
    send(ClientSocket,&decision,sizeof(decision),0);

    //sends message
    
//SEND C: STRING
    send(ClientSocket,&h,sizeof(h),0);


    
    decision=2;
    //sends decision(read)

//SEND B: INT
    send(ClientSocket,&decision,sizeof(decision),0);

    int loops=0;
    char buf[640];

//RECIEVE D:
    recv(ClientSocket,&loops,sizeof(loops),0);
    printf("%d\n",loops);

    for(int i=0;i<loops;i++){
        printf("looped once\n");
        recv(ClientSocket,&buf,sizeof(buf),0);
        printf("%s",buf);
    }
    


    decision=0;
    //sends decision (quit)

//SEND B: INT
    send(ClientSocket,&decision,sizeof(decision),0);


	printf("ending\n");

	//closesocket
	close(ClientSocket);


	return 0;
}