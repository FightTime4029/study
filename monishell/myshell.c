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
while(myargv[i++] = strtok(NULL," "));


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

waitpid(id,NULL,0);

}
return 0;
}
