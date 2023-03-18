#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

class Process {
public:
    int Burst_Time;
    int Left_bt;
    int wating_time;

    Process(int burst_Time):Burst_Time(burst_Time){
        Left_bt = burst_Time;
        wating_time = 0;
    }
};

std::vector<Process> q0;    //q0 - Round Robin (RR), Priority number 1
std::vector<Process> q1;    //q1 - Shortest Job First (SJF), Priority number 2
std::vector<Process> q2;    //q2 - Shortest Job First (SJF), Priority number 3
std::vector<Process> q3;    //q3 - First-In-First-Out (FIFO), Priority number 4..*
const int qt = 50;            //time quantum for q1
const int t = 20 * 1000;    //time quamtum for each queue
int total_waiting_time_q0 = 0;
int total_waiting_time_q1 = 0;
int total_waiting_time_q2 = 0;
int total_waiting_time_q3 = 0;

void take_input_console();
void take_input_file();
void print_queues();
void shedule_q123(const int& tq, std::vector<Process>& q, int queue);
void shedule_q0(const int& t, const int& qt, std::vector<Process>& q);
void compare(std::vector<Process>& q, int& temp_time, int queue);
void update_wating_times(std::vector<Process>& q, int time);
int calculate_total_burst_time(std::vector<Process> &q);
int calculate_total_turnaround_time(int& total_wating_time, int& total_burst_time, int& total_processes);

int main()
{
    take_input_file();
    int total_processes[4] = { q0.size(), q1.size(), q2.size(), q3.size() };
    int total_burst_times[4];
    total_burst_times[0] = calculate_total_burst_time(q0);
    total_burst_times[1] = calculate_total_burst_time(q1);
    total_burst_times[2] = calculate_total_burst_time(q2);
    total_burst_times[3] = calculate_total_burst_time(q3);
        

    print_queues();

    std::sort(q1.begin(), q1.end(), [](Process const& a, Process const& b) {return a.Burst_Time < b.Burst_Time;});
    std::sort(q2.begin(), q2.end(), [](Process const& a, Process const& b) {return a.Burst_Time < b.Burst_Time; });
    std::cout << "Sheduling Queues..." << std::endl;
    print_queues();

    while ((q0.size() + q1.size() + q2.size() + q3.size() != 0)) {
        shedule_q0(t, qt, q0);
        shedule_q123(t, q1, 1);
        shedule_q123(t, q2, 2);
        shedule_q123(t, q3, 3);
    }

    std::cout << "\nTotal wating time of q0 : " << total_waiting_time_q0;
    std::cout << "\tAverage turnaround time of q0: ";
    std::cout << calculate_total_turnaround_time(total_waiting_time_q0, total_burst_times[0], total_processes[0]) << std::endl;

    std::cout << "Total wating time of q1 : " << total_waiting_time_q1;
    std::cout << "\tAverage turnaround time of q1: ";
    std::cout << calculate_total_turnaround_time(total_waiting_time_q1, total_burst_times[1], total_processes[1]) << std::endl;

    std::cout << "Total wating time of q2 : " << total_waiting_time_q2;
    std::cout << "\tAverage turnaround time of q2: ";
    std::cout << calculate_total_turnaround_time(total_waiting_time_q2, total_burst_times[2], total_processes[2]) << std::endl;

    std::cout << "Total wating time of q3 : " << total_waiting_time_q3;
    std::cout << "\tAverage turnaround time of q3: ";
    std::cout << calculate_total_turnaround_time(total_waiting_time_q3, total_burst_times[3], total_processes[3]) << std::endl;

    print_queues();
}

//*********************************************************************************************************************************

//Compare burst time and time quantum...
void compare(std::vector<Process>& q, int &temp_time, int queue) {
    if (q[0].Left_bt > temp_time) {
        std::cout << temp_time << " out of " << q[0].Left_bt << ".\tLeft temp_time = " << 0 <<  std::endl;
        q[0].Left_bt -= temp_time;
        update_wating_times(q, temp_time);
        temp_time = 0;
        if (queue == 0) {
            Process temp = q[0];
            q.erase(q.begin());
            q.push_back(temp);
        }
    }
    else if (q[0].Left_bt == temp_time) {
        std::cout << temp_time << " out of " << q[0].Left_bt << ".\tLeft temp_time = " << 0 << std::endl;
        update_wating_times(q, temp_time);
        temp_time = 0;
        switch (queue) {
        case 0: total_waiting_time_q0 += q[0].wating_time; break;
        case 1: total_waiting_time_q1 += q[0].wating_time; break;
        case 2: total_waiting_time_q2 += q[0].wating_time; break;
        default: total_waiting_time_q3 += q[0].wating_time; break;
        }
        q.erase(q.begin());
 
    }
    else {     //That means if(q[0].Left_bt < temp_time)
        std::cout << q[0].Left_bt << " out of " << temp_time << ".\tLeft temp_time = " << temp_time - q[0].Left_bt <<  std::endl;
        temp_time -= q[0].Left_bt;
        update_wating_times(q, q[0].Left_bt);
        switch (queue) {
        case 0: total_waiting_time_q0 += q[0].wating_time; break;
        case 1: total_waiting_time_q1 += q[0].wating_time; break;
        case 2: total_waiting_time_q2 += q[0].wating_time; break;
        default: total_waiting_time_q3 += q[0].wating_time; break;
        }
        q.erase(q.begin());
    }
}


