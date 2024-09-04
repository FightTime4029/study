#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <thread>
#include "Mutex.hpp"
int ticket = 10000;
// 如果我们将锁定义为全局变量 我们就不需要调用函数去初始化和销毁了
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// 如何看待锁？
// 1.锁本身也是共享资源！ 全局的资源是要被保护的，锁用来保护全局资源，锁的安全谁来保护呢？
// 2.pthread_mutex_lock、pthread_mutex_unlock：加锁的过程必须是安全的！ 加锁的工程是原子的
// 3.锁如果申请成功，会继续向后执行，如果申请失败，执行流会阻塞
// 4.谁持有锁，谁就进入临界区
using namespace std;
struct ThreadData
{

public:
    ThreadData(std::string name, pthread_mutex_t *mutex) : _name(name), _mutex_p(mutex)
    {
    }

public:
    std::string _name;
    pthread_mutex_t *_mutex_p;
};

void *GetTicket(void *arg)
{
    // ThreadData* td = (ThreadData*)arg;
    string username = static_cast<const char *>(arg);
    while (true)
    {
        // pthread_mutex_lock(td->_mutex_p);
        //  pthread_mutex_lock(&lock);
        {
            LockGuard lg(&lock); //RAII类型的加锁！  (智能指针)
            if (ticket >= 0)
            {
                usleep(1234);
                cout << username << "正在抢票……" << ticket << endl;
                ticket--;
                // pthread_mutex_unlock(td->_mutex_p);
                //  pthread_mutex_unlock(&lock);
            }
            else
            {
                // pthread_mutex_unlock(td->_mutex_p);
                //  pthread_mutex_unlock(&lock);
                break;
            }
        }
        usleep(1000); // 模拟生成一个订单给用户
    }

    return nullptr;
}

int main()
{
// pthread_mutex_t lock;
// pthread_mutex_init(&lock,nullptr);
#define NUM 5
    std::vector<pthread_t> v(NUM);
    // for(int i = 0;i<NUM;++i)
    // {
    //     char buffer[64]={0};
    //     snprintf(buffer,sizeof buffer,"thread %d",i+1);
    //     // ThreadData* td = new ThreadData(buffer,&lock);
    //     pthread_create(&v[i],nullptr,GetTicket,(void *)buffer);
    // }
    thread t1(GetTicket, (void *)"thread 1");
    thread t2(GetTicket, (void *)"thread 2");
    thread t3(GetTicket, (void *)"thread 3");
    thread t4(GetTicket, (void *)"thread 4");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // for(const auto tid:v)
    // {
    //     pthread_join(tid,nullptr);
    // }

    return 0;
}