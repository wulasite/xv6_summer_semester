#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void run(int);

int times = 21;
int matc[10][10] = {
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
};

int mata[10][10] = {
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
};

int matb[10][10] = {
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
};
int size = 10;


int main(int argc, char const *argv[])
{
	int pid, wpid;
	printf(1, "--------------------cpu type test start!-----------------------------\n" );
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
	printf(1, "--------------------cpu type test end!-----------------------------\n---------------pass %dms--------------", pass*10);

	exit();
}

void run(int times) 
{
	printf(1, "pid %d is running (%d times)!.\n",getpid(),times);
	int count = 24;
	while(count-- > 0) {
	    for (int i = 0; i < size; i ++){
	        for(int j = 0; j < size; j ++){
	            matc[i][j] = 0;
	            for(int k = 0; k < size; k ++){
	            	matc[i][j] += mata[i][k] * matb[k][j];
	            }
	        }
	    }
	    for(int i = 0; i < size; i ++){
	    	for(int j = 0; j < size; j ++){
	    		mata[i][j] = matb[i][j] = matc[i][j];
	    	}
	    }
	}

	printf(1, "pid %d done!.\n",getpid());
}
