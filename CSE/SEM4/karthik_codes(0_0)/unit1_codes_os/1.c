#include<stdio.h>
#include<unistd.h>
int main()
{
    int x=0;
    pid_t p1=x;
    p1= fork();
    if( p1>0)
    {
        x=x-1;
        printf("this is parent process id=%d, x=%d\n",getpid());


    }
    else if(p1==0)
    {
        x=x+1;
        printf("this is child process  id=%d, x=%d\n",getpid());
    }
    else //fork failed
    {
        printf("unsucessfull");
    }
}