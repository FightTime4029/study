#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <cassert>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#define MAKE_SEED() srand((unsigned long)time(nullptr) ^ getpid() ^ rand() % 1234)
#define Process_Sum 3
/////////////////////////////////////////任务列表///////////////////////////
typedef void (*func)();

void flushTask()
{
   std::cout<< getpid() <<' '<<"刷新 任务"<< std::endl;
    sleep(1);
}

void IoTask()
{
    std::cout <<getpid() <<' '<< "IO 任务" << std::endl;
    sleep(1);
}

void downloadTask()
{
    std::cout <<getpid() <<' '<< "下载 任务" << std::endl;
    sleep(1);
}

void loadTaskFunc(std::vector<func>* out)
{
    assert(out);
    out->push_back(flushTask);
    out->push_back(IoTask);
    out->push_back(downloadTask);
}

/////////////////////////////////////////多线程阶段///////////////////////
class Sub_EP
{
public:
    Sub_EP(pid_t id, int writefd)
        : _ID(id), _writefd(writefd)
    {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "process-%d[pid(%d) - fd(%d)]", num++, _ID, _writefd);
        _name = buffer;
    }

public:
    std::string _name;
    pid_t _ID;
    int _writefd;
    static int num;
};

int Sub_EP::num = 0;

void send_process(const Sub_EP &sub, int f_index)
{
    std::cout << "send task num : " << f_index << " send to ->" << sub._name << std::endl;
    ssize_t n = write(sub._writefd, &f_index, sizeof(f_index));
    assert(n == sizeof(int));
    (void)n;
}

int recvTask(int readfd)
{
    int code = 0;
    ssize_t n = read(readfd, &code, sizeof(code));
    if (n == sizeof(int))
        return code;
    else
        return -1;
}

void create_child_process(std::vector<Sub_EP>& subs,std::vector<func>& funcMap)
{
    // 创建子进程，保存子进程的id和管道通信信道
    for(int i = 0; i < Process_Sum; i++)
    {
        // 创建数组 打开管道文件 保存通信信道  交给子进程 读端
        int fds[2];
        int n = pipe(fds);
        assert(n == 0);
        (void)n;
        pid_t id = fork();

        if (id == 0)
        {
            close(fds[1]);
            // child 接受commend_code 执行命令
            while(true)
            {
                int command_code = recvTask(fds[0]);
               
                 if(command_code >= 0 && command_code < funcMap.size())
                {
                    funcMap[command_code]();
                }
                else if(command_code == -1) break;
            }
            exit(0);
        }
        close(fds[0]);
        Sub_EP sep(id, fds[1]);
        subs.push_back(sep);
    }
}

void load_blue_contrl(const std::vector<Sub_EP>& subs, const std::vector<func>& funcMap,int count)
{
    int subs_size = subs.size();
    int func_size = funcMap.size();
    // int count = 3; // 0 永远循环
    bool forever = count == 0 ? true:false;
    while (true)
    {
        int p_index = rand() % subs_size;
        int f_index = rand() % func_size;
        send_process(subs[p_index], f_index);

        sleep(1);

        if(!forever)
        {
            count--;
            if(count == 0) break;
        }
    }

    for(int i = 0;i<subs_size ;i++)
    close(subs[i]._writefd);
}

void waitprocess(const std::vector<Sub_EP>& subs)
{
    for(int i = 0;i<subs.size();i++)
    {
        pid_t id = waitpid(subs[i]._ID,nullptr,0);
        std::cout<< "waitprocess sucess ..."<< id<<std::endl;
    }
}


int main()
{
    MAKE_SEED();
    std::vector<Sub_EP> subs;
    std::vector<func> funcMap;
    // 加载任务
    create_child_process(subs, funcMap);
    loadTaskFunc(&funcMap);
    // 给子进程分配任务 采用负载均衡
    int count = 3;
    load_blue_contrl(subs,funcMap,count);

    // 回收子进程
    waitprocess(subs);
    return 0;
}