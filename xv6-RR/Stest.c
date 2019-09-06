//
// Created by 王珅祎 on 2019/9/4.
//

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int** matc;

int** Minite(int size){
//    int** matc = (int**) malloc(sizeof(int*) * size);
    matc = (int**) malloc(sizeof(int*) * size);
    for(int i = 0; i < size; i++){
        matc[i] = (int*)malloc((sizeof(int) * size));
        for(int j = 0; j < size; j++){
            matc[i][j] = 7;
        }
    }
    return matc;
}

void Mdelete(int** matc, int size){
    for(int i = 0; i < size; i++){
        free(matc[i]);
    }
    free(matc);
}

void* io_task(void* arg){
//    printf(1, "pid %d is running (%d times)!.\n",getpid(), times);
    int fd;
    char data[*(int*)arg * 50];
//    printf(1, "io test for scale %d\n", *(int*)arg);
    char filename[] = "SampleFile";

//    filename[4] += times;

    memset(data, 'a', sizeof(data));
    if((fd = open(filename, O_CREATE | O_RDWR)) < 0){
        printf(2, "--------------------open and create error-----------------------------\n");
        exit();
    }
    if(write(fd, data, sizeof(data)) < 0) {
        printf(2, "--------------------write error---------------------------------------\n", fd);
        exit();
    }
    close(fd);

    fd = open(filename, O_RDONLY);
    read(fd, data, sizeof(data));
    close(fd);
//    printf(1, "pid %d done!.\n",getpid());

    exit();
}

void* cpu_task(void* arg){
//    printf(1, "pid %d is running (%d times)!.\n",getpid(),times);
    int count = 24;
    int size = *(int*)arg;
//    printf(1, "cpu test for scale %d\n", size);
//    int ** matc = Minite(size);
//    int matc[size][size];
//    for(int i = 0; i < size; i++){
//        for(int j = 0; j < size; j++){
//            matc[i][j] = 7;
//        }
//    }

    while(count-- > 0) {
        for (int i = 0; i < size; i ++){
            for(int j = 0; j < size; j ++){
//                matc[i][j] = 0;
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
//    Mdelete(matc, size);
//    printf(1, "pid %d done!.\n",getpid());

    exit();
}

void* mem_task(void* arg){
//    printf(1, "pid %d is running (%d times)!.\n", getpid(), times);

    int size = 1000;
    char * p = (char *) malloc(sizeof(char) * 2 * size);
//    printf(1, "memory test for scale %d\n", size);

    for(int i = 0; i < size; i++){
        *(p + i) = i&0x0f;
        // printf(1, "%s ", p[i]);
    }
    for(int i = 0; i < size; i++){
        *(p + i + size) = (*(p + i))&0x0f;
    }
    for(int i = 0; i < 2 * size; i++){
        *(p + i) = i&0x0f;
    }
    free(p);
//    printf(1, "pid %d done!.\n",getpid());

    exit();
}

int main(int argc, char **argv){
    int* size = (int*)malloc(sizeof(int));
    *size = atoi(argv[1]);
    int runtimes;
    int time;
    int NUM_THREADS = 1;
    Minite(*size);

    // Allocate stacks and args and make sure we have them all
    // Bail if something fails

//    while(*size <= 210){
//        void *stacks[NUM_THREADS * 3];
//        for (int i = 0; i < NUM_THREADS * 3; i++) {
//            stacks[i] = (void*) malloc(4096);
//            if (!stacks[i]) {
//                printf(1, "main: could not get stack for thread %d, exiting...\n");
//                exit();
//            }
//        }
//        runtimes = 4;
//        time = uptime();
//        while(runtimes--){
//            for (int i = 0; i < NUM_THREADS; i++) {
//                thread_create(cpu_task, size, stacks[i * 3]);
//                thread_create(mem_task, size, stacks[i * 3 + 1]);
//                thread_create(io_task, size, stacks[i * 3 + 2]);
//            }
//
//            // Wait for all children
//            for (int i=0; i < NUM_THREADS * 3; i++) {
//                void *joinstack;
//                thread_join(&joinstack);
//                for (int j = 0; j < NUM_THREADS * 3; j++) {
//                    if (joinstack == stacks[i]) {
//                        printf(1, "main: thread %d joined...\n", i);
//                        break;
//                    }
//                }
//            }
//        }
//        time = uptime() - time;
//        printf(1, "for scale %d\naverage run time %dms\n", *size, 10 * time/runtimes);
//        *size += 50;
//        NUM_THREADS *= 2;
//    }
    printf(1, "for scale %d\n", *size);
    while(NUM_THREADS <= 8){
        void *stacks[NUM_THREADS * 3];
//      int *args[NUM_THREADS * 3];
        for (int i = 0; i < NUM_THREADS * 3; i++) {
            stacks[i] = (void*) malloc(4096);
            if (!stacks[i]) {
                printf(1, "main: could not get stack for thread %d, exiting...\n");
                exit();
            }
        }
        runtimes = 4;
        time = uptime();
        while(runtimes--){
            for (int i = 0; i < NUM_THREADS; i++){
                thread_create(io_task, size, stacks[i * 3]);
                thread_create(mem_task, size, stacks[i * 3 + 1]);
                thread_create(cpu_task, size, stacks[i * 3 + 2]);
            }

            // Wait for all children
            for (int i = 0; i < NUM_THREADS * 3; i++) {
                void *joinstack;
                thread_join(&joinstack);
                for (int j = 0; j < NUM_THREADS * 3; j++) {
                    if (joinstack == stacks[i]) {
                        printf(1, "main: thread %d joined...\n", i);
                        break;
                    }
                }
            }
        }
        time = uptime() - time;
        printf(1, "run for %d average use time %dms\n", 3 * NUM_THREADS, 10 * time / 4);
        NUM_THREADS *= 2;
    }
    for(int i = 0; i < *size; i++){
        free(matc[i]);
    }
    free(matc);
    // Exit
    exit();
}