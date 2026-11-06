#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "zemaphore.h"

Zem_t s;

void *child(void *arg) {
    printf("child\n");
    sleep(1);
    Zem_post(&s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Zem_init(&s, 0);
    assert(pthread_create(&p, NULL, child, NULL) == 0);
    Zem_wait(&s);
    printf("parent: end\n");
    return 0;
}
