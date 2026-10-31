#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct __node_t {
    int key;
    struct __node_t *next;
    pthread_mutex_t lock;
} node_t;

typedef struct {
    node_t *head;
    pthread_mutex_t head_lock;
} list_t;

void list_init(list_t *L) {
    L->head = NULL;
    pthread_mutex_init(&L->head_lock, NULL);
}

void list_insert(list_t *L, int key) {
    node_t *new = malloc(sizeof(node_t));
    new->key = key;
    pthread_mutex_init(&new->lock, NULL);
    pthread_mutex_lock(&L->head_lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->head_lock);
}

int list_lookup(list_t *L, int key) {
    pthread_mutex_lock(&L->head_lock);
    node_t *curr = L->head;
    if (!curr) { pthread_mutex_unlock(&L->head_lock); return -1; }
    pthread_mutex_lock(&curr->lock);
    pthread_mutex_unlock(&L->head_lock);
    while (curr) {
        if (curr->key == key) {
            pthread_mutex_unlock(&curr->lock);
            return 0;
        }
        node_t *next = curr->next;
        if (next) pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr = next;
    }
    return -1;
}

list_t list;
int ops_per_thread;
int list_size;

void *worker(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < ops_per_thread; i++) {
        list_lookup(&list, (int)((id * 31 + i) % list_size));
    }
    return NULL;
}

double now_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    int max_threads = (argc > 1) ? atoi(argv[1]) : 8;
    ops_per_thread = (argc > 2) ? atoi(argv[2]) : 20000;
    list_size = (argc > 3) ? atoi(argv[3]) : 1000;
    pthread_t th[64];

    printf("threads,time_s\n");
    for (int t = 1; t <= max_threads; t++) {
        list_init(&list);
        for (int i = 0; i < list_size; i++) list_insert(&list, i);
        double start = now_sec();
        for (long i = 0; i < t; i++)
            pthread_create(&th[i], NULL, worker, (void *)i);
        for (int i = 0; i < t; i++)
            pthread_join(th[i], NULL);
        double end = now_sec();
        printf("%d,%.4f\n", t, end - start);
    }
    return 0;
}
