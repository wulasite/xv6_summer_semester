#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void run(int);

// int times = 21;
// int size = 10;

int** matc;

void Minite(int size){
    matc = (int**) malloc(sizeof(int*) * size);
    for(int i = 0; i < size; i++){
        matc[i] = (int*)malloc((sizeof(int) * size));
        for(int j = 0; j < size; j++){
            matc[i][j] = 7;
        }
    }
}

void Mdelete(int size){
    for(int i = 0; i < size; i++){
        free(matc[i]);
    }
    free(matc);
}

int main(int argc, char const *argv[])
{
	int pid, wpid;
	// printf(1, "--------------------cpu type test start!-----------------------------\n" );
	int x[4] = {10, 20, 50, 100};
	int i = 0;

	for (int y=x[i]; i<4; y=x[i])
	{
		i++;
        int start = uptime();
	    pid = fork();
	    if(pid < 0){
	      printf(1, "cpu_test: fork failed\n");
	      exit();
	    }
	    if(pid == 0){
	      run(y);
	      exit();
	    }
	    while((wpid=wait()) >= 0 && wpid != pid)
	      printf(1, "zombie!\n");
        int end = uptime();
        int pass = end-start ;
        // printf(1, "--------------------cpu type test end!-----------------------------\n");
        printf(1, "--------------------scale %d cpu type test pass %dms--------------\n", y, pass*10);
	}
	

	exit();
}

void run(int size) 
{

	//    printf(1, "pid %d is running (%d times)!.\n",getpid(),times);
    int count = 24;
    // int matc[size][size];
    Minite(size);
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            matc[i][j] = 7;
        }
    }
    while(count-- > 0) {
        for (int i = 0; i < size; i ++){
            for(int j = 0; j < size; j ++){
                matc[i][j] = 0;
                for(int k = 0; k < size; k ++){
                    matc[i][j] += matc[i][k] * matc[k][j];
                }
            }
        }
        for(int i = 0; i < size; i ++){
            for(int j = 0; j < size; j ++){
                matc[i][j] = matc[i][j] + matc[i][j];
            }
        }
    }

    for(int i = 0; i < size; i++){
    	free(matc[i]);
    }
    free(matc);

}
