#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MYPWD "PWD"
int main()
{
char* who = getenv("USER");
if(strcmp(who,"sxj")==0)
{
printf("欢迎伟大的%s同志,%s！\n",who,getenv(MYPWD));
}
else
{
printf("%s同志，欢迎,%s!\n",who,getenv(MYPWD));
}

return 0;
}
