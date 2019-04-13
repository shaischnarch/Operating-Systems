#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

Process CreatePro(char* name, int pid, int index) {
	Process pro = malloc(sizeof(process_t));
	if (!pro)
		return NULL;
	pro->pid = pid;
	pro->index = index;
	pro->initial_time = time(NULL);
	strcpy(pro->name, name);
	pro->is_running=1;
	return pro;
}

Process CopyPro(Process old_pro) {
	Process temp_process = malloc(sizeof(process_t));
	if (!temp_process)
		return NULL;
	temp_process->index = old_pro->index;
	temp_process->initial_time = old_pro->initial_time;
	temp_process->is_running = old_pro->is_running;
	temp_process->pid = old_pro->pid;
	strcpy(temp_process->name, old_pro->name);
	return temp_process;
}

void DeletePro(Process pro) {
	free(pro);
}

void PrintPro(Process pro) {
	if (!pro)
		return;
	printf("[%d] %s : %d %d secs ", pro->index, pro->name, pro->pid, ((time(NULL)) - (pro->initial_time)));
	if (pro->is_running == 0)
		printf("(Stopped)");
	printf("\n");

}
