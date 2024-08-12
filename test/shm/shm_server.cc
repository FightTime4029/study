#include "comm.hpp"

int main()
{
    //获取key
    key_t k =get_key();
    //将key传给创建共享内存函数
    int shmid = create_shm(k);
    //进行关联
    char* start =(char*)attch_shm(shmid);
    //使用
    while(true)
    {
        sleep(1);
        printf("client say : %s\n",start);
    }

    //去关联
    del_shmdt(start);

    //回收共享内存
    del_shm(shmid);
    return 0;
}