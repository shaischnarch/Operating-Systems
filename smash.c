/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h> /* standard I/O functions */ 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20


//List history_list; change here decleration
List jobs;
Process fg_job;
//int program_done;

char* L_Fg_Cmd;
List jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	 
	 	struct sigaction SigactionZ;
    	sigset_t MaskZ;
    	sigfillset(&MaskZ);
    	SigactionZ.sa_flags = SA_RESTART;
    	SigactionZ.sa_mask = MaskZ;
    	SigactionZ.sa_handler = signalCtrlZ;	// ctrl+z
        sigaction(SIGTSTP, &SigactionZ, NULL);

    	struct sigaction SigactionC;
    	sigset_t MaskC;
    	sigfillset(&MaskC);
    	SigactionC.sa_flags = SA_RESTART;
    	SigactionC.sa_mask = MaskC;
    	SigactionC.sa_handler = signalCtrlC;	// ctrl+c
        sigaction(SIGINT, &SigactionC, NULL);

    	struct sigaction SigactionCLD;
    	sigset_t MaskCLD;
    	sigfillset(&MaskCLD);
    	SigactionCLD.sa_flags = SA_RESTART;
    	SigactionCLD.sa_mask = MaskCLD;
    	SigactionCLD.sa_handler = signalCLD;	// child
        sigaction(SIGCHLD, &SigactionCLD, NULL);
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 
	
	
  //change!!  history_list=listCreate(string_copy,string_free);
    jobs   =listCreate(processCopy, processDelete);
    fg_job = NULL;

   // program_done=0;

	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue; 
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
		
		//		if(program_done) break;

	}
    return 0;
}

