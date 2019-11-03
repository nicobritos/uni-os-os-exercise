#include "includes/newSyscalls.h"

uint64_t _systemCall();

uint64_t sys_read( int fileDescriptor, void * buff, int length)
{
    return _systemCall(__SYSTEM_CALL_READ, fileDescriptor, buff, length);
}

uint64_t sys_write(int fileDescriptor, void * buff, int length)
{
    return _systemCall(__SYSTEM_CALL_WRITE, fileDescriptor, buff, length);
}

uint64_t sys_clear()
{
    return _systemCall(__SYSTEM_CALL_CLEAR);
}

uint64_t sys_draw( int x, int y, int red, int green, int blue)
{
    return _systemCall(__SYSTEM_CALL_DRAW, x, y, red, green, blue);
}

uint64_t *sys_time(){
    return (uint64_t*)_systemCall(__SYSTEM_CALL_TIME);
}

pid_t sys_getPid()
{
    return (pid_t)_systemCall(__SYSTEM_CALL_GET_PID);
}

void *sys_newProcess(char * name, int(* foo)(int argc, char** argv), int argc, char * argv[])
{
    return (void*)_systemCall(__SYSTEM_CALL_NEW_PROCESS,name,foo,argc,argv);
}

void sys_freeProcess(pid_t pid)
{
    _systemCall(__SYSTEM_CALL_FREE_PROCESS, pid);
}

void sys_free(void * address)
{
    _systemCall(__SYSTEM_CALL_FREE,address);    
}

uint64_t sys_ticks(int * ticks)
{
    return _systemCall(__SYSTEM_CALL_TICKS,ticks);
}

uint64_t sys_ticksPerSecond(int * ticks)
{
    return _systemCall(__SYSTEM_CALL_TICKS_PER_SECOND,ticks);
}

uint64_t sys_usedMem()
{
    return _systemCall(__SYSTEM_CALL_USED_MEM);
}

uint64_t sys_freeMem()
{
    return _systemCall(__SYSTEM_CALL_FREE_MEM);
}

void *sys_malloc(uint64_t size)
{
    return (void*)_systemCall(__SYSTEM_CALL_MALLOC,size);
}
