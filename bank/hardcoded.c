#include <stdio.h>
#include <stdbool.h>

// Number of processes and resources
#define P 5  // Number of processes
#define R 3  // Number of resources

// Function to print the current state of the system
void printState(int available[], int maximum[P][R],
                int allocation[P][R], int need[P][R]) {
    printf("\nCurrent System State:");

    printf("\n\nAvailable Resources: ");
    for(int i = 0; i < R; i++) {
        printf("%d ", available[i]);
    }

    printf("\n\nAllocation Matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ", i);
        for(int j = 0; j < R; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("\nMaximum Matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ", i);
        for(int j = 0; j < R; j++) {
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }

    printf("\nNeed Matrix:\n");
    for(int i = 0; i < P; i++) {
        printf("P%d: ", i);
        for(int j = 0; j < R; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

// Function to check if the system is in safe state
bool isSafeState(int available[], int maximum[P][R],
                 int allocation[P][R], int need[P][R]) {
    int work[R];
    bool finish[P] = {false};
    int safeSeq[P];
    int count = 0;

    // Initialize work = available
    for(int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    // Find a process which can be allocated resources
    while(count < P) {
        bool found = false;

        for(int p = 0; p < P; p++) {
            if(!finish[p]) {
                bool canAllocate = true;

                // Check if all resources can be allocated
                for(int r = 0; r < R; r++) {
                    if(need[p][r] > work[r]) {
                        canAllocate = false;
                        break;
                    }
                }

                // If we can allocate resources to this process
                if(canAllocate) {
                    // Add the allocated resources back to work
                    for(int r = 0; r < R; r++) {
                        work[r] += allocation[p][r];
                    }

                    safeSeq[count] = p;
                    finish[p] = true;
                    count++;
                    found = true;
                }
            }
        }

        // If we couldn't find a process to allocate resources
        if(!found) {
            printf("\nSystem is NOT in safe state!\n");
            return false;
        }
    }

    // System is in safe state, print the safe sequence
    printf("\nSystem is in SAFE state.");
    printf("\nSafe sequence is: ");
    for(int i = 0; i < P; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return true;
}

int main() {
    // Available instances of resources
    int available[R] = {3, 3, 2};

    // Maximum resources that can be allocated to processes
    int maximum[P][R] = {
        {7, 5, 3},  // P0
        {3, 2, 2},  // P1
        {9, 0, 2},  // P2
        {2, 2, 2},  // P3
        {4, 3, 3}   // P4
    };

    // Resources currently allocated to processes
    int allocation[P][R] = {
        {0, 1, 0},  // P0
        {2, 0, 0},  // P1
        {3, 0, 2},  // P2
        {2, 1, 1},  // P3
        {0, 0, 2}   // P4
    };

    // Calculate need matrix
    int need[P][R];
    for(int i = 0; i < P; i++) {
        for(int j = 0; j < R; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    // Print the current state
    printState(available, maximum, allocation, need);

    // Check if system is in safe state
    isSafeState(available, maximum, allocation, need);

    // Example of resource request
    printf("\n\nExample Resource Request:");
    int process = 1;  // Process P1 requesting resources
    int request[R] = {1, 0, 2};  // Request vector

    printf("\nProcess P%d requesting resources: ", process);
    for(int i = 0; i < R; i++) {
        printf("%d ", request[i]);
    }

    // Check if request is valid
    bool valid = true;
    for(int i = 0; i < R; i++) {
        if(request[i] > need[process][i]) {
            valid = false;
            printf("\nError: Process has exceeded its maximum claim");
            break;
        }
        if(request[i] > available[i]) {
            valid = false;
            printf("\nError: Resources are not available");
            break;
        }
    }

    if(valid) {
        // Try to allocate resources
        for(int i = 0; i < R; i++) {
            available[i] -= request[i];
            allocation[process][i] += request[i];
            need[process][i] -= request[i];
        }

        // Check if system is still safe after allocation
        if(isSafeState(available, maximum, allocation, need)) {
            printf("\nResource request granted.\n");
            printState(available, maximum, allocation, need);
        } else {
            // If not safe, rollback changes
            for(int i = 0; i < R; i++) {
                available[i] += request[i];
                allocation[process][i] -= request[i];
                need[process][i] += request[i];
            }
            printf("\nResource request denied. Request would lead to unsafe state.\n");
        }
    }

    return 0;
}
