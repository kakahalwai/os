#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 10
#define TIME_QUANTUM 2
#define MAX_TIMELINE 100

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

void displayResults(Process *p, int n, char *algo_name);
void displayGanttChart(int timeline[], int time_points[], int timeline_length, char *algo_name);
void fcfsScheduling(Process *p, int n, int timeline[], int time_points[], int *timeline_length);
void preemptiveSJF(Process *p, int n, int timeline[], int time_points[], int *timeline_length);
void nonPreemptiveSJF(Process *p, int n, int timeline[], int time_points[], int *timeline_length);
void roundRobin(Process *p, int n, int timeline[], int time_points[], int *timeline_length);
void nonPreemptivePriority(Process *p, int n, int timeline[], int time_points[], int *timeline_length);
void preemptivePriority(Process *p, int n, int timeline[], int time_points[], int *timeline_length);

void displayResults(Process *p, int n, char *algo_name) {
    float total_turnaround = 0, total_waiting = 0;
    
    printf("\n%s Scheduling Results:\n", algo_name);
    printf("Process\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse Time\n");
    
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\t%d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, 
               p[i].completion_time, p[i].turnaround_time, 
               p[i].waiting_time,
               p[i].turnaround_time - p[i].burst_time);
        
        total_turnaround += p[i].turnaround_time;
        total_waiting += p[i].waiting_time;
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", total_waiting / n);
}

void displayGanttChart(int timeline[], int time_points[], int timeline_length, char *algo_name) {
    printf("\n%s Gantt Chart:\n", algo_name);
    printf("Time: ");
    for (int i = 0; i < timeline_length; i++) {
        printf("%d ", time_points[i]);
    }
    printf("\nProc: ");
    for (int i = 0; i < timeline_length; i++) {
        printf("P%d ", timeline[i]);
    }
    printf("\n");
}

void fcfsScheduling(Process *p, int n, int timeline[], int time_points[], int *timeline_length) {
    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    *timeline_length = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        timeline[*timeline_length] = p[i].pid;
        time_points[*timeline_length] = current_time;
        (*timeline_length)++;

        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        current_time = p[i].completion_time;
    }
    
    timeline[*timeline_length] = 0;
    time_points[*timeline_length] = current_time;
    (*timeline_length)++;
    
    displayResults(p, n, "FCFS");
    displayGanttChart(timeline, time_points, *timeline_length, "FCFS");
}

void preemptiveSJF(Process *p, int n, int timeline[], int time_points[], int *timeline_length) {
    int remaining[MAX_PROCESSES];
    for (int i = 0; i < n; i++) remaining[i] = p[i].burst_time;

    int current_time = 0, completed = 0;
    *timeline_length = 0;

    while (completed != n) {
        int shortest = -1, min_burst = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && 
                remaining[i] > 0 && 
                remaining[i] < min_burst) {
                shortest = i;
                min_burst = remaining[i];
            }
        }

        if (shortest == -1) {
            current_time++;
            continue;
        }

        timeline[*timeline_length] = p[shortest].pid;
        time_points[*timeline_length] = current_time;
        (*timeline_length)++;

        remaining[shortest]--;
        current_time++;

        if (remaining[shortest] == 0) {
            completed++;
            p[shortest].completion_time = current_time;
            p[shortest].turnaround_time = p[shortest].completion_time - p[shortest].arrival_time;
            p[shortest].waiting_time = p[shortest].turnaround_time - p[shortest].burst_time;
        }
    }

    timeline[*timeline_length] = 0;
    time_points[*timeline_length] = current_time;
    (*timeline_length)++;

    displayResults(p, n, "Preemptive SJF");
    displayGanttChart(timeline, time_points, *timeline_length, "Preemptive SJF");
}

void nonPreemptiveSJF(Process *p, int n, int timeline[], int time_points[], int *timeline_length) {
    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    *timeline_length = 0;
    for (int i = 0; i < n; i++) {
        // Find the shortest job among arrived processes
        int shortest_job_index = i;
        for (int j = i + 1; j < n; j++) {
            if (p[j].arrival_time <= current_time && 
                p[j].burst_time < p[shortest_job_index].burst_time) {
                shortest_job_index = j;
            }
        }

        // Swap processes
        Process temp = p[i];
        p[i] = p[shortest_job_index];
        p[shortest_job_index] = temp;

        // Wait if needed
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        timeline[*timeline_length] = p[i].pid;
        time_points[*timeline_length] = current_time;
        (*timeline_length)++;

        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        current_time = p[i].completion_time;
    }

    timeline[*timeline_length] = 0;
    time_points[*timeline_length] = current_time;
    (*timeline_length)++;

    displayResults(p, n, "Non-Preemptive SJF");
    displayGanttChart(timeline, time_points, *timeline_length, "Non-Preemptive SJF");
}

void roundRobin(Process *p, int n, int timeline[], int time_points[], int *timeline_length) {
    int remaining[MAX_PROCESSES];
    for (int i = 0; i < n; i++) remaining[i] = p[i].burst_time;

    int current_time = 0, completed = 0;
    *timeline_length = 0;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0) {
                int execute_time = (remaining[i] < TIME_QUANTUM) ? remaining[i] : TIME_QUANTUM;
                
                timeline[*timeline_length] = p[i].pid;
                time_points[*timeline_length] = current_time;
                (*timeline_length)++;

                remaining[i] -= execute_time;
                current_time += execute_time;

                if (remaining[i] == 0) {
                    completed++;
                    p[i].completion_time = current_time;
                    p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                }
            }
        }
    }

    timeline[*timeline_length] = 0;
    time_points[*timeline_length] = current_time;
    (*timeline_length)++;

    displayResults(p, n, "Round Robin");
    displayGanttChart(timeline, time_points, *timeline_length, "Round Robin");
}

