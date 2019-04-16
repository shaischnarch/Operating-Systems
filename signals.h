#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h> /* various type definitions, like pid_t */
#include <sys/wait.h>
#include <unistd.h>  /* standard unix functions, like getpid() */ 
#include <stdio.h> /* standard I/O functions */ 
#include <stdlib.h>
#include <string.h>
#include <signal.h> /* signal name macros, and the signal() prototype */ 
#include "process.h"
#include "list.h"

void signalCtrlC(int signum);
void signalCtrlZ(int signum);
void signalCLD(int signum);

#endif

