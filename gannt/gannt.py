import matplotlib.pyplot as plt
import numpy as np
from collections import deque

class Process:
    def __init__(self, pid, arrival_time, burst_time, priority=0):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.priority = priority
        self.start_time = 0
        self.completion_time = 0
        self.waiting_time = 0
        self.turnaround_time = 0

class CPUScheduler:
    def __init__(self):
        self.processes = []
        self.gantt_chart = []
        
    def add_process(self, pid, arrival_time, burst_time, priority=0):
        self.processes.append(Process(pid, arrival_time, burst_time, priority))
        
    def calculate_times(self):
        avg_waiting_time = sum(p.waiting_time for p in self.processes) / len(self.processes)
        avg_turnaround_time = sum(p.turnaround_time for p in self.processes) / len(self.processes)
        return avg_waiting_time, avg_turnaround_time
    
    def plot_gantt_chart(self, title):
        fig, ax = plt.subplots(figsize=(12, 4))
        
        for i, (pid, start, end) in enumerate(self.gantt_chart):
            ax.barh(0, end - start, left=start, height=0.3, 
                   align='center', color=f'C{pid}', alpha=0.8)
            ax.text((start + end)/2, 0, f'P{pid}', 
                   ha='center', va='center')
            
        ax.set_ylim(-0.5, 0.5)
        ax.set_xlabel('Time')
        ax.set_yticks([])
        ax.set_title(title)
        plt.grid(True)
        plt.show()

    def fcfs(self):
        self.gantt_chart = []
        processes = sorted(self.processes, key=lambda x: (x.arrival_time, x.pid))
        current_time = 0
        
        for process in processes:
            if current_time < process.arrival_time:
                current_time = process.arrival_time
                
            process.start_time = current_time
            process.completion_time = current_time + process.burst_time
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time
            
            self.gantt_chart.append((process.pid, current_time, process.completion_time))
            current_time = process.completion_time
            
        return self.calculate_times()

    def sjf_non_preemptive(self):
        self.gantt_chart = []
        remaining_processes = self.processes.copy()
        current_time = 0
        
        while remaining_processes:
            available_processes = [p for p in remaining_processes 
                                 if p.arrival_time <= current_time]
            
            if not available_processes:
                current_time += 1
                continue
                
            process = min(available_processes, key=lambda x: (x.burst_time, x.arrival_time))
            process.start_time = current_time
            process.completion_time = current_time + process.burst_time
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time
            
            self.gantt_chart.append((process.pid, current_time, process.completion_time))
            current_time = process.completion_time
            remaining_processes.remove(process)
            
        return self.calculate_times()

    def sjf_preemptive(self):
        self.gantt_chart = []
        remaining_processes = self.processes.copy()
        current_time = 0
        current_process = None
        
        for p in remaining_processes:
            p.remaining_time = p.burst_time
            
        while remaining_processes:
            available_processes = [p for p in remaining_processes 
                                 if p.arrival_time <= current_time]
            
            if not available_processes:
                current_time += 1
                continue
                
            process = min(available_processes, key=lambda x: (x.remaining_time, x.arrival_time))
            
            if current_process != process.pid:
                if current_process is not None:
                    self.gantt_chart.append((current_process, start_time, current_time))
                current_process = process.pid
                start_time = current_time
                
            process.remaining_time -= 1
            current_time += 1
            
            if process.remaining_time == 0:
                process.completion_time = current_time
                process.turnaround_time = process.completion_time - process.arrival_time
                process.waiting_time = process.turnaround_time - process.burst_time
                remaining_processes.remove(process)
                self.gantt_chart.append((current_process, start_time, current_time))
                current_process = None
                
        return self.calculate_times()

    def round_robin(self, quantum):
        self.gantt_chart = []
        remaining_processes = deque(sorted(self.processes, key=lambda x: x.arrival_time))
        current_time = 0
        
        for p in remaining_processes:
            p.remaining_time = p.burst_time
            
        while remaining_processes:
            if not remaining_processes:
                break
                
            process = remaining_processes.popleft()
            
            if process.arrival_time > current_time:
                current_time = process.arrival_time
                
            execution_time = min(quantum, process.remaining_time)
            self.gantt_chart.append((process.pid, current_time, 
                                   current_time + execution_time))
            
            current_time += execution_time
            process.remaining_time -= execution_time
            
            if process.remaining_time > 0:
                # Find the position to insert the process back
                insert_pos = 0
                for i, p in enumerate(remaining_processes):
                    if p.arrival_time > current_time:
                        insert_pos = i
                        break
                    insert_pos = i + 1
                remaining_processes.insert(insert_pos, process)
            else:
                process.completion_time = current_time
                process.turnaround_time = process.completion_time - process.arrival_time
                process.waiting_time = process.turnaround_time - process.burst_time
                
        return self.calculate_times()

    def priority_non_preemptive(self):
        self.gantt_chart = []
        remaining_processes = self.processes.copy()
        current_time = 0
        
        while remaining_processes:
            available_processes = [p for p in remaining_processes 
                                 if p.arrival_time <= current_time]
            
            if not available_processes:
                current_time += 1
                continue
                
            process = min(available_processes, key=lambda x: (x.priority, x.arrival_time))
            process.start_time = current_time
            process.completion_time = current_time + process.burst_time
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time
            
            self.gantt_chart.append((process.pid, current_time, process.completion_time))
            current_time = process.completion_time
            remaining_processes.remove(process)
            
        return self.calculate_times()

    def priority_preemptive(self):
        self.gantt_chart = []
        remaining_processes = self.processes.copy()
        current_time = 0
        current_process = None
        
        for p in remaining_processes:
            p.remaining_time = p.burst_time
            
        while remaining_processes:
            available_processes = [p for p in remaining_processes 
                                 if p.arrival_time <= current_time]
            
            if not available_processes:
                current_time += 1
                continue
                
            process = min(available_processes, key=lambda x: (x.priority, x.arrival_time))
            
            if current_process != process.pid:
                if current_process is not None:
                    self.gantt_chart.append((current_process, start_time, current_time))
                current_process = process.pid
                start_time = current_time
                
            process.remaining_time -= 1
            current_time += 1
            
            if process.remaining_time == 0:
                process.completion_time = current_time
                process.turnaround_time = process.completion_time - process.arrival_time
                process.waiting_time = process.turnaround_time - process.burst_time
                remaining_processes.remove(process)
                self.gantt_chart.append((current_process, start_time, current_time))
                current_process = None
                
        return self.calculate_times()