void nonPreemptivePriority(Process *p, int n, int timeline[], int time_points[], int *timeline_length) {
    int current_time = 0;
    *timeline_length = 0;
    for (int i = 0; i < n; i++) {
        int highest_priority = INT_MAX;
        int selected_process = -1;

        for (int j = i; j < n; j++) {
            if (p[j].arrival_time <= current_time && 
                p[j].priority < highest_priority) {
                highest_priority = p[j].priority;
                selected_process = j;
            }
        }

        if (selected_process == -1) {
            current_time++;
            i--;
            continue;
        }

        // Swap selected process with current iteration
        Process temp = p[i];
        p[i] = p[selected_process];
        p[selected_process] = temp;

        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        timeline[*timeline_length] = p[i].pid;
        time_points[*timeline_length] = current_time;
        (*timeline_length)++;

        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        current_time = p[i].completion_time;
    }

    timeline[*timeline_length] = 0;
    time_points[*timeline_length] = current_time;
    (*timeline_length)++;

    displayResults(p, n, "Non-Preemptive Priority");
    displayGanttChart(timeline, time_points, *timeline_length, "Non-Preemptive Priority");
}

void preemptivePriority(Process *p, int n, int timeline[], int time_points[], int *timeline_length) {
    int remaining[MAX_PROCESSES];
    for (int i = 0; i < n; i++) remaining[i] = p[i].burst_time;

    int current_time = 0, completed = 0;
    *timeline_length = 0;

    while (completed != n) {
        int highest_priority = INT_MAX;
        int selected_process = -1;
        
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && 
                remaining[i] > 0 && 
                p[i].priority < highest_priority) {
                highest_priority = p[i].priority;
                selected_process = i;
            }
        }

        if (selected_process == -1) {
            current_time++;
            continue;
        }

        timeline[*timeline_length] = p[selected_process].pid;
        time_points[*timeline_length] = current_time;
        (*timeline_length)++;

        remaining[selected_process]--;
        current_time++;

        if (remaining[selected_process] == 0) {
            completed++;
            p[selected_process].completion_time = current_time;
            p[selected_process].turnaround_time = 
                p[selected_process].completion_time - p[selected_process].arrival_time;
            p[selected_process].waiting_time = 
                p[selected_process].turnaround_time - p[selected_process].burst_time;
        }
    }

    timeline[*timeline_length] = 0;
    time_points[*timeline_length] = current_time;
    (*timeline_length)++;

    displayResults(p, n, "Preemptive Priority");
    displayGanttChart(timeline, time_points, *timeline_length, "Preemptive Priority");
}

int main() {
    // Sample processes 
    Process processes[] = {
        {1, 0, 7, 7, 3, 0, 0, 0},   // pid, arrival, burst, remaining, priority
        {2, 1, 1, 1, 1, 0, 0, 0},
        {3, 2, 3, 3, 2, 0, 0, 0},
        {4, 3, 4, 4, 4, 0, 0, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    // Timeline arrays for Gantt charts
    int fcfs_timeline[MAX_TIMELINE];
    int fcfs_time_points[MAX_TIMELINE];
    int sjf_pre_timeline[MAX_TIMELINE];
    int sjf_pre_time_points[MAX_TIMELINE];
    int sjf_np_timeline[MAX_TIMELINE];
    int sjf_np_time_points[MAX_TIMELINE];
    int rr_timeline[MAX_TIMELINE];
    int rr_time_points[MAX_TIMELINE];
    int np_priority_timeline[MAX_TIMELINE];
    int np_priority_time_points[MAX_TIMELINE];
    int pre_priority_timeline[MAX_TIMELINE];
    int pre_priority_time_points[MAX_TIMELINE];

    int timeline_length = 0;

    // Create copies for each algorithm to preserve original data
    Process fcfs_proc[MAX_PROCESSES];
    Process sjf_preemptive_proc[MAX_PROCESSES];
    Process sjf_non_preemptive_proc[MAX_PROCESSES];
    Process rr_proc[MAX_PROCESSES];
    Process np_priority_proc[MAX_PROCESSES];
    Process p_priority_proc[MAX_PROCESSES];

    // Copy original processes
    for (int i = 0; i < n; i++) {
        fcfs_proc[i] = processes[i];
        sjf_preemptive_proc[i] = processes[i];
        sjf_non_preemptive_proc[i] = processes[i];
        rr_proc[i] = processes[i];
        np_priority_proc[i] = processes[i];
        p_priority_proc[i] = processes[i];
    }

    // Run each scheduling algorithm
    fcfsScheduling(fcfs_proc, n, fcfs_timeline, fcfs_time_points, &timeline_length);
    preemptiveSJF(sjf_preemptive_proc, n, sjf_pre_timeline, sjf_pre_time_points, &timeline_length);
    nonPreemptiveSJF(sjf_non_preemptive_proc, n, sjf_np_timeline, sjf_np_time_points, &timeline_length);
    roundRobin(rr_proc, n, rr_timeline, rr_time_points, &timeline_length);
    nonPreemptivePriority(np_priority_proc, n, np_priority_timeline, np_priority_time_points, &timeline_length);
    preemptivePriority(p_priority_proc, n, pre_priority_timeline, pre_priority_time_points, &timeline_length);

    return 0;
}
