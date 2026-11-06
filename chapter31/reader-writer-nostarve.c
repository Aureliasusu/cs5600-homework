#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "zemaphore.h"

typedef struct __rwlock_t {
    Zem_t lock;        // protects readers count
    Zem_t writelock;   // one writer OR many readers
    Zem_t turnstile;   // blocks new readers when a writer is waiting
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    Zem_init(&rw->lock, 1);
    Zem_init(&rw->writelock, 1);
    Zem_init(&rw->turnstile, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    Zem_wait(&rw->turnstile);   // pass through the turnstile
    Zem_post(&rw->turnstile);
    Zem_wait(&rw->lock);
    rw->readers++;
    if (rw->readers == 1)
        Zem_wait(&rw->writelock);
    Zem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
    Zem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0)
        Zem_post(&rw->writelock);
    Zem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    Zem_wait(&rw->turnstile);   // writer holds the turnstile
    Zem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
    Zem_post(&rw->writelock);
    Zem_post(&rw->turnstile);   // reopen the gate
}

int loops;
int value = 0;
rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        rwlock_acquire_readlock(&lock);
        printf("read %d\n", value);
        sleep(1);
        rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        rwlock_acquire_writelock(&lock);
        value++;
        printf("write %d\n", value);
        sleep(1);
        rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];
    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
        assert(pthread_create(&pr[i], NULL, reader, NULL) == 0);
    for (i = 0; i < num_writers; i++)
        assert(pthread_create(&pw[i], NULL, writer, NULL) == 0);

    for (i = 0; i < num_readers; i++)
        assert(pthread_join(pr[i], NULL) == 0);
    for (i = 0; i < num_writers; i++)
        assert(pthread_join(pw[i], NULL) == 0);

    printf("end: value %d\n", value);
    return 0;
}