void shedule_q0(const int& t, const int& qt, std::vector<Process>& q) {
    std::cout << "\nSheduling queue 0..." << std::endl;
    int extra_time = 0;   //To calculate the extra time.
    int rounds = t / qt;
    std::cout << "rounds : " << rounds << std::endl;
    for (int i = 0; i < rounds; i++) {
        int temp_time = qt;
        if (q0.size() != 0) {
            compare(q, temp_time, 0);
            extra_time += temp_time;
        }
    }
    std::cout << "Extra time = " << extra_time << std::endl;
    if (extra_time > 0) {
        shedule_q0(extra_time, qt, q);
    }
}

void shedule_q123(const int &t, std::vector<Process> &q, int queue){
    std::cout << "\nSheduling queue " << queue << "..." << std::endl;
    int temp_time = t;
    while (q.size() != 0 && temp_time > 0) {
        switch (queue) {
        case 1: compare(q, temp_time, 1); break;
        case 2: compare(q, temp_time, 2); break;
        default: compare(q, temp_time, 3); break;
        }
    }
}

void update_wating_times(std::vector<Process> &q, int time){
    for (int i = 1; i < q.size(); i++)
        q[i].wating_time += time;
}

int calculate_total_burst_time(std::vector<Process>& q) {
    int tbt = 0;
    for (int i = 0; i < q.size(); i++)
        tbt += q[i].Burst_Time;
    return tbt;
}

int calculate_total_turnaround_time(int& total_wating_time, int& total_burst_time, int& total_processes) {
    if (total_processes == 0)
        return 0;
    else
        return (total_wating_time + total_burst_time) / total_processes;
}

void take_input_console() {
    int size;
    std::cout << "How Many Processors : ";
    std::cin >> size;

    std::cout << "Enter processors : <Burst Time> <Priority>" << std::endl;
    for (int i = 0; i < size; i++) {
        int bt, p;
        std::cin >> bt >> p;
        switch (p) {
        case 1: q0.push_back(Process(bt)); break;
        case 2: q1.push_back(Process(bt)); break;
        case 3: q2.push_back(Process(bt)); break;
        default: q3.push_back(Process(bt)); break;
        }
    }
}

void take_input_file() {
    std::fstream file;
    std::vector<int> arr;
    file.open("data.txt", std::ios::in);
    int x, tbt = 0;
    while (file >> x) {
        arr.push_back(x);
    }

    for (int i = 0; i < arr.size(); i++) {
        switch (arr[i + 1]) {
        case 1: q0.push_back(Process(arr[i])); break;
        case 2: q1.push_back(Process(arr[i])); break;
        case 3: q2.push_back(Process(arr[i])); break;
        default: q3.push_back(Process(arr[i])); break;
        }
        tbt += arr[i];
        i++;
    }
}

void print_queues() {
    std::cout << "\nq0 : ";
    for (int i = 0; i < q0.size(); i++)
        std::cout << q0[i].Burst_Time << '(' << q0[i].Left_bt << "), ";
    std::cout << std::endl << std::endl;

    std::cout << "q1 : ";
    for (int i = 0; i < q1.size(); i++)
        std::cout << q1[i].Burst_Time << '(' << q1[i].Left_bt << "), ";
    std::cout << std::endl << std::endl;

    std::cout << "q2 : ";
    for (int i = 0; i < q2.size(); i++)
        std::cout << q2[i].Burst_Time << '(' << q2[i].Left_bt << "), ";
    std::cout << std::endl << std::endl;

    std::cout << "q3 : ";
    for (int i = 0; i < q3.size(); i++)
        std::cout << q3[i].Burst_Time << '(' << q3[i].Left_bt << "), ";
    std::cout << std::endl << std::endl;
}

/*
    Turnaround time = Wating time + Burst time
*/