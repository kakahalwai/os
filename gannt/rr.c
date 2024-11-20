#include <stdio.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void roundRobin(Process *p, int n, int time_quantum) {
    int current_time = 0, completed = 0;
    float total_turnaround = 0, total_waiting = 0;
    int queue[MAX_PROCESSES], front = 0, rear = 0; // For process scheduling
    int visited[MAX_PROCESSES] = {0};              // Track if process is added to queue

    printf("\nGantt Chart:\n");
    printf("%d", current_time);

    // Add the first process to the queue
    queue[rear++] = 0;
    visited[0] = 1;

    while (completed < n) {
        int pid = queue[front++]; // Dequeue
        Process *current_process = &p[pid];

        if (current_time < current_process->arrival_time) {
            // If no process can run, CPU is idle
            current_time = current_process->arrival_time;
            printf(" --IDLE-- %d", current_time);
        }

        int execution_time = (current_process->remaining_time > time_quantum)
                                 ? time_quantum
                                 : current_process->remaining_time;

        // Execute the process
        current_time += execution_time;
        current_process->remaining_time -= execution_time;

        printf(" --P%d-- %d", current_process->pid, current_time);

        // Check if the process is completed
        if (current_process->remaining_time == 0) {
            completed++;
            current_process->completion_time = current_time;
            current_process->turnaround_time =
                current_process->completion_time - current_process->arrival_time;
            current_process->waiting_time =
                current_process->turnaround_time - current_process->burst_time;

            total_turnaround += current_process->turnaround_time;
            total_waiting += current_process->waiting_time;
        }

        // Add newly arrived processes to the queue
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival_time <= current_time &&
                p[i].remaining_time > 0) {
                queue[rear++] = i; // Enqueue
                visited[i] = 1;
            }
        }

        // If the process is not completed, add it back to the queue
        if (current_process->remaining_time > 0) {
            queue[rear++] = pid; // Enqueue
        }
    }

    // Print process details
    printf("\n\nProcess Details:\n");
    printf("Process\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time,
               p[i].waiting_time);
    }

    // Print averages
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", total_waiting / n);
}

int main() {
    Process processes[] = {
        {1, 0, 7, 7, 0, 0, 0},
        {2, 2, 4, 4, 0, 0, 0},
        {3, 4, 1, 1, 0, 0, 0},
        {4, 5, 4, 4, 0, 0, 0}
    };

    int n = sizeof(processes) / sizeof(processes[0]);
    int time_quantum = 2;

    roundRobin(processes, n, time_quantum);

    return 0;
}

