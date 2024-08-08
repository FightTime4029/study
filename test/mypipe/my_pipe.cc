#include <iostream>
#include <unistd.h>
#include <cassert>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
using namespace std;
int main()
{
    int pipe_fd[2];
    int n = pipe(pipe_fd);
    assert(n == 0);

    pid_t id = fork();
    assert(id >= 0);

    if(id == 0)
    {
        // child
    int cut = 10;
        while(cut)
        {
            close(pipe_fd[0]);
            char buffer[1024];
            //写端写入数据， 读端不读 那么写入到管道缓冲区满则不会再写入 而是等待读端读取数据
            snprintf(buffer,sizeof buffer,"child working pid:[%d] count:[%d]",getpid(),cut);
            write(pipe_fd[1],buffer,sizeof(buffer));
            cut--;
            sleep(1);
         }
        exit(0);
    }

    // father
    close(pipe_fd[1]);
    while(true)
    {
        char buffer[1024];
        //如果管道内部没有数据， 写端没有关闭 read会阻塞读取  写端如果写入数据后 关闭端口  我们就会读到read返回值为0  那么说明写完了 没有别的数据了
        // 所以读端直接退出
        ssize_t s = read(pipe_fd[0],buffer,sizeof(buffer)-1);

    if( s > 0)
    {
        buffer[s] = 0;
        cout<<"父进程pid:" << getpid() << ' ' << buffer << endl;
    }
    else if(s == 0)
    {
        cout<<"数据读完，关闭读取"<<endl;
        break;
    }

    }

    pid_t a = waitpid(id,nullptr,0);
    assert( a == id);
    close(pipe_fd[0]);

    return 0;
}