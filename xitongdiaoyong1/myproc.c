#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
//创建子进程  fork是一个函数  函数执行前：只有一个父进程 函数执行后：父进程+子进程
pid_t id = fork();

if(id == 0)
{
//子进程
 while(1)
{
printf("子进程,我的PID是 %d,我的父进程PID是 %d，ID是 %d\n",getpid(),getppid(),id);
sleep(1);
}
}
else if(id>0)
{
//父进程
 while(1)
{
printf("父进程,我的PID是 %d,我的父进程PID是 %d,ID是 %d\n",getpid(),getppid(),id);
sleep(2);
}
}
else
{
//其他情况不做处理
}

// while(1)
//{

//printf("这是一个进程,我的PID是 %d,我的父进程PID是 %d\n",getpid(),getppid());

//sleep(1);
//}

return 0;
}
