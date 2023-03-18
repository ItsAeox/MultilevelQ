class Process:
    def __init__(self, pid, priority, burst_time):
        self.pid = pid
        self.priority = priority
        self.burst_time = burst_time
        self.arrival_time = 0
        self.waiting_time = 0
        self.turnaround_time = 0
        self.remaining_time = burst_time

with open("processes.txt", "r") as f:
    lines = f.readlines()

processes = []
for i, line in enumerate(lines):
    pid, priority, burst_time = line.split()
    processes.append(Process(i, int(priority), int(burst_time)))

q0 = []
q1 = []
q2 = []
q3 = []

for process in processes:
    if process.priority == 0:
        q0.append(process)
    elif process.priority == 1:
        q1.append(process)
    elif process.priority == 2:
        q2.append(process)
    else:
        q3.append(process)

time_quantum = 20
round_robin = len(q0)
temp = 1

def calculate_times(processes):
    current_time = 0
    total_waiting_time = 0
    total_turnaround_time = 0
    current_queue = processes

    for process in processes:
        process.waiting_time = current_time

        if current_queue == q0:
            time_slice = min(time_quantum, process.remaining_time)
        else:
            time_slice = process.remaining_time

        current_time += time_slice
        process.remaining_time -= time_slice

        if process.remaining_time <= 0:
            process.turnaround_time = current_time
            if current_queue == q0:
                process.waiting_time = process.turnaround_time - process.burst_time
            total_waiting_time += process.waiting_time
            total_turnaround_time += process.turnaround_time
        else:
            current_queue.append(process)

    return (total_waiting_time, total_turnaround_time)

q0_waiting_time, q0_turnaround_time = calculate_times(q0)
q1_waiting_time, q1_turnaround_time = calculate_times(sorted(q1, key=lambda x: x.burst_time))
q2_waiting_time, q2_turnaround_time = calculate_times(sorted(q2, key=lambda x: x.burst_time))
q3_waiting_time, q3_turnaround_time = calculate_times(q3)

q1_turnaround_time += q0_turnaround_time
q2_turnaround_time += q1_turnaround_time
q3_turnaround_time += q2_turnaround_time

q1_waiting_time += q0_turnaround_time
q2_waiting_time += q1_turnaround_time
q3_waiting_time += q2_turnaround_time

print("Queue 0:")
for process in q0:
    print(f"Process {process.pid}: waiting time = {process.waiting_time}, turnaround time = {process.turnaround_time}")
    if temp == round_robin:
        break
    temp += 1

print("Queue 1:")
for process in sorted(q1, key=lambda x: x.burst_time):
    print(f"Process {process.pid}: waiting time = {process.waiting_time}, turnaround time = {process.turnaround_time}")

print("Queue 2:")
for process in sorted(q2, key=lambda x: x.burst_time):
    print(f"Process {process.pid}: waiting time = {process.waiting_time}, turnaround time = {process.turnaround_time}")

print("Queue 3:")
for process in q3:
    print(f"Process {process.pid}: waiting time = {process.waiting_time}, turnaround time = {process.turnaround_time}")

q0_average_waiting_time = q0_waiting_time / round_robin
q0_average_turnaround_time = q0_turnaround_time / round_robin
q1_average_waiting_time = q1_waiting_time / len(q1)
q1_average_turnaround_time = q1_turnaround_time / len(q1)
q2_average_waiting_time = q2_waiting_time / len(q2)
q2_average_turnaround_time = q2_turnaround_time / len(q2)
q3_average_waiting_time = q3_waiting_time / len(q3)
q3_average_turnaround_time = q3_turnaround_time / len(q3)

print("Average waiting times and turnaround times")
print(f"Queue 0 : average waiting time = {int(q0_average_waiting_time)}, average turnaround time = {int(q0_average_turnaround_time)}")
print(f"Queue 1 : average waiting time = {int(q1_average_waiting_time)}, average turnaround time = {int(q1_average_turnaround_time)}")
print(f"Queue 2 : average waiting time = {int(q2_average_waiting_time)}, average turnaround time = {int(q2_average_turnaround_time)}")
print(f"Queue 3 : average waiting time = {int(q3_average_waiting_time)}, average turnaround time = {int(q3_average_turnaround_time)}")

