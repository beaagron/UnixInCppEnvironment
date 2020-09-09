#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

using namespace std;

int main() 
{
    // reading files in from a3ping2Data.txt
    cout << "Step1. a3ping2 process starts. pid=" << getpid() << endl;
    ifstream infile;
    infile.open("a3ping2Data.txt");
    cout << "Step2. Ready to read the input file: a3ping2Data.txt" << endl;

    // looping through until there is no more data to be read
    string command;
    string server;
    string cval;
    string childnum;
    int childnum2;
    
    while(infile)
    {
        infile >> server;
        infile >> cval;
        infile >> childnum;
        cout << "Step3. Input Data is: " << server << ' ' << cval << ' ' << childnum << endl << endl;
        command = "ping -c " + cval + " " + server;
        char temp[command.length() + 1];
        strcpy(temp, command.c_str());
        char server1[server.length() + 1];
        strcpy(server1, server.c_str());
        char cval1[cval.length() + 1];
        strcpy(cval1, cval.c_str());
        stringstream childnum1(childnum);
        childnum1 >> childnum2;

        // forking the child process
        cout << "Step4. Fork child process" << endl;
        pid_t parentpid = getpid();
        pid_t pid1;
        pid_t pid2;
        int fd1[2];
        int fd2[2];
        char *argv[5];
        char program[80];
        int i;
        int status;
        pipe(fd1);
        pid1 = fork();

        for(int i = 0; i < childnum2; i++)
        {
            if(pid1 == 0)
            {
                cout << "\tChild Process " << (i + 1) << " (pid=" << pid1 << ") to run: ping -c " << cval << server << endl << endl;
                close(fd1[1]);
                close(STDIN_FILENO);
                dup(fd1[0]);

            int c;
        
            while ((c = getc(stdin)) != EOF)
                    if (putc(c, stdout) == EOF) 
                    {
                        fprintf(stderr, "write error\n");
                        exit(1);
                    }

                if (ferror(stdin)) {
                    fprintf(stderr, "read error\n");
                    exit(1);
                }

                exit(0);
            }

            pid2 = fork();

            if (pid2 == 0) 
            {
                close(fd1[0]);
                close(STDOUT_FILENO); 
                dup(fd1[1]);  
                strcpy(program, "ping");
                argv[0] = "ping";
                argv[1] = "-c"; 
                argv[2] = cval1; 
                argv[3] = server1; 
                argv[4] = NULL;
                
                cout << "Step5. Output the result of the ping command" << endl;
                i = execvp(program, argv);
                printf("i=%d ... did it work?\n" ,i);
                exit(-2);
            }
            else if (pid2 > 0)
            {
                if ((pid2 = waitpid(pid2, &status, 0)) < 0)
                {
                    fprintf(stderr, " parent: waitpid error: %s\n", strerror(errno));
                }
            }
            else
            {
                printf("fork fails \n");
            }
        }

        close(fd1[0]);
        close(fd1[1]);

        wait(fd2);
        wait(fd2+1);
        
    }

    infile.close();
    cout << "Step7. End of the program run" << endl;

    return 0;
}