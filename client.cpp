#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
using namespace std;

struct block {
	int ecodedm[12];
	int walshcode[4];
};

struct message{
	int destination;
	int value;
};
void error(char *msg)
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])
{
	//struct hostent *lh = gethostbyname("localhost");
	string line;
    int pid;
	struct message mess[3];
	struct block b;
	int p = 0;
	while (cin >> mess[p].destination >> mess[p].value) {
		p++;
	}
	/*
	mess[0].destination = 3;
	mess[0].value = 4;
	mess[1].destination = 1;
	mess[1].value = 5;
	mess[2].destination = 2;
	mess[2].value = 7;
	*/
	//client
	int sockfd[3], portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	//int sockets[3];
	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
    server = gethostbyname(argv[1]);
	portno = atoi(argv[2]);
	
	int i;
	for (i = 0; i < 3; i++) {
		if ((pid = fork()) == 0) { // child process
			
			break;
		}
		//sleep(1);
	}
	//for (int r = 0; r < 3; r++) {
		
	//}
	if (pid == 0) {
		//under pid == 0 else wait
		sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
		
		if (server == NULL) {
			fprintf(stderr, "ERROR, no such host\n");
			exit(0);
		}
		bzero((char *)&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
		serv_addr.sin_port = htons(portno);
		if (connect(sockfd[i], (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			//cout << "connect works" << endl;
		}
		cout << "Child " << i + 1 << ", sending value: " << mess[i].value << " to child process " << mess[i].destination << endl;
		n = write(sockfd[i], &mess[i], sizeof(struct message));
		if (n < 0) {
			cout << "ERROR writing to socket";
		}
		//bzero(&b, 255);
		n = read(sockfd[i], &b, sizeof(struct block));
		int w[12];
		for (int j = 0; j < 4; j++) {
			w[j] = b.walshcode[j];
			w[j+4] = b.walshcode[j];
			w[j+8] = b.walshcode[j];
		}
		
		int DM1[12];
		
		int d1 = 0, d2 = 0, d3 = 0;
		for (int j = 0; j < 12; j++) {
			DM1[j] = w[j] * b.ecodedm[j];
		}
		for (int j = 0; j < 4; j++) {
			d1 = DM1[j] + d1;
			d2 = DM1[j + 4] + d2;
			d3 = DM1[j + 8] + d3;
		}
		d1 = d1 / 4;
		d2 = d2 / 4;
		d3 = d3 / 4;
		if (d1 == -1) {
			d1 = 0;
		}
		if (d2 == -1) {
			d2 = 0;
		}
		if (d3 == -1) {
			d3 = 0;
		}
		int d = (d1 * 4) + (d2 * 2) + (d3 * 1);
		cout << "Child " << i+1 << endl;
		cout << "Signal:";
		for (int g = 0; g < 12; g++) {
			cout << b.ecodedm[g];
		}
		cout << endl<<"Code: ";
		for (int e = 0; e < 4; e++) {
			cout << b.walshcode[e];
		}
		cout << endl;
		cout << "Received value = " << d << endl << endl;
		
	}
	else {
		for (int k = 0; k < 3; k++) {
			wait(NULL);
		}
	}
	return 0;
}
