/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Reece Harris & Calvary Fisher
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_fcfs.h"

SchedulerFCFS::SchedulerFCFS() {
    //private variables do not need initialization 
}

SchedulerFCFS::~SchedulerFCFS() {
    //default constructor will work fine -> vector auto deallocates
}


void SchedulerFCFS::init(std::vector<PCB>& process_list) { 
    this->list_pcb = process_list;
}

//print times for each process followd by average times
//times: turn-around time, wait time
void SchedulerFCFS::print_results() {
    for (int i = 0; i < list_pcb.size(); i++) {
        cout << "T" << i + 1 << " turn-around time = " << turnaround_time[i]
        << ", waiting time = " << wait_time[i] << endl;
    }
    cout << "Average turn-around time = " << turnaround_time[turnaround_time.size() - 1]
    << ", Average waiting time = " << wait_time[wait_time.size() - 1];
}

//Formula: use default order
void SchedulerFCFS::simulate() {
    int n = 0;
    double calc_avg_wait = 0.0;
    double calc_avg_turn = 0;
    for (int i = 0; i < list_pcb.size(); i++) {
        int calc_wait = 0;
        int calc_turn = 0;
        

        cout << "Running Process T" << list_pcb[i].id + 1 << " for "
        << list_pcb[i].burst_time << " time units" << endl;

        for (int j = 0; j < n; j++) {
            calc_wait += list_pcb[j].burst_time;
        }
        n++;
        
        calc_turn = calc_wait + list_pcb[i].burst_time;
        this->wait_time.push_back(calc_wait);
        this->turnaround_time.push_back(calc_turn);

        
        
    }

    //calculate average times 
    for (int i = 0; i < wait_time.size(); i++) {
        calc_avg_wait +=  wait_time[i];
        calc_avg_turn += turnaround_time[i];
    }
    calc_avg_wait = calc_avg_wait/wait_time.size();
    calc_avg_turn = calc_avg_turn/turnaround_time.size();

    //final element of time lists is the average
    wait_time.push_back(calc_avg_wait);
    turnaround_time.push_back(calc_avg_turn);

}
