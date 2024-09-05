#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cassert>

static const int gcap = 5;

template <class T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        int n = sem_wait(&sem);
        assert(n == 0);
    }

    void V(sem_t &sem)
    {
        int n = sem_post(&sem);
        assert(n == 0);
    }

public:
    RingQueue(const int &cap = gcap) : _cap(cap), _ringqueue(cap)
    {
        int n = sem_init(&_spacesem, 0, _cap);
        assert(n == 0);

        n = sem_init(&_datasem, 0, 0);
        assert(n == 0);

        _producterstep = 0;
        _consumerstep = 0;

        pthread_mutex_init(&_pmutex, nullptr);
        pthread_mutex_init(&_cmutex, nullptr);
    }

    void push(const T &in)
    {
        P(_spacesem); // 先对信号量进行预定
        // 再去申请锁
        pthread_mutex_lock(&_pmutex);
        _ringqueue[_producterstep++] = in;
        _producterstep %= _cap;
        pthread_mutex_unlock(&_pmutex);
        V(_datasem);
    }

    void pop(T *out) // 输出型参数
    {
        P(_datasem);
        pthread_mutex_lock(&_cmutex);
        *out = _ringqueue[_consumerstep++];
        _consumerstep %= _cap;
        pthread_mutex_unlock(&_cmutex);
        V(_spacesem);
    }

    ~RingQueue()
    {
        sem_destroy(&_spacesem);
        sem_destroy(&_datasem);
    }

private:
    std::vector<T> _ringqueue;
    int _cap;
    sem_t _spacesem;
    sem_t _datasem;
    int _producterstep;
    int _consumerstep;
    pthread_mutex_t _pmutex;
    pthread_mutex_t _cmutex;
};