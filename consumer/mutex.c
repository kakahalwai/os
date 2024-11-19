#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define MAX_STRING_LENGTH 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 1
#define ITEMS_PER_THREAD 10

char buffer[BUFFER_SIZE][MAX_STRING_LENGTH];
int count = 0;
pthread_mutex_t mutex;

void print_buffer_status() {
    printf("Buffer status: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < count) {
            printf("%s", buffer[i]);
        } else {
            printf("_");
        }
        if (i != BUFFER_SIZE - 1) {
            printf(", ");
        }
    }
    printf("] (count = %d)\n", count);
}

void generate_string(char* str, int max_length) {
    int length = rand() % max_length + 1;
    for (int i = 0; i < length; i++) {
        str[i] = 'A' + rand() % 26;
    }
    str[length] = '\0';
}

void* producer(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        char item[MAX_STRING_LENGTH];
        generate_string(item, MAX_STRING_LENGTH);
        
        pthread_mutex_lock(&mutex);
        if (count < BUFFER_SIZE) {
            strcpy(buffer[count++], item);
            printf("Producer %d produced: %s\n", thread_id, item);
            print_buffer_status();
        } else {
            printf("Buffer is full, Producer %d waiting...\n", thread_id);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        pthread_mutex_lock(&mutex);
        if (count > 0) {
            char item[MAX_STRING_LENGTH];
            strcpy(item, buffer[--count]);
            printf("Consumer %d consumed: %s\n", thread_id, item);
            print_buffer_status();
        } else {
            printf("Buffer is empty, Consumer %d waiting...\n", thread_id);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_threads[NUM_PRODUCERS];
    pthread_t cons_threads[NUM_CONSUMERS];
    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&prod_threads[i], NULL, producer, thread_id);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&cons_threads[i], NULL, consumer, thread_id);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
