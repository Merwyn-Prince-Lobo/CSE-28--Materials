#include<pthread.h>
#include<stdlib.h>  
#include<unistd.h>
#include<stdio.h> 
int sum;
void *runner(void *param)
{
    int i, upper = atoi(param);
    sum = 0;
    for(i=1; i<=upper; i++)
        sum += i;
    pthread_exit(NULL);
}  
int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    if(argc != 2)
    {
        fprintf(stderr,"usage: a.out <integer value>\n");
        return -1;
    }
    if(atoi(argv[1]) < 0)
    {
        fprintf(stderr,"Argument must be non-negative\n");
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_create(&tid,&attr,runner,argv[1]);
    pthread_join(tid,NULL);
    printf("sum = %d\n",sum);
    return 0;
}