/**
* Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Reece Harris & Calvary Fisher
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_rr.h"

SchedulerRR::SchedulerRR(int time_quantum) {
    //save time quantum into new variable 
    tq = time_quantum;
}

SchedulerRR::~SchedulerRR() {
    //default constructor will work fine -> vector auto deallocates
}


void SchedulerRR::init(std::vector<PCB>& process_list) { 
    this->list_pcb = process_list;
}

//print times for each process followd by average times
//times: turn-around time, wait time
void SchedulerRR::print_results() {
    for (int i = 0; i < list_pcb.size(); i++) {
        cout << "T" << i + 1 << " turn-around time = " << turnaround_time[i]
        << ", waiting time = " << wait_time[i] << endl;
    }
    cout << "Average turn-around time = " << turnaround_time[turnaround_time.size() - 1]
    << ", Average waiting time = " << wait_time[wait_time.size() - 1];
}

void SchedulerRR::simulate() {
    int i = 0;
    int time_marker = 0;

    //save list for original burst times
    vector<PCB> copy_list = list_pcb;

    for (int i = 0; i < list_pcb.size(); i++) {
        wait_time.push_back(0);
        turnaround_time.push_back(0);
    }

    double calc_avg_wait = 0;
    double calc_avg_turn = 0;
    bool done = false;

    while (done == false) {   
        if (list_pcb[i].burst_time != 0) {

            
            cout << "Running Process T" << list_pcb[i].id + 1 << " for ";
            if (list_pcb[i].burst_time > this->tq) { //PROCESS RUNNING FOR TIME QUANTUM -> NOT DONE
                cout << tq << " time units" << endl;
                list_pcb[i].burst_time = list_pcb[i].burst_time - tq;
                time_marker += tq;
            }
            else { //PROCESS COMPLETING
                cout << list_pcb[i].burst_time << " time units" << endl;
                time_marker += list_pcb[i].burst_time;
                list_pcb[i].burst_time = 0;
                //CALC TURNAROUND TIME
                turnaround_time[i] = time_marker;
            }

            bool not_done = false;
            for (int k = 0; k < list_pcb.size(); k++) {
                if (list_pcb[k].burst_time > 0) {
                    not_done = true;
                }
            }
            if (not_done == false) {
                done = true;
            }
        }
        i = (i + 1)%list_pcb.size();   
    }

    for (int i = 0; i < list_pcb.size(); i++) {
        //CALCULATE WAIT TIME 
        wait_time[i] = turnaround_time[i] - copy_list[i].burst_time;
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

    copy_list.clear();
    
}