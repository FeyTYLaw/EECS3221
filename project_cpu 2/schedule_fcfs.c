#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "cpu.h"
#include "list.h"
//#include "schedulers.h"


struct node *head = NULL;
//adding task
void add(char *name, int priority, int burst) {

     // Create a new Task structure
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    
    //if the list is empty, add new task to head
    if(head == NULL){
        insert(&head, newTask);
    }else{ //if the list is not empty, find the current node and insert to next to current node
        struct node *current = head;
        while (current -> next != NULL){
            
            current = current -> next;
        }
        insert(&(current -> next), newTask);
    }
    
}

//scheduler
void schedule() {
    //assume arrival time is 0 in this case
int process = 0;
int arrivalTime = 0;
float totalBurst = 0;
float turnaroundTime = 0;
float waitTime = 0;
float responseTime = 0;
float completionTime = 0;
struct node * currentTask = head;


  for (; currentTask != NULL; currentTask = currentTask->next) {
        // Increase the number of processes
        process = process + 1;
        run(currentTask->task, currentTask->task->burst);
        

        // Adding all the burst for each process
        totalBurst = totalBurst + currentTask->task->burst;

        // Completion time = turnaround time + total Burst
        completionTime = completionTime + totalBurst;

        // TurnaroundTime = Completion time - Arrival time
        turnaroundTime = completionTime - arrivalTime;

        if (currentTask->next != NULL) {
            // ResponseTime = Time started executing - Arrival time
            
            responseTime = responseTime + totalBurst - arrivalTime;
        }
        free(currentTask->task);
        free(currentTask);
    }

    // WaitingTime = TurnAroundTime - bursttime;
    waitTime = turnaroundTime - totalBurst;


printf("\nAverage waiting time = %.2f \n", waitTime/process);
printf("Average turnaround time = %.2f \n", turnaroundTime/process);  
printf("Average response time = %.2f \n", responseTime/process);
}
