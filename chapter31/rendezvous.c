#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "zemaphore.h"

Zem_t s1, s2;

void *child_1(void *arg) {
    printf("child 1: before\n");
    sleep(1);
    Zem_post(&s1);      // 告诉 child 2：我到了
    Zem_wait(&s2);      // 等 child 2 到达
    printf("child 1: after\n");
    return NULL;
}

void *child_2(void *arg) {
    printf("child 2: before\n");
    sleep(1);
    Zem_post(&s2);      // 告诉 child 1：我到了
    Zem_wait(&s1);      // 等 child 1 到达
    printf("child 2: after\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    printf("parent: begin\n");
    Zem_init(&s1, 0);
    Zem_init(&s2, 0);
    assert(pthread_create(&p1, NULL, child_1, NULL) == 0);
    assert(pthread_create(&p2, NULL, child_2, NULL) == 0);
    assert(pthread_join(p1, NULL) == 0);
    assert(pthread_join(p2, NULL) == 0);
    printf("parent: end\n");
    return 0;
}
