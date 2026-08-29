/**
 * client program that uses thread pool.
 */

#define NUMBER_OF_WORKS 4
#define MAX_NUM 10000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

void sub(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I subtract %d from %d result = %d\n",temp->b, temp->a, temp->a - temp->b);
}

void multiply(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I multiply %d by %d result = %d\n",temp->a, temp->b, temp->a * temp->b);
}

void divide(void *param)
{
    struct data *temp;
    temp = (struct data*)param;
    if(temp->b == 0){
        printf("%d divided by 0, error", temp->a);
        return;
    }
    printf("I divided %d by %d result = %d\n",temp->a, temp->b, temp->a / temp->b);
}

void gen_work(struct data *works, int n){
    for(int i = 0; i < n; i++){
        works[i].a = rand() % MAX_NUM;
        works[i].b = rand() % MAX_NUM;
    }
}

void print_work(struct data *works, int i){
    printf("work[%d].a = %d, word[%d].b = %d.\n", i, works[i].a, i, works[i].b);
}

int main(void)
{
    srand(time(NULL));
    // create some work to do
    struct data works[NUMBER_OF_WORKS];
    gen_work(works, NUMBER_OF_WORKS);
    // initialize the thread pool
    pool_init();

    // print the original work parameters
    for(int i = 0; i < NUMBER_OF_WORKS; i++){
        print_work(works, i);
    }

    // submit the work to the queue
    for(int i = 0; i < NUMBER_OF_WORKS; i++){
        switch(i % 4){
            case 0:
                pool_submit(&add, &works[i]);
                break;
            case 1:
                pool_submit(&sub, &works[i]);
                break;
            case 2:
                pool_submit(&multiply, &works[i]);
                break;
            case 3:
                pool_submit(&divide, &works[i]);
                break;
        }
    }

    pool_shutdown();

    return 0;
}
