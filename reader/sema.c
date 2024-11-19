#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t resource_access;    // Controls access to the resource
sem_t read_count_access;  // Controls access to read count
sem_t service_queue;      // Ensures fair scheduling between readers and writers
int read_count = 0;

// Function to format and print the timestamp
void print_timestamp() {
    static int hours = 23, minutes = 59, seconds = 55;

    // Increment the time by 1 second
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours++;
            if (hours == 24) {
                hours = 0;  // Reset the clock after 23:59:59
            }
        }
    }

    // Print the formatted timestamp
    printf("[%02d:%02d:%02d] ", hours, minutes, seconds);
}

void* reader(void* reader_id) {
    int id = *((int*)reader_id);
    while (1) {
        sem_wait(&service_queue);
        sem_wait(&read_count_access);

        read_count++;
        if (read_count == 1) {
            sem_wait(&resource_access);  // First reader locks the resource
        }
        sem_post(&read_count_access);
        sem_post(&service_queue);

        print_timestamp();
        printf("Reader %d is reading...\n", id);
        sleep(rand() % 3 + 1);  // Simulate reading

        sem_wait(&read_count_access);
        read_count--;
        if (read_count == 0) {
            sem_post(&resource_access);  // Last reader unlocks the resource
        }
        sem_post(&read_count_access);

        sleep(rand() % 3 + 1);  // Simulate waiting before reading again
    }
    pthread_exit(NULL);
}

void* writer(void* writer_id) {
    int id = *((int*)writer_id);
    while (1) {
        sem_wait(&service_queue);
        sem_wait(&resource_access);  // Writer locks the resource

        print_timestamp();
        printf("Writer %d is writing...\n", id);
        sleep(rand() % 3 + 1);  // Simulate writing

        sem_post(&resource_access);
        sem_post(&service_queue);

        sleep(rand() % 3 + 1);  // Simulate waiting before writing again
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[3], writers[2];
    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};

    // Initialize semaphores
    sem_init(&resource_access, 0, 1);
    sem_init(&read_count_access, 0, 1);
    sem_init(&service_queue, 0, 1);

    // Create reader and writer threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for the threads to complete (infinite loop in this case)
    for (int i = 0; i < 3; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&resource_access);
    sem_destroy(&read_count_access);
    sem_destroy(&service_queue);

    return 0;
}

