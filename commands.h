#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>  /* standard unix functions, like getpid() */ 
#include <stdio.h> /* standard I/O functions */ 
#include <time.h>
#include <stdlib.h>
#include <signal.h> /* signal name macros, and the signal() prototype */ 
#include <string.h>
#include <sys/types.h> /* various type definitions, like pid_t */
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
typedef enum { FALSE , TRUE } bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(void* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif

