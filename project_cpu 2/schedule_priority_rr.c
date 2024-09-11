#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *head = NULL;
//struct node *tail = NULL;
int process = 0;
float burstTime = 0; 

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
        burstTime += newTask->burst;

    }else{
		struct node *current = head;
		struct node *previous = NULL;
        //check condition  -> if new burst is > current burst keep traverse 
        while (current != NULL && newTask -> priority <= current -> task -> priority ){
            previous = current;
            current = current->next;
        }
        //insert at the beginning
		if (previous == NULL){
            insert(&(head), newTask);
            process = process + 1; 
            burstTime += newTask->burst;

		}else{
			//insert in the middle
			insert(&(previous->next), newTask);
            process = process + 1; 
            burstTime += newTask->burst;
		}
    }
}

//scheduler
void schedule() {
struct node* currentNode = head;
float totalBurst = 0;
float completionTime = 0;
float  turnaroundTime = 0;
float arrivalTime = 0;
float waitTime = 0;
float responseTime = 0;
float responSum = 0;
float finalBurst = 0;
int firstRoundFlag = 0;


while( currentNode!= NULL ){ //if not empty


    if(currentNode -> next == NULL){ // last task  / only task;
        run(currentNode->task, currentNode->task->burst);
        totalBurst = totalBurst + currentNode->task->burst;
        finalBurst += totalBurst;
        completionTime = completionTime + totalBurst;
        turnaroundTime = completionTime - arrivalTime;
        currentNode = currentNode ->next;
        //no need to calculate response time here 
        break; 
    }

    //only task of that priority, run for the whole burst
 	if(currentNode->task-> priority != currentNode->next->task->priority && currentNode->next != NULL  ){
            run(currentNode->task, currentNode->task->burst);
            totalBurst +=  currentNode->task->burst;
            finalBurst += totalBurst;
            completionTime = completionTime + totalBurst;
            turnaroundTime = completionTime - arrivalTime;

            ////////******Response Time 
            responseTime = responseTime + finalBurst ;
			responSum = responSum + responseTime ;

            currentNode->task->burst = 0;
            //then move ccurrent to current.next for nex priority
            currentNode = currentNode->next ;
        } 
       

    //currentNode have same priority as the next && next is not NULL
    else if(currentNode->task->priority == currentNode->next->task->priority && currentNode->next != NULL ){
        //set taskHead and taskTail to indicate the start and end of same priority
        struct node * taskHead  = currentNode ; 
        struct node * taskTail = currentNode ;
        int burstLeft = 0 ;
        int pCount = 0 ; //to count the task in same priority

         //while priority is the same , keep moving to the next and count
        while(taskHead -> next != NULL && taskHead->task->priority == taskHead->next->task->priority){
            //to count the task in same priority
            pCount ++;
            taskHead = taskHead->next ;  
        } 
        taskHead = currentNode ;  
        int taskLeft = pCount ; 
        
        //to find the tail of the tasks in that priority 
        while(pCount > 0 ){
            taskHead = taskHead->next ; 
            pCount--;
        }
        taskTail = taskHead ; //set taskTail
        taskHead = currentNode ; 
       
        while(taskHead->task->priority != taskTail->next->task->priority ){ 
        
            if(taskLeft >= 0 && taskLeft < 1){//if there still remaining task in the priority
            //firstRoundFlag = 1;
				taskHead = currentNode ; 	
				while(taskHead != taskTail->next){
				if(taskHead->task->burst>0){
                    run(taskHead->task , taskHead->task->burst) ; 
                    totalBurst = totalBurst + taskHead->task->burst;
                    finalBurst += totalBurst;
                    completionTime = completionTime + totalBurst;
                    turnaroundTime = completionTime - arrivalTime;
                    ////////******Response Time not require in here - first round done already                    
                    taskHead->task->burst = 0 ; 
					taskHead= taskHead->next;
					break ; 
					}
				else {
                    firstRoundFlag = 1;
					taskHead = taskHead->next;
				}
				}
				break;
					
            }
             //if remaining task > 0,for condition when the end task burst of the priority is 0, while the remaining tasks are not
            else {
                if(taskHead == taskTail && taskTail->task->burst == 0){
                    firstRoundFlag = 1;
                    taskHead = currentNode;
                }
            }
  ////        //if burst > 0
            if(taskHead->task->burst > 0 ){
                //firstRoundFlag = 1;
                //last 
                if(taskHead->next == NULL){
                    run(taskHead->task, taskHead->task->burst);
                    //totalBurst += hd -> task ->burst;                     
                    taskHead->task->burst =0;
                    break ;  // last task of the whole list. 
                }
                
            if(taskHead != taskTail ){
                if(taskHead->task->burst <= QUANTUM){
                    run(taskHead->task , taskHead->task ->burst) ; 
                    totalBurst = totalBurst + taskHead->task->burst;
                    finalBurst += totalBurst;
                    completionTime = completionTime + totalBurst;
                    turnaroundTime = completionTime - arrivalTime; 
                    //////******Response Time 
                    if (firstRoundFlag == 0) {
                    
                        responseTime = responseTime + taskHead->task->burst ;
			            responSum = responSum + responseTime;
                        }                       
                    taskHead->task->burst = 0;
                    taskLeft--; 

                }else if(taskHead->task->burst > QUANTUM) {
                    run(taskHead->task , QUANTUM) ;
                    totalBurst+= QUANTUM ; 
                    burstLeft = taskHead->task->burst - QUANTUM ; 
                    
                    taskHead->task->burst = burstLeft; 
                    ////////******Response Time 
                    if (firstRoundFlag == 0) {
                    
                        responseTime = responseTime + QUANTUM;
			            responSum = responSum + responseTime;
                        }                    
                    }
                    taskHead = taskHead->next ; 

                }else if(taskHead==taskTail) {//hd==tail
                    if(taskTail->next == NULL){//last task , nomore priority next
                    if(taskHead->task->burst <= QUANTUM && taskHead->task->burst > 0){
                    run(taskHead->task , taskHead->task->burst) ; 
                    totalBurst = totalBurst + taskHead->task->burst;
                    finalBurst += totalBurst;
                    completionTime = completionTime + totalBurst;
                    turnaroundTime = completionTime - arrivalTime;   
                    ////////******Response Time 
                    if (firstRoundFlag == 0) {
                
                        responseTime = responseTime + taskHead->task->burst;
			            responSum = responSum + responseTime ;}       
                                      
                        taskHead->task->burst = 0 ; 
                        taskLeft--;

                    }else if (taskHead->task->burst > QUANTUM) {
                        run(taskHead->task, QUANTUM);	
                        totalBurst += QUANTUM ; 
                        finalBurst += totalBurst;
                        burstLeft = taskHead->task->burst - QUANTUM ; 
                     ////////******Response Time 
                    if (firstRoundFlag == 0) {
                        responseTime = responseTime + totalBurst ;
			            responSum = responSum + responseTime ;
                        }                           
                        taskHead->task->burst = burstLeft ; 
                    }
                    }else if(taskHead->task->burst <= QUANTUM ){
                    run(taskHead->task , taskHead->task->burst) ; 
                    totalBurst = totalBurst + taskHead->task->burst;
                    finalBurst += totalBurst;
                    completionTime = completionTime + totalBurst;
                    turnaroundTime = completionTime - arrivalTime; 
                    ////////******Response Time 
                    if (firstRoundFlag == 0) {
                        responseTime = responseTime + taskHead->task->burst;
			            responSum = responSum + responseTime;
                        }    

                        taskHead->task->burst = 0 ; 
                        taskLeft--;

                    }else if (taskHead->task->burst > QUANTUM) {
                        run(taskHead->task, QUANTUM);	
                        totalBurst += QUANTUM ; 
                        finalBurst += totalBurst;
                        burstLeft = taskHead->task->burst - QUANTUM ; 
                        if (firstRoundFlag == 0) {
                    
                        responseTime = responseTime + QUANTUM ;
			            responSum = responSum + responseTime ;
                        //printf("\nResponsum %f\n", responSum);
                        }                            
                        taskHead->task->burst = burstLeft ; 
                    }
                    
                    taskHead = currentNode ;
                }
            }else { //if hd = tail || burst ==0
                firstRoundFlag = 1;
                taskHead = taskHead ->next ;
                     
                }
            }
                firstRoundFlag = 1;
                currentNode = taskTail->next; 
                taskHead = currentNode;
                //free(currentNode -> task);
                
        }
    }  
        
        waitTime = turnaroundTime - burstTime ;
        //free(currentNode -> task);
        free(currentNode);
        printf("\nAverage waiting time = %.2f \n", waitTime/process);
        printf("Average turnaround time = %.2f \n", turnaroundTime/process);  
        printf("Average response time = %.2f \n", responSum/process);
        
}
