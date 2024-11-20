#include <stdio.h>
#include <limits.h>

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

void srtf(Process *p, int n) {
    int completed = 0, current_time = 0;
    float total_turnaround = 0, total_waiting = 0;
    int shortest = -1;
    int min_remaining = INT_MAX;

    printf("\nGantt Chart:\n");
    printf("%d", current_time);

    while (completed < n) {
        // Find the process with the shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 &&
                p[i].remaining_time < min_remaining) {
                shortest = i;
                min_remaining = p[i].remaining_time;
            }
        }

        if (shortest == -1) {
            // No process is available at the current time
            current_time++;
            printf(" --IDLE-- %d", current_time);
            continue;
        }

        // Execute the shortest job for one unit of time
        p[shortest].remaining_time--;
        current_time++;
        printf(" --P%d-- %d", p[shortest].pid, current_time);

        // If the process is completed
        if (p[shortest].remaining_time == 0) {
            completed++;
            p[shortest].completion_time = current_time;
            p[shortest].turnaround_time = p[shortest].completion_time - p[shortest].arrival_time;
            p[shortest].waiting_time = p[shortest].turnaround_time - p[shortest].burst_time;

            total_turnaround += p[shortest].turnaround_time;
            total_waiting += p[shortest].waiting_time;

            // Reset the shortest and minimum remaining time for next iteration
            shortest = -1;
            min_remaining = INT_MAX;
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

    srtf(processes, n);

    return 0;
}

