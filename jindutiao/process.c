#include "process.h"

void ProcessOn()
{
 int cut = 0 ;
 char bar[NUM];
 char* lable="|\\-/";
 memset(bar,'\0',sizeof(bar));

 while(cut <= 100)
 { 
  printf("[%-100s][%3d%%][%c]\r",bar,cut,lable[cut%4]);
  fflush(stdout);
  bar[cut++] =STYLE;
  usleep(50000);
 } 

printf("\n");
}

