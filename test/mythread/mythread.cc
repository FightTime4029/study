#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <unistd.h>
#include <cassert>
#include <vector>
using namespace std;

class pthreadNode
{
    public:
    int num;
    pthread_t id;
    char buffer[64];
};

void* thread_routine(void* args)
{
    pthreadNode* pn =static_cast<pthreadNode*>(args); // 安全的强制类型转换
    int cut = 10;
    while(cut)
    {
        cout<< pn->buffer <<':'<< cut-- <<endl;
        sleep(1);
    }

    //pthread_exit((void*)pn->num);  1.调用库返回
    return (void*)pn->num; //2. 直接返回
}


int main()
{
    vector<pthreadNode *> v;
    #define NUM 10
  for(int i = 0; i < NUM ; i++)
  {
    pthreadNode* pn = new pthreadNode;
    pn->num = i+1;
    snprintf(pn->buffer,sizeof(pn->buffer),"%s : %d","pthread sucess",i+1);
    int n = pthread_create(&(pn->id),NULL,thread_routine,pn);
    v.push_back(pn);
  }

sleep(5);
for(int i =0;i<v.size()/2;i++)
{

    pthread_cancel(v[i]->id);
}

  for(auto& e : v)
  {
    void* ret = nullptr;
    pthread_join(e->id,&ret);
    cout<<"已回收线程: "<<e->buffer<<"   NUM:"<<(long long)ret<<endl;
    delete e;
  }

    return 0;
}