#include <stdio.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void sortProcessesByArrival(Process *p, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void fcfs(Process *p, int n) {
    sortProcessesByArrival(p, n);

    int current_time = 0;
    float total_turnaround = 0, total_waiting = 0;

    printf("\nGantt Chart:\n");
    printf("0");

    for (int i = 0; i < n; i++) {
        // Wait if the process arrives after the current time
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
        }

        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        printf("--P%d--%d", p[i].pid, p[i].completion_time);

        current_time = p[i].completion_time;

        total_turnaround += p[i].turnaround_time;
        total_waiting += p[i].waiting_time;
    }

    printf("\n\nProcess Details:\n");
    printf("Process\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", total_waiting / n);
}

int main() {
    Process processes[] = {
        {1, 0, 7, 0, 0, 0},
        {2, 1, 1, 0, 0, 0},
        {3, 2, 3, 0, 0, 0},
        {4, 3, 4, 0, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    fcfs(processes, n);

    return 0;
}

