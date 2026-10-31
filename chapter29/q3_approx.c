#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUMCPUS 8

typedef struct {
    int global;
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;
} counter_t;

void counter_init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    for (int i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

void counter_update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

int counter_get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}

counter_t counter;
long increments_per_thread;

typedef struct { int threadID; } thread_arg_t;

void *worker(void *arg) {
    thread_arg_t *ta = (thread_arg_t *)arg;
    for (long i = 0; i < increments_per_thread; i++)
        counter_update(&counter, ta->threadID, 1);
    return NULL;
}

double now_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    int max_threads = (argc > 1) ? atoi(argv[1]) : 8;
    increments_per_thread = (argc > 2) ? atol(argv[2]) : 300000;
    int threshold = (argc > 3) ? atoi(argv[3]) : 1024;
    pthread_t th[64];
    thread_arg_t args[64];

    printf("threads,threshold,time_s,global_count,expected\n");
    for (int t = 1; t <= max_threads; t++) {
        counter_init(&counter, threshold);
        double start = now_sec();
        for (int i = 0; i < t; i++) {
            args[i].threadID = i;
            pthread_create(&th[i], NULL, worker, &args[i]);
        }
        for (int i = 0; i < t; i++)
            pthread_join(th[i], NULL);
        double end = now_sec();
        int g = counter_get(&counter);
        printf("%d,%d,%.4f,%d,%ld\n", t, threshold, end - start, g, (long)t * increments_per_thread);
    }
    return 0;
}
