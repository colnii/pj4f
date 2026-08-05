#include<stdio.h>
#include<pthread.h>

void* run(void* something)
{
    int id = *(int*) something;
    printf("Hello from Thread %d !\n", id);
    return NULL;
}

int main()
{
    int id = 1, id2 = 2;
    pthread_t thread,thread2;
    pthread_create(&thread,NULL,&run,(void*) &id);
    pthread_create(&thread2,NULL,&run,(void*) &id2);
    pthread_join(thread,NULL);
    pthread_join(thread2,NULL);
}