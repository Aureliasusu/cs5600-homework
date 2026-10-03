#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} Vector;

void init_vector(Vector *v) {
    v->size = 0;
    v->capacity = 2;
    v->data = malloc(v->capacity * sizeof(int));

    if (v->data == NULL) {
        perror("malloc");
        exit(1);
    }
}

void push_back(Vector *v, int value) {
    if (v->size == v->capacity) {
        v->capacity *= 2;

        int *new_data = realloc(
            v->data,
            v->capacity * sizeof(int)
        );

        if (new_data == NULL) {
            perror("realloc");
            free(v->data);
            exit(1);
        }

        v->data = new_data;
    }

    v->data[v->size] = value;
    v->size++;
}

void print_vector(Vector *v) {
    for (int i = 0; i < v->size; i++) {
        printf("%d ", v->data[i]);
    }

    printf("\n");
}

void free_vector(Vector *v) {
    free(v->data);
}

int main(void) {
    Vector v;

    init_vector(&v);

    for (int i = 0; i < 10; i++) {
        push_back(&v, i * 10);
    }

    print_vector(&v);

    free_vector(&v);

    return 0;
}
