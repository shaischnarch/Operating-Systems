//		commands.c
// random comment blabla	
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************


char jobshistory[50][MAX_LINE_SIZE];//history array containing old commands
int starthistory=0;//starting lockation of history array
int numhistory=0;//number of elements in history array
char lwd[MAX_LINE_SIZE];// last working directory
bool first_wd = TRUE; // first working directory

extern List jobs;//list of all jobs that are in the background (running or not running)
extern Process fg_job = NULL;//current job running in the foreground
extern int program_done;


static Process FindPro(int jobnum)
{
	LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
	{
		if(temp_pro->index == jobnum)
			return temp_pro;
	}
	return NULL;
}


void addhistory(char* cmdString)
{
	if(numhistory <= 50)
		numhistory++;
	starthistory++;
	if(starthistory >= 50)
		starthistory = 0;
	strcpy(jobshistory[starthistory], cmdString);
}

int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	Process LastPro = NULL;//last process inside jobs list;
	Process TargetPro = NULL;
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		if(num_arg != 1)
			illegal_cmd = TRUE;
		else
		{
			getcwd(pwd, sizeof(pwd));
			if(!strcmp(args[1], "-"))
			{
				if(first_wd == FALSE)
				{
					if(chdir(lwd) != 0)
						printf("smash error: > %s - path not found\n",lwd);
					else
					{
						printf("%s \n", lwd);
						strcpy(lwd,pwd);
						first_wd = FALSE;
					}
				}
			}
			else
			{
				if(chdir(args[1]) != 0)
					printf("smash error: > %s - path not found\n",args[1]);
				else
				{
					printf("%s \n", args[1]);
					strcpy(lwd,pwd);
					first_wd = FALSE;
				}
			}
		}
			
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if(num_arg != 0)
			illegal_cmd = TRUE;
		else
		{
			getcwd(pwd, sizeof(pwd));
			printf("%s \n", pwd);
		}
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
 		if(num_arg != 0)
			illegal_cmd = TRUE;
		else
		{
			int temp =0;
			for(int i=0;i<numhistory;i++)
			{
				if(starthistory-i < 0)
					temp = 1;
				printf("%s \n", jobshistory[(starthistory-i)+(50*temp)]);
			}
		}
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
 		if(num_arg != 2)
			illegal_cmd = TRUE;
		else
		{
			Process job = FindPro(atoi(args[2]));
			if(job != NULL)
			{
				int signal = -1*atoi(args[1]);
				if(kill(job->pid,signal) == -1)
					printf("smash error: > kill %s – cannot send signal\n",args[2]);
				else
				{
					if(signal == SIGTSTP || signal == SIGINT)
						job->is_running = 0;
					if(signal == SIGCONT)
						job->is_running = 1;
				}
			}
			else
				printf("smash error: > kill %s – job does not exist\n", args[2]);
		}
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		if(num_arg != 0)
			illegal_cmd = TRUE;
		else
			LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
				PrintPro(temp_pro);//prints each process;
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if (num_arg != 0)
			illegal_cmd = TRUE;
		else
			printf("smash pid is %d\n", getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		if(num_arg == 0)
		{
			LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
				LastPro = temp_pro;
			if(LastPro != NULL) //if there is a job in the list;
			{
				if(LastPro->is_running == 0) // if the last job isnt running make it run;
				{
					kill(LastPro->pid,SIGCONT);
					LastPro->is_running = 1;
				}
				
				fg_job = CopyPro(LastPro); //make a copy of last process and place it in the foreground;
				printf("%s\n", LastPro->name);
				LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs and removing the last process;
				{
					if(temp_pro->pid == LastPro->pid)
						listRemoveCurrent(jobs);
				}
				waitpid(LastPro->pid, NULL, WUNTRACED); // wait until the process in the foreground is finished (or it is stopped (WUNTRACED));
			}
			else
				printf("fg: current: no such job\n");
		}
		else if(num_arg == 1)
		{
			LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
				if(temp_pro->index == atoi(args[1]))
				{
					TargetPro = temp_pro;
					break;
				}
			if(TargetPro != NULL)
			{
				if(TargetPro->is_running == 0) // if the target job isnt running make it run;
				{
					kill(TargetPro->pid,SIGCONT);
					TargetPro->is_running = 1;
				}
				fg_job = CopyPro(TargetPro); //make a copy of targer process and place it in the foreground;
				printf("%s\n", TargetPro->name);
				LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs and removing the target process;
				{
					if(temp_pro->pid == TargetPro->pid)
						listRemoveCurrent(jobs);
				}
				LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs and subtracting the index of process with bigger indexes than target;
				{
					if((temp_pro->index) > (TargetPro->index))
						(temp_pro->index)--;
				}
				
				waitpid(TargetPro->pid, NULL, WUNTRACED); // wait until the process in the foreground is finished (or it is stopped (WUNTRACED));
			}
			else
				printf("fg: %d: no such job\n", atoi(args[1]));
		}
		else
			illegal_cmd = TRUE;
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		if(num_arg == 0)
		{
			Process LastSusPro = NULL;//last suspanded process inside jobs list;
			LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
				if(temp_pro && temp_pro->is_running == 0)
					LastSusPro = temp_pro;
			if(LastSusPro != NULL) //if there is a suspended job in the list;
			{
				kill(LastSusPro->pid,SIGCONT);
				LastSusPro->is_running = 1;
				printf("%s\n", LastSusPro->name);
			}
			else
				printf("bg: current: no such job\n");
		}
		else if(num_arg == 1)
		{
			Process TargetSusPro = NULL;
			LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
				if(temp_pro->index == atoi(args[1]))
				{
					TargetSusPro = temp_pro;
					break;
				}
			if(TargetSusPro != NULL)//if the target suspended process exists
			{
				if(TargetSusPro->is_running == 0) // if the target suspended job isnt running make it run;
				{
					kill(TargetSusPro->pid,SIGCONT);
					TargetSusPro->is_running = 1;
				}
				printf("%s\n", TargetSusPro->name);
			}
			else
				printf("bg: %d: no such job\n", atoi(args[1]));
		}
		else
			illegal_cmd = TRUE;
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		if(num_arg == 0)
		{
			listDestroy(jobs);
			if(fg_job != NULL)
				DeletePro(fg_job);
			program_done = 1;
			return 0;
		}
		else if(num_arg == 1)
		{
			if (strcmp(args[1], "kill")) 
				illegal_cmd = TRUE;
			else
			{
				LIST_FOREACH(Process,temp_pro,jobs)//for loop running on all the process in jobs;
				{
					printf("[%d] %s - sending SIGTERM... ", temp_pro->index,temp_pro->name);
					kill(temp_pro->pid, SIGTERM);
					sleep(5);
					if(waitpid(temp_pro->pid, NULL, WNOHANG) == 0)
					{
						printf("(5 sec passed) Sending SIGKILL... Done.\n");
						kill(temp_pro->pid, SIGKILL);
					}
					else
						printf("Done.\n");
				}
				listDestroy(jobs);
				if(fg_job != NULL)
					DeletePro(fg_job);
				program_done = 1;
				return 0;
			}
		}
		else
			illegal_cmd = TRUE;
	} 
	/*************************************************/
	else if (!strcmp(cmd, "cp"))
	{
   		if(num_arg != 2)
			illegal_cmd = TRUE;
		else
		{
			int    c;//used for copying files
			FILE    *stream_R;
			FILE	*stream_W;
			
			stream_R = fopen(args[1], "r");//open source file
			if (stream_R == NULL)//error opening source
				printf("cp: %s: no such file\n", args[1]);
			else
			{
				stream_W = fopen(args[2], "w");   //create and write to file
				if (stream_W == NULL)//error opening target
				{
					fclose (stream_R);
					printf("cp: %s: error making file\n", args[2]);
				}
				else
				{
					while ((c = fgetc(stream_R)) != EOF)//copies source into target
						fputc (c, stream_W);
					fclose (stream_R);
					fclose (stream_W);
					printf("cp: %s has been copied to %s\n", args[1], args[2]);
				}
			}
		}
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, 0, 0);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString, int comp_flag, int bg_flag) // for flags, 1 is true and 0 is false
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					perror("failed to fork\n");
					exit(-1);
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					if(comp_flag == 1)
						execl("/bin/bash", "/bin/bash", "-c", "-f", cmdString, (char*)NULL);//make bash run the complicated command
					else
					{
						execv(args[0], args);//execute the program
						execvp(args[0], args);//not sure witch one works, so if first one does it wont get here, if it doesent this will try to run
					}
					perror("failed to execute\n");//if we got here, program or command failed
					exit(-1);
			
			default:
                	// Add your code here
					
					if (bg_flag == 1) //if we need to run in the background;
					{
						int index = listGetSize(jobs);//finds the number of process in the list
						Process temp_bg = CreatePro(cmdString, pID, index); //creates temp process

						listInsertLast(jobs, temp_bg);
						DeletePro(temp_bg);
					} 
					else 
					{
						fg_job = CreatePro(cmdString, pID, -1);
						waitpid(fg_job->pid, NULL, WUNTRACED);// wait until the process in the foreground is finished (or it is stopped (WUNTRACED));

					}
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	int bg_flag = 0;
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[1];//for this case we only need one arg
	
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		if (lineSize[strlen(lineSize)-2] == '&')
		{
			lineSize[strlen(lineSize)-2] = '\0';
			bg_flag = 1;
		}
		strcpy(ExtCmd, lineSize);// create a copy of the command without the & sign and send it to get excuted
		args[0] = ExtCmd;
		ExeExternal(args, ExtCmd, 1, bg_flag);//complicated comand
		return 0;
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
		char cmdString[MAX_LINE_SIZE];
		strcpy(cmdString, lineSize);
		int i = 0, num_arg = 0;
		Command = strtok(lineSize, delimiters);
		if (Command == NULL)
			return 0; 
		args[0] = Command;
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
				num_arg++; 
		}			
		ExeExternal(args, cmdString, 0, 1);//execute simple command (comp_flag = 0) but in the background (bg_flag = 1);	
		return 0;
	}
	return -1;
}

