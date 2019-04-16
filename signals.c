// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern Process fg_job;
extern List jobs;
//extern int job_index;

void signalCtrlC(int signum) {
	
	//note for self: check if extern int fg_process_runnig is necessary
	if(!fg_job->is_running)//you cant do Ctrl-C without fg_job, maybe change to fg_job == NULL
		return;
	if (kill(fg_job->pid,SIGINT) == -1){
		printf("\nCan't stop job with PID=%d\n", fg_job->pid);
	}
	else {
		DeletePro(fg_job);
		printf("\nJob with PID=%d was stopped\n", fg_job->pid);
	}
}

void signalCtrlZ(int signum) {
	if(!fg_job->is_running)//you cant do Ctrl-Z without fg_job, maybe change to fg_job == NULL
		return;

	if (kill(fg_job->pid,SIGTSTP) == -1){
		printf("\nCan't suspend job with PID=%d\n", fg_job->pid);
	}
	else {

		if (listGetSize(jobs_list) == 0)
			fg_job->index==1;
		else {
			LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
			fg_job->index = temp_pro->index;
			fg_job->index++;//INDEX OF THE LAST process + 1
		}
		
		fg_job->is_running=0;

		listInsertLast(jobs, fg_job);
			DeletePro(fg_job);
			printf("\nJob with PID=%d was suspended\n", fg_job->pid);
		
	}
}


void signalCLD(int signum){
	Process pro;
	if(fg_job!=NULL) // fg_job->is_running==1{
		if(waitpid(fg_job->pid, NULL, 1))// what here instead of 1{
			free(fg_job);
		}
	}

	pro = listGetFirst(jobs);
	while(pro){

		for( ; pro != NULL ; pro = listGetNext(jobs)){
			if(waitpid(pro->pid, NULL, 1)){
				listRemoveCurrent(jobs_list);
				pro = listGetFirst(jobs_list);
				break;
			}
		}
	}
}


