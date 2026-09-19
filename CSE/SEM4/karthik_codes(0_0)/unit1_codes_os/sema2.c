#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
void *myth1(void *arg)
{
    char s[]="helloworld";
    int l=strlen(s);
    for(int i=0;i<l;i++)
    {
        putchar(s[i]);
        fflush(stdout);
        sleep(2);
    }
}
void *myth2(void *arg)
{
    char s[]="HELLOWORLD@";
    int l=strlen(s);
    for(int i=0;i<l;i++)
    {
        putchar(s[i]);
        fflush(stdout);
        sleep(2);
    }
}
int main()
{   pthread_t t1,t2;
    pthread_create(&t1,NULL,myth1,NULL);
    pthread_create(&t2,NULL,myth2,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}