#include "scheduler.h"
#include "processHandler.h"
#include "memManager.h"
#include "stdio.h"
#include "time.h"

#define QUANTUM 4

typedef struct processNodeCDT *processNodeADT;

typedef struct processNodeCDT {
    uint64_t executedOnTicks;
	t_process process;
	t_priority priority;
} processNodeCDT;

static const uint64_t quantumSlice[2] = {QUANTUM * 2, QUANTUM};

static nodeListADT currentProcessNode = NULL;
static listADT waitingQueue = NULL;
static listADT readyQueue = NULL;

static uint8_t initialized = 0;

static t_process idleProcess = NULL;

void idleFunction();

void loadNextProcess(t_stack currentProcessStack);
nodeListADT fetchNextNode();
void dispatchProcess(t_process process, t_stack currentProcessStack);
nodeListADT getNode(pid_t pid, listADT queue);
nodeListADT getNodeReadyQueue(pid_t pid);
nodeListADT getNodeWaitingQueue(pid_t pid);
void removeProcess(nodeListADT processNode, listADT queue);
void moveNode(nodeListADT processNode, listADT fromQueue, listADT toQueue);
processNodeADT getProcessNodeFromNode(nodeListADT node);
t_process getProcessFromNode(nodeListADT node);
void *duplicateProcessNode(void *_processNode);
void freeProcessNodeReadOnly(void *_processNode);
uint8_t equalsPid(void *_process, void *_pid);
void freeProcessNodeReadOnly(void *_processNode);

// Public
void initializeScheduler() {
	idleProcess = createProcess("Idle Process", idleFunction, SYSTEM_PID, SYSTEM_PID, 0, NULL, NULL);

	waitingQueue = createList();
	readyQueue = createList();

	initialized = 1;
}

void runScheduler(t_stack currentProcessStack) {
	if (!initialized || getSizeList(readyQueue) == 0) {
		return;
	}

	if (currentProcessNode != NULL) {
		processNodeADT myCurrentProcessNode = getProcessNodeFromNode(currentProcessNode);

		if (getProcessState(myCurrentProcessNode->process) == P_RUNNING && 
				ticks_elapsed() - myCurrentProcessNode->executedOnTicks < quantumSlice[myCurrentProcessNode->priority]) {
			return;
		}
	}

	loadNextProcess(currentProcessStack);
}

uint8_t addProcess(t_process process, t_priority priority) {
	processNodeADT processNode = pmalloc(sizeof(processNodeCDT), SYSTEM_PID);
	addElementToIndexList(readyQueue, processNode, getSizeList(readyQueue));

	processNode->executedOnTicks = 0;
	processNode->process = process;
	processNode->priority = priority;

	return 1;
}

void killProcess(pid_t pid) {
	nodeListADT processNode = getNodeReadyQueue(pid);
	if (processNode == NULL) {
		processNode = getNodeWaitingQueue(pid);
		if (processNode == NULL) return;

		removeProcess(processNode, waitingQueue);
		return;
	}

	if (processNode == currentProcessNode) {
		setProcessState(getProcessFromNode(processNode), P_DEAD);
	} else {
		removeProcess(processNode, readyQueue);
	}
}

t_process getCurrentProcess() {
	if (currentProcessNode != NULL) return getProcessFromNode(currentProcessNode);
	return idleProcess;
}

pid_t getCurrentProcessPid() {
	if (currentProcessNode != NULL) return getProcessPid(getProcessFromNode(currentProcessNode));
	return getProcessPid(idleProcess);
}

void lockProcess(pid_t pid) {
	nodeListADT processNode = getNodeReadyQueue(pid);
	if (processNode == NULL) return;
	setProcessState(getProcessFromNode(processNode), P_LOCKED);
	
	if (processNode != currentProcessNode) {
		moveNode(processNode, readyQueue, waitingQueue);
	}
}

void unlockProcess(pid_t pid) {
	nodeListADT processNode = getNodeWaitingQueue(pid);
	if (processNode == NULL) return;
	setProcessState(getProcessFromNode(processNode), P_READY);
	
	moveNode(processNode, waitingQueue, readyQueue);
}

t_state getCurrentProcessState() {
	if (currentProcessNode == NULL) return P_INVALID;
	return getProcessState(getProcessFromNode(currentProcessNode));
}

// Iterator
listADT createProcessList() {
	listADT newList = duplicateList(readyQueue, duplicateProcessNode);
	return duplicateAndConcatList(newList, waitingQueue, duplicateProcessNode);
}

uint8_t hasNextProcess(listADT list) {
	return hasNextListIterator(list);
}

uint64_t getProcessListLength(listADT list) {
	return getSizeList(list);
}

