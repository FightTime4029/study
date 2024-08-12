#include "comm.hpp"

int main()
{
   int shmid = get_shmid();
   //关联共享内存
    char* start = (char*)attch_shm(shmid);

    //使用
   while(true)
   {
     int cut = 0;
    snprintf(start,MAX_SZIE_SHM,"hello bro I am [%d] this is [%d] times to you\n",getpid(),++cut);
     sleep(1);
   }
    //去关联
    del_shmdt(start);
   
    return 0;
}