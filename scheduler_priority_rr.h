/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority_rr.h
 * @author Reece Harris & Calvary Fisher
 * @brief This Scheduler class implements the Priority RR scheduling algorithm.
 * @version 0.1
 */

#ifndef ASSIGN3_SCHEDULER_PRIORITY_RR_H
#define ASSIGN3_SCHEDULER_PRIORITY_RR_H

#include "scheduler.h"

class SchedulerPriorityRR : public Scheduler {
private:
    vector<PCB> list_pcb;  //store PCBs from scheduler.h 
    vector<float> wait_time;  //store wait times for each PCBs (in order)
    vector<float> turnaround_time;  //store turnaround times for each PCB (in order)
    vector<PCB> copy_list;
    vector<int> org_burst_time;

    int tq;
    
public:
    /**
     * @brief Construct a new SchedulerPriority object
     */
    SchedulerPriorityRR(int time_quantum = 10);

    /**
     * @brief Destroy the SchedulerPriority object
     */
    ~SchedulerPriorityRR() override;

    /**
     * @brief This function is called once before the simulation starts.
     *        It is used to initialize the scheduler.
     *  Maybe use a linkedlist
     * @param process_list The list of processes in the simulation.
     */
    void init(std::vector<PCB>& process_list) override;

    /**
     * @brief This function is called once after the simulation ends.
     *        It is used to print out the results of the simulation.
     */
    void print_results() override;

    /**
     * @brief This function simulates the scheduling of processes in the ready queue.
     *        It stops when all processes are finished.
     */
    void simulate() override;

    int get_org_ind(int pcb_id);

};


#endif //ASSIGN3_SCHEDULER_PRIORITY_RR_H
