#ifndef PROCESS_H
#define PROCESS_H

#include "stdint.h"
#include "lib.h"

#define PROC_SIZE 4096
#define MAX_PROC 20


typedef enum {RUNNING, READY, WAITING, DEAD} t_state;

typedef struct
    {
    int pid;
    int pPid;
    char * name;
    t_state state; 
    void * stackPointer;
    void * processMemoryLowerAddress;
    }t_process;

typedef struct 
    {
      //uint16_t gs;
      //uint16_t fs;
	    uint64_t r15;
	    uint64_t r14;
	    uint64_t r13;
	    uint64_t r12;
	    uint64_t r11;
	    uint64_t r10;
	    uint64_t r9;
	    uint64_t r8;
	    uint64_t rsi;
	    uint64_t rdi;
	    uint64_t rbp;
	    uint64_t rdx;
	    uint64_t rcx;
	    uint64_t rbx;
	    uint64_t rax;
	    uint64_t rip;
	    //uint16_t cs;
	    uint64_t rflags;
	    uint64_t rsp;
	    //uint16_t ss;
    } t_stack;

t_process * createProcess(char * name, void* startingPoint, int pid,int ppid, int argc, char * argv[], void * rip);
void freeProcess(t_process * process);
int getPid(t_process * process);

#endif