# Example usage and testing
if __name__ == "__main__":
    # Test data
    processes_data = [
        (1, 0, 6, 2),  # (pid, arrival_time, burst_time, priority)
        (2, 1, 4, 4),
        (3, 2, 2, 1),
        (4, 3, 3, 3)
    ]
    
    algorithms = [
        ("FCFS", lambda s: s.fcfs()),
        ("SJF (Non-preemptive)", lambda s: s.sjf_non_preemptive()),
        ("SJF (Preemptive)", lambda s: s.sjf_preemptive()),
        ("Round Robin (Q=2)", lambda s: s.round_robin(2)),
        ("Priority (Non-preemptive)", lambda s: s.priority_non_preemptive()),
        ("Priority (Preemptive)", lambda s: s.priority_preemptive())
    ]
    
    for algo_name, algo_func in algorithms:
        scheduler = CPUScheduler()
        
        # Add processes
        for pid, arrival_time, burst_time, priority in processes_data:
            scheduler.add_process(pid, arrival_time, burst_time, priority)
        
        # Run algorithm
        avg_waiting_time, avg_turnaround_time = algo_func(scheduler)
        
        # Print results
        print(f"\n{algo_name} Results:")
        print(f"Average Waiting Time: {avg_waiting_time:.2f}")
        print(f"Average Turnaround Time: {avg_turnaround_time:.2f}")
        
        # Plot Gantt chart
        scheduler.plot_gantt_chart(f"{algo_name} - Gantt Chart")
