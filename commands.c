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
bool first_wd = True; // first working directory

extern List jobs_list;


static void addhistory(char* cmdString)
{
	if(numhistory <= 50)
		numhistory++;
	starthistory++;
	if(starthistory >= 50)
		starthistory = 0;
	jobshistory[starthistory] = cmdString;
}

int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
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
			if(args[1] == '-')
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
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		if(num_arg != 0)
			illegal_cmd = TRUE;
		else
			LIST_FOREACH(Process,temp_pro,jobs_list)//for loop running on all the process in jobs_list;
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
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
		addhistory(cmdString);
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	addhistory(cmdString);
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
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
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
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
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
					
		/* 
		your code
		*/
		
	}
	return -1;
}

