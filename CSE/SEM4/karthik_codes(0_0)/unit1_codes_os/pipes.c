#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define bsize 20
int main()
{
    char wmsg[bsize]="Hello, World!";
    char rmsg[bsize]="";
    int fd[2];
    pid_t p1;
    if (pipe(fd)==-1) //create a pipe
    {
        printf("Pipe Failed");
        return 0;
    }
    p1=fork();
    if (p1>0) //parent process
    {
        close(fd[0]); //close reading 
        write(fd[1],wmsg,strlen(wmsg)+1); //write message to pipe
        close(fd[1]); //close writing 
        wait(NULL); //wait for child process 

    }
    else if (p1==0) //child process
    {
        close(fd[1]); //close writing 
        read(fd[0],rmsg,bsize); //read message from pipe
        printf("Child is reading a message : %s\n",rmsg);
        close(fd[0]); //close reading 
    }
    else{
        printf("Fork Failed");
    }
    return 0;

}
//gcc pipes.c
//./a.out