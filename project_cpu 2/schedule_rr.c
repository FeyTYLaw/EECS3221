// //Round-robin (rr) scheduling, where each task is run for a time quantum (or for the remainder of its CPU burst).
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *head = NULL;
//struct node *tail = NULL;
int process = 0;

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
        process = process + 1; 
    }else{ //if the list is not empty, find the current node and insert to next to current node
        struct node *current = head;
        while (current -> next != NULL){
            
            current = current -> next;
        }
        insert(&(current -> next), newTask);
        process = process + 1; 
    }
}

//scheduler
void schedule() {
int arrivalTime = 0;
int burstLeft = 0 ;
int totaBurstLeft = 0 ;
int totalBurst =0;
float turnaroundTime = 0;
float waitTime = 0;
float responseTime = 0;
float completionTime = 0;
int taskFlag = 0;
float totalResponseTIme = 0;
float totalBursttest = 0;
float finalburst = 0;
struct node* currentNode = head ; 
struct node * tail = head ; 

//burst time calculating 
while(currentNode->next != NULL){
    if (currentNode -> task ->burst <= QUANTUM){
        totalBursttest = totalBursttest + currentNode-> task->burst;
    }else{
        totalBursttest = totalBursttest + QUANTUM;
    }
        finalburst = finalburst + totalBursttest;
	    currentNode = currentNode->next ;
}
    tail = currentNode; 
    currentNode = head ;
//-----//
while (currentNode != NULL) {
        if (currentNode->task->burst > 0) { //check burts size 
            if (currentNode ->next == NULL) { // check next
            //-- run condition 
                if (currentNode->task->burst <= QUANTUM) {
                    run(currentNode->task, currentNode->task->burst);
                    // burst / turnaround time calculating
                    totalBurst = totalBurst + currentNode->task->burst;
                    completionTime = completionTime + totalBurst;
                    turnaroundTime = completionTime - arrivalTime;
                    //response time
                    if (taskFlag == 0) {
                        responseTime = responseTime + currentNode->task->burst ;
                        totalResponseTIme = totalResponseTIme + responseTime;}
                        currentNode->task->burst = 0;    
                } else {// if Q > b
                    // burst / turnaround time calculating
                    run(currentNode->task, QUANTUM);
                    totalBurst = totalBurst + QUANTUM;
                    burstLeft = currentNode->task->burst - QUANTUM;
                    currentNode->task->burst = burstLeft;
                    if (currentNode->task->burst > QUANTUM && taskFlag == 0){
                        responseTime = responseTime + QUANTUM;
                        totalResponseTIme = totalResponseTIme + responseTime;
                    }
                }
                currentNode = head;//set currentnode back to head and run until burst = 0;
            } else { //if next != null
                if (currentNode->task->burst <= QUANTUM) {
                   run(currentNode->task, currentNode->task->burst);
                    totalBurst = totalBurst + currentNode->task->burst;
                    completionTime = completionTime + totalBurst;
                    turnaroundTime = completionTime - arrivalTime;
                    if (taskFlag == 0) {
                        responseTime = responseTime + currentNode->task->burst ;
                        totalResponseTIme = totalResponseTIme + responseTime;}
                        currentNode->task->burst = 0;
                    
                } else {
                    run(currentNode->task, QUANTUM);
                    totalBurst = totalBurst + QUANTUM; 
                    burstLeft = currentNode->task->burst - QUANTUM;                  
                    currentNode->task->burst = burstLeft;
                    totaBurstLeft = totaBurstLeft + burstLeft;
                     if (currentNode->task->burst > QUANTUM && taskFlag == 0){
                        responseTime = responseTime + QUANTUM;
                        totalResponseTIme = totalResponseTIme + responseTime;
                    }
                }
                currentNode = currentNode->next;
            }
           
        } else { //afterfist round is doen set
            taskFlag = 1;
            currentNode = currentNode->next;
        }
    }
    waitTime = turnaroundTime - totalBurst;
    currentNode = head;
    free(currentNode->task);
    free(currentNode);

printf("\nAverage waiting time = %.2f \n", waitTime/process);
printf("Average turnaround time = %.2f \n", turnaroundTime/process);  
printf("Average response time = %.2f \n", finalburst/process);
}

