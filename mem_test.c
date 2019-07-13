#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void run(int times);
int times = 21;

int main(int argc, char const *argv[])
{
	int pid, wpid;
	printf(1, "--------------------mem type test start!-----------------------------\n");
	int start = uptime();
	for (int i = 0; i < times; ++i)
	{
	    pid = fork();
	    if(pid < 0){
	      printf(1, "cpu_test: fork failed\n");
	      exit();
	    }
	    if(pid == 0){
	      run(i);
	      exit();
	    }
	    while((wpid=wait()) >= 0 && wpid != pid)
	      printf(1, "zombie!\n");
			
	}
	int end = uptime();
	int pass = end-start ;
	printf(1, "--------------------mem type test end!-----------------------------\n");
	printf(1, "---------------pass %dms--------------", pass*10);
	exit();
}

void run(int times)
{
	printf(1, "pid %d is running (%d times)!.\n",getpid(),times);
	int i;
	char * p;
	p = (char *) malloc(sizeof(char) * 2001);
	for(i = 0; i <= 1000; i++){
		*(p + i) = i&0x0f;
		// printf(1, "%s ", p[i]);
	}
	for(i = 0; i <= 1; i++){
		*(p + i + 1000) = (*(p + i))&0x0f;
	}
	for(i = 0; i <= 1000; i++){
		*(p + i) = i&0x0f;
	}
	free(p);
	printf(1, "pid %d done!.\n",getpid());
}
