#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
char* who = getenv("USER");
if(strcmp(who,"sxj")==0)
{
printf("欢迎伟大的%s同志！\n",who);
}
else
{
printf("%s同志，欢迎!\n",who);
}

return 0;
}
