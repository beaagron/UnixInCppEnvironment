#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <pthread.h>

using namespace std;

struct thread_data {
    int fd1[2];
    int index;
    string cval;
    string server;
};

void *thr_fn(void *arg)
{
    // initializing structure, using pointer to make as argument
    struct thread_data *tdata;
    tdata = (struct thread_data *) arg;
    int i;
    char *argv[5];
    char program[80];
    // conversions
    char server1[tdata->server.length() + 1];
    strcpy(server1, tdata->server.c_str());
    char cval1[tdata->cval.length() + 1];
    strcpy(cval1, tdata->cval.c_str());

    cout << "\tThread " << (i + 1) <<  "(tid=" << pthread_self() << ") to run: ping -c " << tdata->cval << tdata->server << endl << endl;
    strcpy(program, "ping");
    argv[0] = "ping";
    argv[1] = "-c"; 
    argv[2] = cval1; 
    argv[3] = server1; 
    argv[4] = NULL;

    int pid = fork();
    if(pid < 0)
    {
        cout << "error with fork" << endl;
    }
    else if (pid == 0)
    {
        dup2(tdata->fd1[1], STDOUT_FILENO);
        close(tdata->fd1[1]);
        i = execvp(program, argv);
        printf("i=%d ... did it work?\n" ,i);
    }
    else
    {
        close(tdata->fd1[1]);
    }

    return (void*) 0;
}

int main() 
{
    // reading files in from a3ping1Data.txt
    cout << "Step1. a4ping1 process starts. pid=" << getpid() << endl;
    ifstream infile;
    infile.open("a4ping1Data.txt");
    cout << "Step2. Ready to read the input file: a3ping1Data.txt" << endl;

    // looping through until there is no more data to be read
    string command;
    string server;
    string cval;
    string numThreads;
    int numThreads2;
    int cval2;
    
    while(infile)
    {

        // reading information in, converting it to be digestable for program
        infile >> server;
        infile >> cval;
        infile >> numThreads;
        cout << "Step3. Input Data is: " << server << ' ' << cval << ' ' << numThreads << endl << endl;
        command = "ping -c " + cval + " " + server;
        stringstream numThreads1(numThreads);
        numThreads1 >> numThreads2;
        stringstream cval1(cval);
        cval1 >> cval2;
        int totalPackets = (cval2 * numThreads2);

        // creating the threads in a for loop
        cout << "Step4. Create Threads" << endl;
        pid_t parentpid = getpid();
        //pid_t pid1;
        //pid_t pid2;
        //int fd2[2];
        pthread_t threads[numThreads2];
        struct thread_data td[numThreads2];
        int fileDesc [numThreads2][2];
        int rc;
        int status;
        char bufferThreads [numThreads2] [4096];
        int bytes;

        for(int i = 0; i < numThreads2; i++)
        {
            // initalizing elements for the struct
            int fd[2];
            pipe(fd);
            td[i].fd1[0] = fd[0];
            td[i].fd1[1] = fd[1];
            fileDesc[i][0] = fd[0];
            fileDesc[i][1] = fd[1];
            td[i].index = i;
            td[i].cval = cval;
            td[i].server = server;

            // creating the thread, testing for errors
            rc = pthread_create(&threads[i], NULL, thr_fn, (void *)&td[i]);
            if (rc)
            {
                cout << "error: unable to create thread" << rc << endl;
                exit(-1);
            }
            // ??? pthread join ?
            
        }

        for(int i =0; i < numThreads2; i++)
        {
            pthread_join(threads[i], NULL);
        }
    

        //pthread_exit(NULL);
        cout << "\nStep5. All Threads are terminated. The parent processing the results" << endl;
        for(int i = 0; i < numThreads2; i++)
        {
            cout << "\tProcessing Thread " << (i + 1) << " result" << endl;

            // read and print each line to be processed
            while((bytes = read(fileDesc[i][0], bufferThreads[i], sizeof(bufferThreads[i]))) != 0)
            {
                bufferThreads[i] [bytes] = 0;
                printf("%s", bufferThreads[i]);
            }
        }

        //close(fd1[0]);
        //close(fd1[1]);

        //wait(fd2);
        //wait(fd2+1);

        cout << "\nStep6. Summary of ping command" << endl;
        cout << "\tthe input line: ping -c " << cval << ' ' << server << endl;
        cout << "\ttotal number of packets: " << totalPackets << endl;
        cout << "\t end of case" << endl << endl;
        
    }

    infile.close();
    cout << "\nStep7. End of the program run" << endl;

    return 0;
}