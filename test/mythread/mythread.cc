#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cassert>
using namespace std;


void* thread_routine(void* args)
{
  while(true)
    {
        cout<<"我是新线程!   "<< (char*)args  <<endl;
        sleep(1);
    }
}


int main()
{
    pthread_t tid;
    int n = pthread_create(&tid,NULL,thread_routine,(void*)"thread one");
    assert(n==0);

    while(true)
    {
        cout<<"我是主线程 ! "<<endl;
        sleep(1);

    }
    return 0;
}