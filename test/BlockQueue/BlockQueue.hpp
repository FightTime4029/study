#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>

const int gmaxcap = 5;



template<class T>
class BlockQueue
{
public:
    BlockQueue(const int& maxcap= gmaxcap):_maxcap(maxcap)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_pcond,nullptr);
        pthread_cond_init(&_ccond,nullptr);
    }

    void push(const T& in)
    {
        pthread_mutex_lock(&_mutex);
        //如果队列里面有数据 则阻塞

       

        //细节二 当有一个消费者 一堆生产者的时候 我们只消费一个 但是给生产者发信号使用pthread_cond_boardcast
        //会调所有在条件变量下的线程  如果是if 则只判断一次挂机返回时有可能早能越界。
        // 充当条件编译的语法必须是while 不能用if
        while(is_full())
        {
        //细节一 pthread_cond_wait 这个函数的第二个参数，必须是我们正在使用的互斥锁
        // a. pthread_cond_wait 该函数调用的时候 会以原子性的方式，将锁释放
        // b. pthread_cond_wait 返回的时候 会自动获取你所传入的锁
            pthread_cond_wait(&_pcond,&_mutex);
        }
        //到这里就说明队列内数据处于低水平线 需要生产
        _q.push(in);

        pthread_cond_signal(&_ccond);

        pthread_mutex_unlock(&_mutex);
    }

void pop(T* out)
{
    pthread_mutex_lock(&_mutex);
    while(is_empty())
    {
        pthread_cond_wait(&_ccond,&_mutex);
    }

    *out = _q.front();
    _q.pop();

    pthread_cond_signal(&_pcond);
    pthread_mutex_unlock(&_mutex);
}


     ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_pcond);
        pthread_cond_destroy(&_ccond);
    }

 private:
    bool in_lowweater()
    {
        return _q.size() <= _maxcap/3;
    }

    bool in_highweater()
    {
        return _q.size() >=_maxcap/3*2;
    }
    bool is_empty()
    {
        return _q.empty();
    }

    bool is_full()
    {
        return _q.size() == _maxcap;
    }


private:
    std::queue<T> _q;
    int _maxcap; //队列中元素上线
    pthread_mutex_t _mutex;
    pthread_cond_t _pcond; //生产者条件变量
    pthread_cond_t _ccond; //消费者条件变量
};