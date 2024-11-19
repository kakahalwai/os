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

// Function to check if the system is in a safe state
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
    int available[R];
    int maximum[P][R];
    int allocation[P][R];
    int need[P][R];

    // User input for Available Resources
    printf("Enter available resources (space separated): ");
    for(int i = 0; i < R; i++) {
        scanf("%d", &available[i]);
    }

    // User input for Maximum Resources
    for(int i = 0; i < P; i++) {
        printf("Enter maximum resources for Process P%d (space separated): ", i);
        for(int j = 0; j < R; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    // User input for Allocated Resources
    for(int i = 0; i < P; i++) {
        printf("Enter allocated resources for Process P%d (space separated): ", i);
        for(int j = 0; j < R; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate the Need Matrix (Need = Maximum - Allocation)
    for(int i = 0; i < P; i++) {
        for(int j = 0; j < R; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    // Print the current system state
    printState(available, maximum, allocation, need);

    // Check if the system is in a safe state
    isSafeState(available, maximum, allocation, need);

    // Example of resource request
    printf("\n\nExample Resource Request:");
    int process;
    printf("\nEnter the process number (0-4) requesting resources: ");
    scanf("%d", &process);
    
    int request[R];
    printf("\nEnter the resource request for Process P%d (space separated): ", process);
    for(int i = 0; i < R; i++) {
        scanf("%d", &request[i]);
    }

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

