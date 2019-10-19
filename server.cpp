#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
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
	//Used the mathematics behind walsh codes in order to implement the CDMA system
	struct block b[3];
	struct message mess[3];
	int w[3][4] = { -1, 1, -1, 1,-1, -1, 1, 1,-1, 1, 1, -1 };
	int walsh1[4] = { -1, 1, -1, 1 };
	int walsh2[4] = { -1, -1, 1, 1 };
	int walsh3[4] = { -1, 1, 1, -1 };
	mess[1].value = 66666;
	mess[2].value = 88888;
	
	//server
	int sockfd, newsockfd[3], portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0)
		cout<<"ERROR opening socket";
	int i;
	
		bzero((char *)&serv_addr, sizeof(serv_addr));
		portno = atoi(argv[1]);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			cout<<"ERROR on binding";
		}
		listen(sockfd, 5);
		clilen = sizeof(cli_addr);
		for (int j = 0; j < 3; j++) {
			//Creates a new socket that fully establishes a connection between client and server
			newsockfd[j] = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
			//the message on the socket is being read
			n = read(newsockfd[j], &messsj], sizeof(struct message));
			cout << "Here is the message from child " << j + 1 << ": Value = " << mess[j].value << ", Destination = " << mess[j].destination << endl;
			int binary[3][3];
			int x = 0;
			while (mess[j].value > 0)
			{
				int temp = mess[j].value % 2;
				if (temp == 0) {
					binary[j][2-x] = -1;

				}
				else {
					binary[j][2-x] = 1;
				}
				mess[j].value /= 2;
				x++;
			}
			int em1[12];
			int em2[12];
			int em3[12];
			for (int i = 0; i < 4; i++) {
				em1[i] = w[0][i] * binary[0][0];
				em2[i] = w[1][i] * binary[1][0];
				em3[i] = w[2][i] * binary[2][0];
			}
			for (int i = 0; i < 4; i++) {
				em1[i + 4] = w[0][i] * binary[0][1];
				em2[i + 4] = w[1][i] * binary[1][1];
				em3[i + 4] = w[2][i] * binary[2][1];
			}
			for (int i = 0; i < 4; i++) {
				em1[i + 8] = w[0][i] * binary[0][2];
				em2[i + 8] = w[1][i] * binary[1][2];
				em3[i + 8] = w[2][i] * binary[2][2];
			}
			for (int i = 0; i < 12; i++) {
				b[j].ecodedm[i] = em1[i] + em2[i] + em3[i];
			}
		}
		memcpy(b[0].ecodedm, b[2].ecodedm, sizeof(b[2].ecodedm));
		memcpy(b[1].ecodedm, b[2].ecodedm, sizeof(b[2].ecodedm));
		memcpy(b[2].ecodedm, b[2].ecodedm, sizeof(b[2].ecodedm));
		for (int c = 0; c < 3; c++) {
			if (mess[c].destination == 1) {
				for (int f = 0; f < 4; f++) {
					b[0].walshcode[f] = w[c][f];
				}
			}
			else if (mess[c].destination == 2) {
				for (int f = 0; f < 4; f++) {
					b[1].walshcode[f] = w[c][f];
				}
			}
			else if (mess[c].destination == 3) {
				for (int f = 0; f < 4; f++) {
					b[2].walshcode[f] = w[c][f];
				}
			}
		}
		
		for (int a = 0; a < 12; a++) {
			cout<<b[0].ecodedm[a];
		}
		cout << endl;
		for (int a = 0; a < 4; a++) {
			cout << b[0].walshcode[a];
		}
		cout << endl;
		if (n < 0) {
			cout<<"ERROR reading from socket";
		}
		for (int i = 0; i < 3; i++) {
			n = write(newsockfd[i], &b[i], sizeof(struct block));
			sleep(2);
		}
		for (int i = 0; i < 3; i++)
		{
			close(newsockfd[i]);
		}
		close(sockfd);
	
	return 0;
}
