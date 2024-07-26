#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
int main()
{
pid_t id = fork();

if(id == 0)
 {
//child
int cur = 5;
while(cur)
 {
printf("child runing pid:%d ppid:%d cur:%d\n",getpid(),getppid(),cur--);
sleep(1);
 } 
 } 
//parent
int status = 0;
//进程等待 阻塞等待
int ret = waitpid(id,&status,0);
//int ret = waitpid(id,&status,WNOHANG) 非阻塞等待
if(ret > 0)
 {
//WIFEXITED 一个宏检测子进程是否正常退出 正常退出就返回真 否则假
if(WIFEXITED(status)) 
 {
//WEXITSTATUS 返回子进程退出码
printf("exit code:%d\n",WEXITSTATUS(status));
 }
 }
return 0;
}
