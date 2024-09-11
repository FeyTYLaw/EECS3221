//Shortest-job-first (SJF), which schedules tasks in order of the length of the tasks’ next CPU burst.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "cpu.h"
#include "list.h"
//#include "schedulers.h"

struct node *head = NULL;
struct node *next = NULL;

//adding task
void add(char *name, int priority, int burst) {
     // Create a new Task structure
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;

   //creating newnode
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = NULL;
    //if the list is empty, add new task to head
	//Case 1
    if(head == NULL){
        insert(&head, newTask);
		//head = current;
    }else{ // if head != NULL

		struct node *current = head;
		struct node *previous = NULL;
        //check condition  -> if new burst is > current burst keep traverse 
        while (current != NULL && newTask -> burst >= current -> task -> burst ){
            previous = current;
            current = current->next;
        }

        //insert at the beginning
		if (previous == NULL){
            insert(&(head), newTask);
			//newNode -> next = head;
			//head = newNode;
		}else{
			//insert in the middle
			previous->next = newNode;
			newNode->next = current;
		}
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
        //count the total # of process 
        process = process + 1;
        run(currentTask->task, currentTask->task->burst);
        //

        // Adding all the burst for each process
        totalBurst = totalBurst + currentTask->task->burst;

        // Completion time 
        completionTime = completionTime + totalBurst;

        // TurnaroundTime 
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
