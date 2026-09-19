#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
sem_t mutex;
void *mythread(void *arg)
{
    sem_wait(&mutex);
    printf("Entered..\n");
    sleep(4);
    printf("Exiting...\n");
    sem_post(&mutex);
}
int main()
{   pthread_t t1,t2;
    sem_init(&mutex,0,1);
    pthread_create(&t1,NULL,mythread,NULL);
    pthread_create(&t2,NULL,mythread,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&mutex);
    return 0;
}