#pragma once
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>


#define PathName "."
#define Proj_Id 66
#define MAX_SZIE_SHM 4096

key_t get_key()
{
    key_t k = ftok(PathName,Proj_Id);
      if(k == -1)
    {
        std::cerr<<"get_key:"<<strerror(errno);
    }
    return k;
}

int get_shmid()
{
    key_t k = get_key();
     int shmid = shmget(k,MAX_SZIE_SHM,IPC_CREAT);
     if(shmid == -1)
    {
        std::cerr<<"get_shmid:"<<strerror(errno);
    }
     return shmid;
}

int create_shm(key_t k)
{

    int shmid = shmget(k,MAX_SZIE_SHM, IPC_CREAT | IPC_EXCL | 0600);
       if(shmid == -1)
    {
        std::cerr<<"create_shm:"<<strerror(errno);
    }
    return shmid;
}

void* attch_shm(int shmid)
{
    //第二个参数 是想将共享区放在那个地址 一般我们也不知道地址所以传空指针默认
    //第三个参数 以什么方式连接 0就是读写
    void* ret =shmat(shmid,nullptr,0);
    if((long long)ret == -1l)
    {
        std::cerr<<"attch_shm:"<<strerror(errno);
    }
    return ret;
}

void del_shmdt(void* start)
{
    int ret = shmdt(start);
    if(ret == -1)
    {
        std::cerr<<"del_shmdt:"<<strerror(errno);
    }
}

void del_shm(int shmid)
{
    int ret = shmctl(shmid,IPC_RMID,nullptr);
    if(ret == -1)
    {
        std::cerr<<"del_shm:"<<strerror(errno);
    }
}