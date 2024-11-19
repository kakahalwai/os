#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource_lock;      // Controls access to the resource
pthread_mutex_t read_count_lock;    // Controls access to read count
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
        pthread_mutex_lock(&read_count_lock);

        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&resource_lock);  // First reader locks the resource
        }
        pthread_mutex_unlock(&read_count_lock);

        print_timestamp();
        printf("Reader %d is reading...\n", id);
        sleep(rand() % 3 + 1);  // Simulate reading

        pthread_mutex_lock(&read_count_lock);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&resource_lock);  // Last reader unlocks the resource
        }
        pthread_mutex_unlock(&read_count_lock);

        sleep(rand() % 3 + 1);  // Simulate waiting before reading again
    }
    pthread_exit(NULL);
}

void* writer(void* writer_id) {
    int id = *((int*)writer_id);
    while (1) {
        pthread_mutex_lock(&resource_lock);  // Writers lock the resource

        print_timestamp();
        printf("Writer %d is writing...\n", id);
        sleep(rand() % 3 + 1);  // Simulate writing

        pthread_mutex_unlock(&resource_lock);

        sleep(rand() % 3 + 1);  // Simulate waiting before writing again
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[3], writers[2];
    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};

    // Initialize mutexes
    pthread_mutex_init(&resource_lock, NULL);
    pthread_mutex_init(&read_count_lock, NULL);

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

    // Destroy mutexes
    pthread_mutex_destroy(&resource_lock);
    pthread_mutex_destroy(&read_count_lock);

    return 0;
}

