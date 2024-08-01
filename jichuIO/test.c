#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
int fd = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC,0666);
if(fd < 0)
 { 
	perror("open:");
	return 1;
 }
//重定向 dup2(int oldfd,int newfd) 这两个参数的意思就是 
//oldfd拷贝到newfd里
dup2(fd,1);

printf("fd : %d\n",fd);
fprintf(stdout,"fd : %d\n",fd);

return 0;
}
