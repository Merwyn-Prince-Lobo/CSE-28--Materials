#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
int g=0;
void *myfun(void *vargp)
{
    int *myid=(int *)vargp;
    static int s=0;
    int l=0;
    l++;
    s++;
    g++;
    printf("Thread %d: static=%d, global=%d, local=%d\n",*myid,s,g,l);
    pthread_exit(NULL);
}
int main()
{
    pthread_t tid;
    int i;
    for(i=0;i<3;i++)
    {
        pthread_create(&tid,NULL,myfun,(void *)&tid);
        pthread_join(tid,NULL);
    }
    printf("in main code\n");
    return 0;
}