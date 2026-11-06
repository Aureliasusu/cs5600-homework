#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "zemaphore.h"

typedef struct __ns_mutex_t {
    Zem_t lock;
    Zem_t turnstile;
    int next_ticket;
    int now_serving;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    Zem_init(&m->lock, 1);
    Zem_init(&m->turnstile, 1);   // ★ 初始 1：第一个线程直接通过
    m->next_ticket = 0;
    m->now_serving = 0;
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Zem_wait(&m->lock);
    int my_ticket = m->next_ticket++;
    Zem_post(&m->lock);

    while (1) {
        Zem_wait(&m->turnstile);
        Zem_wait(&m->lock);
        int my_turn = (m->now_serving == my_ticket);
        Zem_post(&m->lock);
        if (my_turn) return;       // 轮到我了，进临界区
        Zem_post(&m->turnstile);   // 不是我，把令牌传下去
    }
}

void ns_mutex_release(ns_mutex_t *m) {
    Zem_wait(&m->lock);
    m->now_serving++;              // ★ 只有 release 才递增 now_serving
    Zem_post(&m->lock);
    Zem_post(&m->turnstile);       // 唤醒一个等待者
}

#define NTHREADS 8
#define LOOPS    1000

ns_mutex_t m;
int shared_counter = 0;
int per_thread[NTHREADS];

void *worker(void *arg) {
    int id = (int)(long)arg;
    int my_count = 0;
    for (int i = 0; i < LOOPS; i++) {
        ns_mutex_acquire(&m);
        shared_counter++;
        my_count++;
        ns_mutex_release(&m);
    }
    per_thread[id] = my_count;
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t th[NTHREADS];
    ns_mutex_init(&m);

    printf("parent: begin\n");
    for (long i = 0; i < NTHREADS; i++)
        assert(pthread_create(&th[i], NULL, worker, (void *)i) == 0);
    for (int i = 0; i < NTHREADS; i++)
        assert(pthread_join(th[i], NULL) == 0);

    printf("shared_counter = %d (expected %d)\n",
           shared_counter, NTHREADS * LOOPS);
    for (int i = 0; i < NTHREADS; i++)
        printf("thread %d: %d iterations\n", i, per_thread[i]);
    printf("parent: end\n");
    return 0;
}
