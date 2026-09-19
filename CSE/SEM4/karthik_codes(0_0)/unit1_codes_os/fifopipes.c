#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define bsize 50
int main()
{
    int fd;
    char buff[bsize]="";

    char myfifo[bsize]="/tmp/myfifo";
    printf("FIFO named pipe example\n");
    read(0,buff,bsize); //read message from standard input
    mkfifo(myfifo,0666); //create named pipe
    fd=open(myfifo,O_WRONLY); //open named pipe for writing
    write(fd,buff,strlen(buff)+1); //write message to named pipe
    close(fd); //close named pipe
    sleep(20);
    return 0;
}
//gcc fifopipes.c
//./a.out