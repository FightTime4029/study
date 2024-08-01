#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define NUM 1024
#define ARGV_NUM 64
char LineCommand[NUM];
char* myargv[ARGV_NUM];
int lastcode = 9;
int lastsig = 0;

int main()
{
while(1)
{
//输出提示符
printf("用户名@服务器地址 当前路径# ");
//获取用户输入 输入时用户按下回车会进入到数组中
char* s =fgets(LineCommand,sizeof(LineCommand)-1,stdin);
assert(s != NULL);
(void)s;
//将数组中的\n去掉
LineCommand[strlen(LineCommand)-1] = 0;
//测试用户输入数组中的字符
//printf("test: %s\n",s);

//进行字符串切割
myargv[0] = strtok(LineCommand," ");
int i = 1;
//初始判断执行语句
if(myargv[0] != NULL && strcmp(myargv[0],"ls") == 0)
{
	myargv[i++] = "--color=auto";
}

while(myargv[i++] = strtok(NULL," "));
//如果是cd命令，不需要创建子进程，让shell自己执行对应的命令，本质就是执行系统接口
//像这种不需要子进程来执行，而是让shell来执行的命令 -- 叫做内建(内置)命令
if(myargv[0] != NULL && strcmp(myargv[0],"cd") == 0)
{
	chdir(myargv[1]);
	continue;
}

if(myargv[0] != NULL && strcmp(myargv[0],"echo") == 0)
{
 if(strcmp(myargv[1],"$?") == 0)
{
printf("Code:%d Sig:%d\n",lastcode,lastsig);
}
continue;
}
#ifdef DEBUG
 for( i = 0; myargv[i];i++)
 {
 printf("argv[%d]: %s\n",i,myargv[i]);
 }
#endif

pid_t id = fork();

assert(id != -1);

if(id == 0)
{
//child
execvp(myargv[0],myargv);
exit(1);
}
int status = 0;
pid_t ret = waitpid(id,&status,0);
assert(ret>0);
lastcode = (status>>8) & 0xff;
lastsig = status & 0x7f;


}
return 0;
}