t_process getNextProcess(listADT list) {
	if (!hasNextProcess(list)) return NULL;
	nodeListADT processNode = getNextNodeListIterator(list);
	if (processNode != NULL) {
		return getProcessFromNode(processNode);
	}
	return NULL;
}

void freeProcessesList(listADT list) {
	freeList(list, freeProcessNodeReadOnly);
}

// Private
void loadNextProcess(t_stack currentProcessStack) {
	nodeListADT nextProcessNode = fetchNextNode();
	if (nextProcessNode == NULL) {		
		if (currentProcessNode != NULL) {
			setProcessState(getProcessFromNode(currentProcessNode), P_READY);
			updateStack(getProcessStackFrame(getProcessFromNode(currentProcessNode)), currentProcessStack);
		}

		dispatchProcess(idleProcess, currentProcessStack);
		return;
	}

	if (nextProcessNode == currentProcessNode) {
		getProcessNodeFromNode(currentProcessNode)->executedOnTicks = ticks_elapsed();
	} else {
		if (currentProcessNode != NULL) {
			setProcessState(getProcessFromNode(currentProcessNode), P_READY);
			updateStack(getProcessStackFrame(getProcessFromNode(currentProcessNode)), currentProcessStack);
		}

		currentProcessNode = nextProcessNode;
		getProcessNodeFromNode(currentProcessNode)->executedOnTicks = ticks_elapsed();
		dispatchProcess(getProcessFromNode(currentProcessNode), currentProcessStack);
	}
}

nodeListADT fetchNextNode() {
	nodeListADT currentNode = currentProcessNode;
	if (currentNode == NULL) {
		currentNode = getNodeAtIndexList(readyQueue, 0);
	} else {
		currentNode = getNextNodeList(currentProcessNode);
	}

	while (currentNode != currentProcessNode && getSizeList(readyQueue) >= 1) {
		if (currentNode == NULL) {
			currentNode = getNodeAtIndexList(readyQueue, 0);
		} else {
			if (getProcessState(getProcessFromNode(currentNode)) == P_DEAD) {
				removeProcess(currentNode, readyQueue);
			} else if (getProcessState(getProcessFromNode(currentNode)) == P_READY) {
				break;
			} else if (getProcessState(getProcessFromNode(currentNode)) == P_LOCKED) {
				moveNode(currentNode, readyQueue, waitingQueue);
			}

			currentNode = getNextNodeList(currentNode);
		}
	}
	if (currentNode == currentProcessNode) {
		return currentNode;
	}
	if (getSizeList(readyQueue) == 0 || currentNode == NULL || getProcessState(getProcessFromNode(currentNode)) != P_READY) {
		return NULL;
	}

	return currentNode;
}

void dispatchProcess(t_process process, t_stack currentProcessStack) {
	setProcessState(process, P_RUNNING);
	updateStack(currentProcessStack, getProcessStackFrame(process));
}

nodeListADT getNode(pid_t pid, listADT list) {
	return searchNodeList(list, (void *)pid, equalsPid);
}

nodeListADT getNodeReadyQueue(pid_t pid) {
	return getNode(pid, readyQueue);
}

nodeListADT getNodeWaitingQueue(pid_t pid) {
	return getNode(pid, waitingQueue);
}

void removeProcess(nodeListADT processNode, listADT queue) {
	processNodeADT myProcessNode = getProcessNodeFromNode(processNode);
	removeNodeList(queue, processNode);
	freeProcessNodeReadOnly((void*) myProcessNode);
}

void moveNode(nodeListADT processNode, listADT fromQueue, listADT toQueue) {
	moveNodeToIndexList(toQueue, fromQueue, processNode, getSizeList(toQueue));
}

void *duplicateProcessNode(void *_processNode) {
	processNodeADT processNode = (processNodeADT) _processNode;
	processNodeADT newProcessNode = pmalloc(sizeof(processNodeCDT), SYSTEM_PID);
	newProcessNode->process = duplicateProcessReadOnly(processNode->process);
	newProcessNode->executedOnTicks = processNode->executedOnTicks;
	newProcessNode->priority = processNode->priority;

	return (void*)newProcessNode;
}

void freeProcessNodeReadOnly(void *_processNode) {
	processNodeADT processNode = (processNodeADT) _processNode;
	freeProcessReadOnly(processNode->process);
	pfree(processNode, SYSTEM_PID);
}

processNodeADT getProcessNodeFromNode(nodeListADT node) {
	return (processNodeADT)(getElementList(node));
}

t_process getProcessFromNode(nodeListADT node) {
	if (node == NULL) return NULL;
	return getProcessNodeFromNode(node)->process;
}

uint8_t equalsPid(void *_process, void *_pid) {
	t_process process = (t_process ) process;
	pid_t pid = (pid_t) _pid;
	return getProcessPid(process) == pid;
}
