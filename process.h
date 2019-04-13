/*
*	process.h
*	created by: shai and matan
*	date: april 2019
*/

#ifndef PROCESS_H_
#define PROCESS_H_
#define MAX_LINE_SIZE 80

typedef struct process_t *Process;


struct process_t{
	char name[MAX_LINE_SIZE];
	int index;
	int pid;
	int initial_time;
	int is_running;
};


Process CreatPro(char* name,int pid, int index);
Process CopyPro(Process old_pro);
void DeletePro(Process pro);
void PrintPro(Process pro);


#endif /* PROCESS_H_ */
