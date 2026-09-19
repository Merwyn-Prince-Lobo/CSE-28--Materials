#include<windows.h>
#include<stdio.h>
DWORD sum;
DWORD WINAPI summation(LPVOID param)
{
    sum=2;
    DWORD Upper=*(DWORD *)param;
    for(DWORD i=1;i<=Upper;i++)
        sum+=i;
    return 0;
}
int main(int argc,char *argv[])
{
    DWORD threadId;
    HANDLE Threadhandle;
    int param;
    if(argc!=2)
    {
        fprintf(stderr,"an interger parameter is needed\n");
        return -1;
    }
    param=atoi(argv[1]);
    if(param<0)
    {
        fprintf(stderr,"the parameter must be non-negative\n");
        return -1;
    }
    Threadhandle=CreateThread(NULL,0,summation,&param,0,&threadId);
    if(Threadhandle!=NULL)
    {
        WaitForSingleObject(Threadhandle,INFINITE);
        printf("sum=%d\n",sum);
        CloseHandle(Threadhandle);
    }
    return 0;
    
}