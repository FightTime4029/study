#include <iostream>
#include <signal.h>
#include <unistd.h>

#define Max_PendNum 31

void show_pending(const sigset_t& set)
{
    for(int i = Max_PendNum; i >= 1;i--)
    {
        if(sigismember(&set,i))
        {
            std::cout<<"1";
        }
        else
        {
            std::cout<<'0';
        }
    }
    std::cout<<std::endl;
}

void handler(int signo)
{
    std::cout<<"捕获到信号，信号为:"<<signo<<std::endl;
}

int main()
{
    //创建信号结构体
    sigset_t block,oblock,set;
    //结构体初始化
    sigemptyset(&block);
    sigemptyset(&oblock);
    sigemptyset(&set);
    //将2号信号添加到阻塞结构体中
    sigaddset(&block,2);

    //将2号信号放入进程PCB中的block信号位图   
    //第二个参数是我们想要阻塞那些或者那个信号
    //第三个参数是输入型参数，它会将未更改前位图的状态写入第三个参数中
    //屏蔽信号 设置进内核
    sigprocmask(SIG_BLOCK,&block,&oblock);
    int cur = 10;
    // signal(2,handler);
    while(true)
    {
        //获取当前pending信号位图结构
        sigpending(&set);
        show_pending(set);
        sleep(1);
        if(cur-- == 5)
        {
            //将阻塞的信号 解除阻塞 
        sigprocmask(SIG_SETMASK,&oblock,&block); //一旦对特定信号解除屏蔽，一般OS都要至少立马递达一个信号
        }
    }

    return 0;
}