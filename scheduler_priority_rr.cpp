/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.cpp
 * @author Reece Harris & Calvary Fisher
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_priority_rr.h"
#include <unistd.h>

SchedulerPriorityRR::SchedulerPriorityRR(int time_quantum) {
    //save time_quantum into new variable 
    tq = time_quantum;
}

SchedulerPriorityRR::~SchedulerPriorityRR() {
    //default constructor will work fine -> vector auto deallocates
}

//Bubble sort by burst time -> least to greatest 

void SchedulerPriorityRR::init(std::vector<PCB>& process_list) { 
    this->list_pcb = process_list;
    for (int i = 0; i < list_pcb.size(); i++) {
        org_burst_time.push_back(list_pcb[i].burst_time);
    }
}

void SchedulerPriorityRR::print_results() {
    for (int i = 0; i < list_pcb.size(); i++) {
        cout << "T" << i + 1 << " turn-around time = " << turnaround_time[i]
        << ", waiting time = " << wait_time[i] << endl;
    }
    cout << "Average turn-around time = " << turnaround_time[turnaround_time.size() - 1]
    << ", Average waiting time = " << wait_time[wait_time.size() - 1];
}

//Formula: first find highest priority
//if # of processes with this priority is > 1, activate round robin 
//if round robin is active, run processes for time quantum time 
void SchedulerPriorityRR::simulate() {
    int time_marker = 0;
    bool round_robin;

    for (int i = 0; i < list_pcb.size(); i++) {
        wait_time.push_back(0);
        turnaround_time.push_back(0);
        
    }

    double calc_avg_wait = 0;
    double calc_avg_turn = 0;
    bool done = false;

    //RUNS ALL PROCESSES
    while (done == false) {   
        int highest_priority = 0;
        int highest_priority_index = 0;
        //if burst time for every process equals 0, all processes are done running 
        bool not_done = false;
        for (int k = 0; k < list_pcb.size(); k++) {
            if (list_pcb[k].burst_time > 0) {
                not_done = true;
            }
        }
        if (not_done == false) {
            done = true;
            break;
        }
        
        //calculate highest priority
        for (int p = 0; p < list_pcb.size(); p++) {
            if ((list_pcb[p].priority > list_pcb[highest_priority_index].priority) && (list_pcb[p].burst_time > 0)) {
                highest_priority_index = p;
            }
        }

        highest_priority = list_pcb[highest_priority_index].priority;
        //count how many processes have the highest priority
        int counter = 0;
        for (int c = 0; c < list_pcb.size(); c++) {
            if (list_pcb[c].priority == highest_priority) {
                copy_list.push_back(list_pcb[c]);
                counter++;
            }
        }
        
        //if count of highest priority == 1, run it non-preemptively (round_robin = false)
        if (counter == 1) {
            round_robin = false;
        }
        else {
            round_robin = true;
        }
        

        //while the top priority processes are getting handled 
        bool current_done = false;
         
        //RUNS SOME PROCESSES
        int queue_counter = 0;
        while (current_done == false) {            
            if (copy_list.size() < 2) {
                round_robin = false;
            }

            cout << "Running Process T" << copy_list[queue_counter].id + 1 << " for ";
            bool done_pcb = false;
            if (round_robin == false || (copy_list[queue_counter].burst_time < this->tq)) {
                cout << copy_list[queue_counter].burst_time << " time units" << endl;
                time_marker += copy_list[queue_counter].burst_time;
                copy_list[queue_counter].priority = 0;
                copy_list[queue_counter].burst_time = 0;
                //CALC TURNAROUND TIME
                turnaround_time[get_org_ind(copy_list[queue_counter].id)] = time_marker;
                done_pcb = true;
            }
            else { //PROCESS RUNNING FOR TIME QUANTUM -> NOT DONE
                cout << tq << " time units" << endl;
                copy_list[queue_counter].burst_time = copy_list[queue_counter].burst_time - tq;
                time_marker += tq;
                if (copy_list[queue_counter].burst_time == 0) {
                    done_pcb = true;
                }
            }
            
            //swap -> if PCB in current highest priority execution is done, remove it from copy_list
            bool dont_switch = false;
            list_pcb[get_org_ind(copy_list[queue_counter].id)] = copy_list[queue_counter];
            //put PCB at the end of the list -> maintain order
            if (done_pcb == true) {
                for (int i = queue_counter; i < copy_list.size() - 1; i++) {

                    swap(copy_list[i], copy_list[i + 1]);
                    
                }
                //pop final element -> done pcb 
                copy_list.pop_back();
                dont_switch = true;
                
            }

            //if the list containing highest priority PCBs is empty, we are done 
            int are_we_done = 1;
            if (copy_list.size() == 0) {
                are_we_done = 0;
                break;
            }
            for (int j = 0; j < copy_list.size(); j++) {
                if (copy_list[j].burst_time > 0) {
                    are_we_done = 0;
                }
            }
            

            if (are_we_done == 1 || round_robin == false) {
                current_done = true;
                cout << "Breaking loop" << endl;
            }
            if (dont_switch == false) {
                queue_counter = (queue_counter + 1) % copy_list.size();

            }
        }
        //clear copied list to ensure vector is deallocated 
        copy_list.clear();
        
    }
          
    //calculate wait times
    for (int i = 0; i < list_pcb.size(); i++) {
        wait_time[i] = turnaround_time[i] - org_burst_time[i];
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

//function to get original index of pcb (in list_pcb) by pcb's id
int SchedulerPriorityRR::get_org_ind(int pcb_id) {
    int result = -1;
    for (int i = 0; i < list_pcb.size(); i++) {
        if (list_pcb[i].id == pcb_id) {
            result = i;
        }
    }
    return result;
}