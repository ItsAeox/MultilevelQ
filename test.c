#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define MAX_QUEUE_SIZE 100
#define TIME_QUANTUM 20

struct Process {
    int id;
    int priority;
    int burst_time;
    int remaining_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
};

struct Queue {
    int front;
    int rear;
    int size;
    struct Process processes[MAX_QUEUE_SIZE];
};

struct Queue q0, q1, q2, q3;
int num_processes;

void init_queue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
    q->size = 0;
}

void enqueue(struct Queue *q, struct Process process) {
    if (q->size == MAX_QUEUE_SIZE) {
        printf("Queue overflow!\n");
        exit(1);
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->processes[q->rear] = process;
    q->size++;
}

struct Process dequeue(struct Queue *q) {
    if (q->size == 0) {
        printf("Queue underflow!\n");
        exit(1);
    }
    struct Process process = q->processes[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->size--;
    return process;
}

void execute_rr(struct Queue *q) {
    int time_elapsed = 0;
    struct Process process = dequeue(q);
    int time_left = TIME_QUANTUM;
    while (process.remaining_time > 0 && time_left > 0) {
        if (process.remaining_time <= time_left) {
            time_left -= process.remaining_time;
            process.remaining_time = 0;
        } else {
            process.remaining_time -= time_left;
            time_left = 0;
        }
        printf("Executing process P%d from queue 0 for 1 second\n", process.id);
        fflush(stdout);
        sleep(1);
        if (process.remaining_time > 0) {
            enqueue(q, process);
        }
        process = dequeue(q);
    }
    if (process.remaining_time > 0) {
        enqueue(q, process);
    } else {
        process.turnaround_time = time_elapsed - process.arrival_time;
        process.waiting_time = process.turnaround_time - process.burst_time;
        printf("Process P%d completed in queue 0\n", process.id);
        fflush(stdout);
    }
}

void execute_sjf(struct Queue *q) {
    int time_elapsed = 0;
    struct Process process = dequeue(q);
    struct Process shortest = process;
    while (process.remaining_time > 0) {
        if (process.remaining_time < shortest.remaining_time) {
            shortest = process;
        }
        process = dequeue(q);
    }
    printf("Executing process P%d from queue %d for %d seconds\n", shortest.id, shortest.priority, shortest.remaining_time);
    fflush(stdout);
    sleep(shortest.remaining_time);
    shortest.turnaround_time = time_elapsed + shortest.remaining_time - shortest.arrival_time;
    shortest.waiting_time = shortest.turnaround_time - shortest.burst_time;
    printf("Process P%d completed in queue %d\n", shortest.id, shortest.priority);
    fflush(stdout);
}

void execute_fifo(struct Queue *q) {
    int time_elapsed = 0;
    struct Process process = dequeue(q);
    printf("Executing process P%d from queue%d for %d seconds\n", process.id, process.priority, process.remaining_time);
fflush(stdout);
sleep(process.remaining_time);
process.turnaround_time = time_elapsed + process.remaining_time - process.arrival_time;
process.waiting_time = process.turnaround_time - process.burst_time;
printf("Process P%d completed in queue %d\n", process.id, process.priority);
fflush(stdout);
}

int main() {
printf("Enter the number of processes: ");
scanf("%d", &num_processes);
for (int i = 0; i < num_processes; i++) {
struct Process process;
printf("Enter the priority of process P%d (0-3): ", i);
scanf("%d", &process.priority);
printf("Enter the burst time of process P%d: ", i);
scanf("%d", &process.burst_time);
process.remaining_time = process.burst_time;
process.id = i;
process.arrival_time = 0;
process.waiting_time = 0;
process.turnaround_time = 0;
switch (process.priority) {
case 0:
enqueue(&q0, process);
break;
case 1:
enqueue(&q1, process);
break;
case 2:
enqueue(&q2, process);
break;
case 3:
enqueue(&q3, process);
break;
default:
printf("Invalid priority for process P%d, must be between 0 and 3\n", i);
exit(1);
}
}
init_queue(&q0);
init_queue(&q1);
init_queue(&q2);
init_queue(&q3);
int time_elapsed = 0;
while (q0.size > 0 || q1.size > 0 || q2.size > 0 || q3.size > 0) {
if (q0.size > 0) {
execute_rr(&q0);
}
if (q1.size > 0) {
execute_sjf(&q1);
}
if (q2.size > 0) {
execute_sjf(&q2);
}
if (q3.size > 0) {
execute_fifo(&q3);
}
time_elapsed += TIME_QUANTUM;
}
float total_waiting_time = 0;
float total_turnaround_time = 0;
for (int i = 0; i < num_processes; i++) {
struct Process process;
switch (i / (num_processes / 4)) {
case 0:
process = q0.processes[i % (num_processes / 4)];
break;
case 1:
process = q1.processes[i % (num_processes / 4)];
break;
case 2:
process = q2.processes[i % (num_processes / 4)];
break;
case 3:
process = q3.processes[i % (num_processes / 4)];
break;
}
total_waiting_time += process.waiting_time;
total_turnaround_time += process.turnaround_time;
}
float avg_waiting_time = total_waiting_time / num_processes;
float avg_turnaround_time = total_turnaround_time / num_processes;
printf("Average waiting time: %f\n", avg_waiting_time);
printf("Average turnaround time: %f\n", avg_turnaround_time);
return 0;
}
