#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define bsize 50
//this program is to read 
int main()
{
    int fd;
    char buff[bsize]="";

    char myfifo[bsize]="/tmp/myfifo";
   
    mkfifo(myfifo,0666); //create named pipe
    fd=open(myfifo,O_RDONLY); //open named pipe for reading
    read(fd,buff,bsize); //read message from named pipe
    printf("info read is : %s\n",buff);
    close(fd); //close named pipe
    
    return 0;
}
//gcc fifopipes1.c
//./a.out