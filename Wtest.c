//
// Created by 王珅祎 on 2019/9/5.
//

//
// Created by 王珅祎 on 2019-09-03.
//

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define TARGET_COUNT_PER_THREAD 500000

int NUM_THREADS = 1;
int sz;
int *book;

struct Arg{
    int begin;
    int end;
};

void *count(void* input)
{
    struct Arg* arg = (struct Arg*)input;

    printf(1, "tid %d: run on %d CPU\n", getpid(), CPUnum());

    for (int i = arg->begin; i < arg->end; i++) {
        book[i]++;
    }

    exit();
}

int main(int argc, char **argv)
{
    int runtimes = 4;
    if(argc > 1){
        NUM_THREADS = atoi(argv[1]);
    }else{
        NUM_THREADS = 1;
    }
    int stime, ptime;
    int sflag, pflag;
    while(NUM_THREADS <= 32){
        stime = ptime = 0;
        sz = NUM_THREADS * TARGET_COUNT_PER_THREAD;
        book = (int*)malloc(sz * sizeof(int));

        // test single thread
        memset(book, 0, sizeof book);
        for(int k = 0; k < runtimes; k++){
            sflag = uptime();
            for (int i = 0; i < NUM_THREADS * TARGET_COUNT_PER_THREAD; i++) {
                book[i]++;
            }
            sflag = uptime() - sflag;
            stime += sflag;
        }

        // test Multi Thread
        void *stacks[NUM_THREADS];
        struct Arg* args[NUM_THREADS];

        for (int i=0,c=0; i < NUM_THREADS; i++) {
            stacks[i] = (void*) malloc(4096);
            if (!stacks[i]) {
                printf(1, "main: could not get stack for thread %d, exiting...\n");
                exit();
            }

            args[i] = (struct Arg*) malloc(sizeof(struct Arg));
            if (!args[i]) {
                printf(1, "main: could not get memory (for arg) for thread %d, exiting...\n");
                exit();
            }

            args[i]->begin = c;
            c += TARGET_COUNT_PER_THREAD;
            args[i]->end = c;
        }

//    printf(1, "main: running with %d threads...\n", NUM_THREADS);

        for(int k = 0; k < runtimes; k++){
            pflag = uptime();
            // Start all children
            for (int i=0; i<NUM_THREADS; i++) {
                thread_create(count, args[i], stacks[i]);
//                int pid = thread_create(count, args[i], stacks[i]);
//                printf(1, "main: created thread with pid %d\n", pid);
            }

            // Wait for all children
            for (int i=0; i<NUM_THREADS; i++) {
                void *joinstack;
                thread_join(&joinstack);
                for (int j=0; j<NUM_THREADS; j++) {
                    if (joinstack == stacks[i]) {
                        printf(1, "main: thread %d joined...\n", i);
                        break;
                    }
                }
            }
            pflag = uptime() - pflag;
            ptime += pflag;
        }


        printf(1, "Run as %d threads\n", NUM_THREADS);
        printf(1, "单：%dms；多：%dms\n", stime/runtimes, ptime/runtimes);
//        printf(1, "single-thread run %d times use %dms, average use %dms\n", runtimes, stime, stime/4);
//        printf(1, "multip-thread run %d times use %dms, average use %dms\n", runtimes, ptime, ptime/4);
        free(book);
        NUM_THREADS *= 2;
    }


    // Exit
    exit();
}