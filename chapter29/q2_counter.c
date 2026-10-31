#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int value;
    pthread_mutex_t lock;
} counter_t;

void counter_init(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void counter_increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

counter_t counter;
long increments_per_thread;

void *worker(void *arg) {
    for (long i = 0; i < increments_per_thread; i++)
        counter_increment(&counter);
    return NULL;
}

double now_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    int max_threads = (argc > 1) ? atoi(argv[1]) : 8;
    increments_per_thread = (argc > 2) ? atol(argv[2]) : 1000000;
    pthread_t th[64];

    printf("threads,time_s,final_count\n");
    for (int t = 1; t <= max_threads; t++) {
        counter_init(&counter);
        double start = now_sec();
        for (int i = 0; i < t; i++)
            pthread_create(&th[i], NULL, worker, NULL);
        for (int i = 0; i < t; i++)
            pthread_join(th[i], NULL);
        double end = now_sec();
        printf("%d,%.4f,%d\n", t, end - start, counter.value);
    }
    return 0;
}
