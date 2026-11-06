#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "zemaphore.h"

typedef struct __barrier_t {
    int n;
    int count;
    Zem_t mutex;
    Zem_t turnstile;
} barrier_t;

barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    b->n = num_threads;
    b->count = 0;
    Zem_init(&b->mutex, 1);
    Zem_init(&b->turnstile, 0);
}

void barrier(barrier_t *b) {
    Zem_wait(&b->mutex);
    b->count++;
    if (b->count == b->n) {
        b->count = 0;
        Zem_post(&b->mutex);
        Zem_post(&b->turnstile);
    } else {
        Zem_post(&b->mutex);
        Zem_wait(&b->turnstile);
        Zem_post(&b->turnstile);
    }
}

typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);

    int i;
    for (i = 0; i < num_threads; i++) {
        t[i].thread_id = i;
        assert(pthread_create(&p[i], NULL, child, &t[i]) == 0);
    }
    for (i = 0; i < num_threads; i++)
        assert(pthread_join(p[i], NULL) == 0);

    printf("parent: end\n");
    return 0;
}
