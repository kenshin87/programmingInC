#include <l0typedef.h> 
#include <l1head.h>
#define BUFFERSIZE 1024


//Assuming that the client send a GET request to the server.
//    GET /file HTTP/1.0/r/n/r/n

int hugeWrite(int fileDescriptorPara)
{   // Assume that this is a huge buffer that doesn't overflow
    char buffer[BUFFERSIZE + 1];
    
    int getTxtFd = open("txt", O_RDWR); assert(getTxtFd > 0, "cannot open disk file.");

    int readFlag;
    while(1)
    {
        readFlag = read(getTxtFd, buffer, BUFFERSIZE);
        if (readFlag > 0)
        {
            write(fileDescriptorPara, buffer, readFlag);
        }
        else
        { break; }
    }
    close(getTxtFd);
}

void readAll(int fileDescriptorPara)
{
    char buffer[BUFFERSIZE + 1];
    int readFlag;

    while(1)
    {
        readFlag = read(fileDescriptorPara, buffer, BUFFERSIZE);
        if (readFlag == 0) { break; }
        else 
        {
            buffer[BUFFERSIZE] = '\0';
            printf("%s \n", buffer);
        }
    }
}



int main(int argc, char** argv)
{   
    int serverfd, confd, PORT, clientlen;
    struct sockaddr_in clientSocket;

    PORT = atoi(argv[1]);
    serverfd = getserverfd(PORT);
    printf("listening fd is  %d \n", serverfd); 

    char buffer[BUFFERSIZE + 1];
    
    char c;
    while(1)
    { 
        // The logic here is that, in the subprocee, it needs to close the serverfd, while in the main process, it need to close the confd immediately.

        printf("%s \n", "start of the loop"); 
        clientlen = sizeof(clientSocket); 
        confd     = accept(serverfd, (SA* )&clientSocket, &clientlen);
        int pid = fork();
        if (pid == 0)
        {
            close(serverfd);
            hugeWrite(confd);
            close(confd);
            exit(0);
        }
        close(confd);
    }
    close(serverfd);
}
