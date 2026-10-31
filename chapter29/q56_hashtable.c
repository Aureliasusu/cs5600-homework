#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define BUCKETS 101

typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

typedef struct {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

typedef struct {
    list_t lists[BUCKETS];
    pthread_mutex_t global_lock;
} hash_t;

int mode_global = 1;   // 0 = Q5 global lock, 1 = Q6 per-bucket lock
hash_t table;
int inserts_per_thread;

void list_init(list_t *L) {
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

void list_insert(list_t *L, int key) {
    node_t *new = malloc(sizeof(node_t));
    new->key = key;
    pthread_mutex_lock(&L->lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
}

void hash_init(hash_t *H) {
    for (int i = 0; i < BUCKETS; i++)
        list_init(&H->lists[i]);
    pthread_mutex_init(&H->global_lock, NULL);
}

void hash_insert(hash_t *H, int key, int mode) {
    if (mode == 0) {
        pthread_mutex_lock(&H->global_lock);
        list_insert(&H->lists[key % BUCKETS], key);
        pthread_mutex_unlock(&H->global_lock);
    } else {
        list_insert(&H->lists[key % BUCKETS], key);
    }
}

void *worker(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < inserts_per_thread; i++)
        hash_insert(&table, (int)(id * 100000 + i), mode_global);
    return NULL;
}

double now_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    int max_threads = (argc > 1) ? atoi(argv[1]) : 8;
    inserts_per_thread = (argc > 2) ? atoi(argv[2]) : 20000;
    mode_global = (argc > 3) ? atoi(argv[3]) : 1;
    pthread_t th[64];

    printf("mode,threads,time_s\n");
    for (int t = 1; t <= max_threads; t++) {
        hash_init(&table);
        double start = now_sec();
        for (long i = 0; i < t; i++)
            pthread_create(&th[i], NULL, worker, (void *)i);
        for (int i = 0; i < t; i++)
            pthread_join(th[i], NULL);
        double end = now_sec();
        printf("%s,%d,%.4f\n",
               mode_global == 0 ? "global" : "per-bucket",
               t, end - start);
    }
    return 0;
}
