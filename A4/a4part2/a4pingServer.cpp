#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define MAXLINE 4096
#define SERV_PORT 3000
#define LISTENQ 8
static int timer_expired = 0;

using namespace std;
 
static void alarm_handler(int sig)
{
    timer_expired = 30;
}

int main (int argc, char **argv)
{
        int listenfd, connfd, n, n2, m;
        pid_t parentpid = getpid();
        pid_t childpid;
        socklen_t clilen;
        char buf[MAXLINE];
        char buf1[MAXLINE]; 
        char line[MAXLINE];
        int i;
        char *args[5];
        char program[80];
        struct sockaddr_in cliaddr, servaddr;

        //Make sure the arguments are correct
        if (argc != 2) {
                cout << "error: argument issues" << endl;
                exit(1);
        }

        //Create a socket for the socket
        //If sockfd<0 there was an erro in the creation of the socket
        if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
                std::cout << "error: issue in the creation of the socket" << std::endl;
                exit(2);
        }

        cout << "Step01. Server - Start with Port=" << SERV_PORT << endl;
        cout << "\tThe server PID is :" << parentpid << endl << endl;

        //preparation of the socket address
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(atoi(argv[1]));

        //bind the socket
        bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        //listen to the socket by creating a connection queue, then wait for clients
        listen (listenfd, LISTENQ);

        cout << "Server running...waiting for connections." << endl;

        for ( ; ; ) {
                clilen = sizeof(cliaddr);
                //accept a connection

                connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);

                cout << "Step07. Server - Input Data received: " << endl;

                if ( (childpid = fork()) == 0) { //if it's 0, it's a child process
                        cout << "Step08. Server - Fork child process" << endl;
                        //close listening socket
                        close(listenfd);

                        FILE *fd;

                        n = recv(connfd, buf, MAXLINE, 0);
                        //n2= recv(connfd, buf1, MAXLINE, 0);

                        cout << n << ' ' << n2 << endl;

                        while ((n > 0)) {
                                if (strncmp(buf, "end", 3) == 0) {
                                        close(connfd);
                                        kill(getpid(), 9);
                                }

                                cout << "\tChild Process (pid=" << childpid << ") to run: ping -c " << buf1 << " " << buf << endl;

                                // running the command using the arguments reccieved
                                strcpy(program, "ping"); 
                                args[0] = "ping";
                                args[1] = "-c"; 
                                args[2] = buf1; 
                                args[3] = buf; 
                                args[4] = NULL;

                                int pid = fork();
                                if(pid < 0)
                                {
                                    cout << "error with fork" << endl;
                                }
                                else if (pid == 0)
                                {
                                    dup2(connfd, STDOUT_FILENO);
                                    close(connfd);
                                    i = execvp(program, argv);
                                    printf("i=%d ... did it work?\n" ,i);
                                }
                                else
                                {
                                    close(connfd);
                                }

                                time_t raw;
                                struct tm * timeinfo;
                                time(&raw);
                                timeinfo = localtime(&raw);
                                printf("Server %s%s%s\n", asctime(timeinfo), buf, line);

                                // sending results back to client
                                cout << "Step09. Server - Sent the result of the ping command to the client via socket" << endl;
                                if (send(connfd, line, strlen(line), 0) < 0) {
                                        perror("Could not send execution");
                                        exit(1);
                                }
                                memset(buf, 0, MAXLINE);
                                memset(line, 0, MAXLINE);
                        }
                        if (n < 0)
                                cout << "error: issue while printing" << endl;
                        exit(0);
                }
                //close socket of the server
                close(connfd);
        }
}
