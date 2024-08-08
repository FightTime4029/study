#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#define NUM 1024
#define ARGV_NUM 64
#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
#define trimSpace(start) do{\
	while(isspace(*start)) start++;\
}while(0);

char LineCommand[NUM];
char* myargv[ARGV_NUM]; //指针数组
int lastcode = 9;
int lastsig = 0;
int redirType = NONE_REDIR;
char* redirFile = NULL;


void commandCheck(char* command)
{
	assert(command);
	char* start = command;
	char* end = command + strlen(command);

	while(start < end)
	{
	 if(*start == '>')
	 {
		*start = '\0';
		*start++;
		if(*start == '>')
		{
		 *start ='\0';
		 *start++;
		 redirType = APPEND_REDIR;
		}
		else
		{
		redirType = OUTPUT_REDIR;
		}
	trimSpace(start);
	redirFile = start;
	break;
	}
	 else if(*start == '<')
	 {
		 *start ='\0';
		 start++;

		 redirType = INPUT_REDIR;
		 trimSpace(start);
		 redirFile = start;
		break;
	 }
	 else
	 {
		start++;
	 }
	}
}

int main()
{
while(1)
{
	redirFile = NULL;
	redirType = NONE_REDIR;
//输出提示符
printf("用户名@服务器地址 当前路径# ");
//获取用户输入 输入时用户按下回车会进入到数组中
fflush(stdout);
char* s =fgets(LineCommand,sizeof(LineCommand)-1,stdin);
assert(s != NULL);
(void)s;
//将数组中的\n去掉
LineCommand[strlen(LineCommand)-1] = 0;
//测试用户输入数组中的字符
//printf("test: %s\n",s);


commandCheck(LineCommand);

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
//
switch(redirType)
{
	case NONE_REDIR:
	//什么都不做
	break;
	case INPUT_REDIR:
	{
	int fd = open(redirFile,O_RDONLY);
	if(fd<0)
	{
		perror("open");
		exit(errno);
	}

	dup2(fd,0);
	close(fd);
	break;

	case APPEND_REDIR:
	case OUTPUT_REDIR:
	{
	int flags = O_WRONLY | O_CREAT;
	if(redirType == APPEND_REDIR)
		{
		flags |= O_APPEND;
		}
	else
		{
		flags |= O_TRUNC;
		}
	int fd = open(redirFile,flags,0666);
	if(fd<0)
	{
		perror("open");
		exit(errno);
	}

	dup2(fd,1);
	close(fd);
	break;
	}
	default:
	printf("BUG?\n");
	break;
	}
}



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
