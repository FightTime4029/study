#include "RingQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
using namespace std;
//生产者
void* Producter(void* args)
{
    RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    while(true)
    {
        //生产活动
        int data= rand() % 100+1;
        rq->push(data);
        std::cout<<"生产活动: "<< data <<std::endl;
        sleep(1);
    }
}

//消费者
void* Consumer(void* args)
{
    RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    while(true)
    {
        //消费活动
        int n;
        rq->pop(&n);
        std::cout<<"消费活动: "<< n <<std::endl;
    }
}



int main()
{
    srand((unsigned int)time(nullptr) ^ getpid());
    RingQueue<int>* rq = new RingQueue<int>();
    pthread_t c[4],p[8];

    for(int i = 0;i<4;i++) pthread_create(p+i,nullptr,Producter,rq);
    for(int i = 0; i<8;i++) pthread_create(c+i,nullptr,Consumer,rq);

    for(int i = 0; i<4;i++)pthread_join(c[i],nullptr);
    for(int i = 0;i<8;i++) pthread_join(p[8],nullptr);

    return 0;
}