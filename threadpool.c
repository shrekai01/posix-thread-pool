/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"
#include "semaphore.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct task
{
    void (*function)(void *p);
    void *data;
    struct task *next;
}
task;

// the work queue
task *worktodo_head = NULL;
task *worktodo_tail = NULL;

// the workers
pthread_t workers[NUMBER_OF_THREADS];

// mutex lock
pthread_mutex_t mutex;

// unnamed counting semaphore
sem_t sem;

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task *t) 
{
    pthread_mutex_lock(&mutex);
    if(worktodo_head == NULL){
        worktodo_head = t;
        worktodo_tail = t;
    }
    else{
        worktodo_tail->next = t;
        worktodo_tail = t;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

// remove a task from the queue
task *dequeue() 
{
    pthread_mutex_lock(&mutex);
    task *t = worktodo_head;
    if(t == NULL){
        return NULL;
    }
    else{
        worktodo_head = worktodo_head->next;
        if(worktodo_head == NULL){
            worktodo_tail = NULL;
        }
    }
    pthread_mutex_unlock(&mutex);
    return t;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    while(TRUE){
        sem_wait(&sem);
        task *work = dequeue();
        if(work != NULL){
            // execute the task
            execute(work->function, work->data);
            free(work);
        }
    }
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task *new_task = malloc(sizeof(task));
    new_task->next = NULL;
    new_task->function = somefunction;
    new_task->data = p;
    if(enqueue(new_task) != 0){
        return 1;
    }
    sem_post(&sem);
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    pthread_mutex_init(&mutex, NULL);
    if(sem_init(&sem, 0, 0) != 0){
        perror("Semaphore init failed");
    }
    for(int i = 0; i < NUMBER_OF_THREADS; i++){
        pthread_create(&workers[i], NULL, worker, NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_cancel(workers[i]);
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);
}
