//
// Created by 王珅祎 on 2019/9/4.
//

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int SEMAPHORE_NUM = 5;//使用sem_table的第几个信号量
int NUM_THREADS = 5;

void *test(void *arg){
    int i;
    int counter;

    printf(1, "thread %d: started...\n", *(int*)arg);

    for(i=0; i<TARGET_COUNT_PER_THREAD; i++){
        sem_wait(SEMAPHORE_NUM, 1);

        counter = g_counter;
        counter++;
        g_counter = counter;

        sem_signal(SEMAPHORE_NUM, 1);
    }

    exit();
}

int main(int argc, char **argv)
{
    int sem_size = 1;

    if (argc > 1){
        SEMAPHORE_NUM = NUM_THREADS = atoi(argv[1]);
    }
    final_target = NUM_THREADS * TARGET_COUNT_PER_THREAD;

    // Initialize all semaphore to 1
    for(int i = 0; i < SEMAPHORE_NUM; i++){
        if (sem_init(i, sem_size) < 0)
        {
            printf(1, "main: error initializing semaphore %d\n", SEMAPHORE_NUM);
            exit();
        }
        printf(1, "main: initialized semaphore %d to %d\n", SEMAPHORE_NUM, sem_size);
    }

    // Set up thread stuff

    // Stacks
    void *stacks[NUM_THREADS];

    // Args
    int *args[NUM_THREADS];

    // Allocate stacks and args and make sure we have them all
    // Bail if something fails
    for (i=0; i<NUM_THREADS; i++) {
        stacks[i] = (void*) malloc(4096);
        if (!stacks[i]) {
            printf(1, "main: could not get stack for thread %d, exiting...\n");
            exit();
        }

        args[i] = (int*) malloc(4);
        if (!args[i]) {
            printf(1, "main: could not get memory (for arg) for thread %d, exiting...\n");
            exit();
        }

        *args[i] = i;
    }

    printf(1, "main: running with %d threads...\n", NUM_THREADS);

    // Start all children
    for (i=0; i<NUM_THREADS; i++) {
        int pid = thread_create(test, args[i], stacks[i]);
        printf(1, "main: created thread with pid %d\n", pid);
    }

    // Wait for all children
    for (i=0; i<NUM_THREADS; i++) {
        void *joinstack;
        thread_join(&joinstack);
        for (j=0; j<NUM_THREADS; j++) {
            if (joinstack == stacks[i]) {
                printf(1, "main: thread %d joined...\n", i);
                break;
            }
        }

    }

    // Check the result
    final_counter = g_counter;
    printf(1, "Final counter is %d, target is %d\n", final_counter, final_target);
    if (final_counter == final_target)
        printf(1, "TEST PASSED!\n");
    else
        printf(1, "TEST FAILED!\n");

    // Clean up semaphore
    sem_destroy(SEMAPHORE_NUM);

    // Exit
    exit();
}