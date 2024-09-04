#include "BlockQueue.hpp"
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <string>
#include "Task.cc"

const std::string oper = "+-*/%";

template<class calTask,class savaTask>
struct BlockQueues
{
    BlockQueue<calTask>* _ctask;
    BlockQueue<savaTask>* _stask;
};

int mymath(int x,int y,char op)
{
    int ret = 0;
    switch(op)
    {
        case '+':
            {
            ret = x+y;
            break;
            }
        case '-':
        {
            ret = x-y;
        break; 
        }
        case '*':
        {
           ret = x * y;
        break; 
        } 
        case '/':
        {
            if( y == 0)
            {
                ret = -1;
                std::cerr<<"除零错误!"<<std::endl;
            } 
            else ret = x/y;
            break;  
        }
        case '%':
        {
             if( y == 0)
            {
                ret = -1;
                std::cerr<<"除零错误!"<<std::endl;
            } 
            else ret = x%y;
              break;
        }
        default:
        {
            break;
        }
    }
    return ret;
}

void Save(const std::string& message)
{
    const std::string target = "./log.txt";

    FILE* fp = fopen(target.c_str(),"a+");
    if(!fp)
    {
        std::cerr<<"fopen fail!"<<std::endl;
    }
    fputs(message.c_str(),fp);
    fputs("\n",fp);

    fclose(fp);
}

void* saver(void* args)
{
    //保存到文件
    BlockQueue<saveTask>* save_bq = (static_cast<BlockQueues<Task,saveTask>*>(args))->_stask;

    while(true)
   {
    saveTask st;
    save_bq->pop(&st);
    st();
    std::cout<<"保存任务完成"<<std::endl;
   } 

   return nullptr;
}

//消费者
void* consumer(void* args)
{
    //类型安全转换
    BlockQueue<Task>* bq = (static_cast<BlockQueues<Task,saveTask>*>(args))->_ctask;
    BlockQueue<saveTask>* save_bq = (static_cast<BlockQueues<Task,saveTask>*>(args))->_stask;
    while(true)
    {
        //进行消费活动
        Task t;
        bq->pop(&t);
        std::string str = t();
        std::cout<<"消费数据: "<< str << std::endl;

        saveTask st(str,Save);
        save_bq->push(st);
        std::cout<<"推出保存任务...成功！"<<std::endl;
    }
    return nullptr;
}
//生产者
void* producer(void* args)
{
    BlockQueue<Task>* bq = (static_cast<BlockQueues<Task,saveTask>*>(args))->_ctask;
    while(true)
    {
        //进行生产活动
        int x = rand() % 1000;
        int y = rand() % 20;
        int opindex = rand() % oper.size(); 
        Task t(x,y,oper[opindex],mymath);
        bq->push(t);
        // std::cout<<"生产数据："<< 
        sleep(1);
    }
    return nullptr;
}



int main()
{
    srand(time(nullptr) ^ getpid());
    // BlockQueues<Task,saveTask>* bqs = new BlockQueues<Task,saveTask>();
   BlockQueues<Task,saveTask>* bqs = new BlockQueues<Task,saveTask>();
   bqs->_stask = new BlockQueue<saveTask>();
   bqs->_ctask = new BlockQueue<Task>();

    pthread_t c, p,s;
    pthread_create(&c,nullptr,consumer,bqs);
    pthread_create(&p,nullptr,producer,bqs);
    pthread_create(&s,nullptr,saver,bqs);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    pthread_join(s,nullptr);

    delete bqs;
    delete bqs->_ctask;
    delete bqs->_stask;
    return 0;
}