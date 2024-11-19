#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define MAX_STRING_LENGTH 10
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 2
#define ITEMS_PER_THREAD 10

// Circular buffer for storing produced strings
char buffer[BUFFER_SIZE][MAX_STRING_LENGTH];
int count = 0;

// Semaphores for synchronization
sem_t bfull, bempty;

// Function prototypes
void print_buffer_status();
void generate_string(char* str, int max_length);
void* producer(void* arg);
void* consumer(void* arg);

// Function to print buffer status
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

// Function to generate a random string within the maximum length
void generate_string(char* str, int max_length) {
    int length = rand() % max_length + 1;
    for (int i = 0; i < length; i++) {
        str[i] = 'A' + rand() % 26;
    }
    str[length] = '\0';
}

// Producer function
void* producer(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        char item[MAX_STRING_LENGTH];
        generate_string(item, MAX_STRING_LENGTH);
        
        sem_wait(&bempty);
        strcpy(buffer[count++], item);
        printf("Producer %d produced: %s\n", thread_id, item);
        print_buffer_status();
        
        sem_post(&bfull);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

// Consumer function
void* consumer(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        sem_wait(&bfull);
        char item[MAX_STRING_LENGTH];
        strcpy(item, buffer[--count]);
        
        printf("Consumer %d consumed: %s\n", thread_id, item);
        print_buffer_status();
        
        sem_post(&bempty);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_threads[NUM_PRODUCERS];
    pthread_t cons_threads[NUM_CONSUMERS];
    
    srand(time(NULL));

    // Initialize the semaphores
    sem_init(&bfull, 0, 0);
    sem_init(&bempty, 0, BUFFER_SIZE);

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&prod_threads[i], NULL, producer, thread_id);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&cons_threads[i], NULL, consumer, thread_id);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&bfull);
    sem_destroy(&bempty);

    return 0;
}
