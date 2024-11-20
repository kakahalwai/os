#include <stdio.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void preemptivePriority(Process *p, int n) {
    int current_time = 0, completed = 0;
    float total_turnaround = 0, total_waiting = 0;
    int queue[MAX_PROCESSES], front = 0, rear = 0; // For process scheduling
    int visited[MAX_PROCESSES] = {0};              // Track if process is added to queue
    int executed[MAX_PROCESSES] = {0};             // Track which processes have been completed

    printf("\nGantt Chart:\n");
    printf("%d", current_time);

    // Add the first process to the queue
    queue[rear++] = 0;
    visited[0] = 1;

    while (completed < n) {
        int highest_priority_pid = -1;
        int highest_priority = 10000;

        // Find the process with the highest priority that is ready to execute
        for (int i = 0; i < n; i++) {
            if (!executed[i] && p[i].arrival_time <= current_time &&
                p[i].priority < highest_priority && p[i].remaining_time > 0) {
                highest_priority_pid = i;
                highest_priority = p[i].priority;
            }
        }

        if (highest_priority_pid == -1) {
            // If no process is ready, CPU is idle
            current_time++;
            printf(" --IDLE-- %d", current_time);
            continue;
        }

        Process *current_process = &p[highest_priority_pid];

        // Execute the process
        current_process->remaining_time--;
        current_time++;

        printf(" --P%d-- %d", current_process->pid, current_time);

        // If the process is completed, calculate its turnaround and waiting times
        if (current_process->remaining_time == 0) {
            executed[highest_priority_pid] = 1;
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
    }

    // Print process details including priority
    printf("\n\nProcess Details:\n");
    printf("Process\tArrival\tBurst\tPriority\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].completion_time, p[i].turnaround_time,
               p[i].waiting_time);
    }

    // Print averages
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", total_waiting / n);
}

int main() {
    Process processes[] = {
        {1, 0, 7, 7, 2, 0, 0, 0},  // P1: arrival_time = 0, burst_time = 7, priority = 2
        {2, 2, 4, 4, 1, 0, 0, 0},  // P2: arrival_time = 2, burst_time = 4, priority = 1
        {3, 4, 1, 1, 3, 0, 0, 0},  // P3: arrival_time = 4, burst_time = 1, priority = 3
        {4, 5, 4, 4, 4, 0, 0, 0}   // P4: arrival_time = 5, burst_time = 4, priority = 4
    };

    int n = sizeof(processes) / sizeof(processes[0]);

    preemptivePriority(processes, n);

    return 0;
}

