#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void run(int times);
int times = 21;

int main(int argc, char const *argv[])
{
	int pid, wpid;
	printf(1, "--------------------io type test start!-----------------------------\n" );
	int start = uptime();
  	for (int i = 0; i < times; ++i)
	{
	    pid = fork();
	    if(pid < 0){
	      printf(1, "io_test: fork failed\n");
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
	printf(1, "--------------------io type test end!-----------------------------\n");
	printf(1, "---------------pass %dms--------------", pass*10/21);
	exit();
}


void run(int times) 
{
	// printf(1, "pid %d is running (%d times)!.\n",getpid(),times);
	// code
	int fd;
	char data[512];
	char filename[] = "test0";

    filename[4] += times;

	memset(data, 'a', sizeof(data));
	if((fd = open(filename, O_CREATE | O_RDWR)) < 0){
		printf(2, "--------------------open and create error-----------------------------\n");
		return;
    }
	if(write(fd, data, sizeof(data)) < 0) {
		printf(2, "--------------------write error-----------------------------\n", fd);
		return;
	}
	close(fd);

  	fd = open(filename, O_RDONLY);
	read(fd, data, sizeof(data));
  	close(fd);
	// printf(1, "pid %d done!.\n",getpid());
}