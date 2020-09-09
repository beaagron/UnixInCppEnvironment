#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <fstream>

#define MAXLINE 4096
static int timer_expired = 0;

using namespace std;

static void alarm_handler(int sig)
{
    timer_expired = 30;
}

int main(int argc, char **argv)
{
        int sockfd, bytes;
        struct sockaddr_in servaddr;
        char sendline[MAXLINE], recvline[MAXLINE];
        pid_t parentpid = getpid();
        ifstream infile;
        string server;
        string cval;

        //basic check of the arguments
        //additional checks can be inserted
        if (argc != 3) {
                cout << "error: invalid number of arugments" << endl;
                exit(1);
        }

        cout << "Step02. Client - Start with IP=" << argv[1] << " and Port=" << argv[2] << endl;
        cout << "\tThe client PID is:" << parentpid << endl;

        //Create a socket for the client
        //If sockfd<0 there was an error in the creation of the socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("Problem in creating the socket");
                exit(2);
        }

        //Creation of the socket
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(argv[1]);
        servaddr.sin_port = htons(atoi(argv[2])); //convert to big-endian order

        //Connection of the client to the socket
        if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
                perror("Problem in connecting to the server");
                exit(3);
        }

        cout << "Step03. Client - Connected to Server" << endl;

        //Open file 
        infile.open("a4ping2Data.txt", ios::in);
        cout << "Step04. Client - Ready to read the input file: a4ping2Data.txt" << endl;

        while(infile)
        {
            // reading in data and putting in send char array
            infile >> server;
            infile >> cval;
            char server1[server.length() + 1];
            strcpy(server1, server.c_str());
            char cval1[cval.length() + 1];
            strcpy(cval1, cval.c_str());
            strcat (sendline, server1);
            strcat (sendline, cval1);

            cout << "Step01. Client - Input Data is: " << server1 << " " << cval1 << endl;

            // sending info to server
            send(sockfd, server1, server.length() + 1, 0);
            send(sockfd, cval1, cval.length() + 1, 0);

            time_t raw;
            struct tm * timeinfo;
            time(&raw);
            timeinfo = localtime(&raw);

            cout << "waiting to get result..." << endl;

            // printing results taken from the Server
            while((bytes = recv(sockfd, recvline, MAXLINE, 0)) != 0)
            {
                recvline [bytes] = 0;
                printf("%s", recvline);
            }
        
        } 
        exit(0);

        return 0;
}
