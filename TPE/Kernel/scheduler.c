#include "scheduler.h"
#include "time.h"
#include "stdio.h"
#include "processHandler.h"
#include "memManager.h"

#define QUANTUM 4

typedef struct processListNodeCDT {
	t_process *process;
	processListNodeADT next;
} processListNodeCDT;

typedef struct processListCDT {
	processListNodeADT firstProcessNode;
	processListNodeADT currentProcessNode;
	uint64_t length;
	uint8_t initialized;
} processListCDT;

typedef struct queueNodeCDT *queueNodeADT;
typedef struct queueCDT *queueADT;

typedef struct queueNodeCDT {
	t_priority priority;
    t_process *process;
    uint64_t executedOnTicks;
    queueNodeADT previous;
    queueNodeADT next;
} queueNodeCDT;

typedef struct queueCDT {
	queueNodeADT firstProcessNode;
	queueNodeADT lastProcessNode;
	uint64_t count;
} queueCDT;

static const uint64_t quantumSlice[2] = {QUANTUM * 2, QUANTUM};

static queueNodeADT currentProcessNode = NULL;
static queueADT waitingQueue = NULL;
static queueADT readyQueue = NULL;

static uint8_t initialized = 0;

static t_process *idleProcess = NULL;

void idleFunction();

void loadNextProcess(t_stack *currentProcessStack);
queueNodeADT fetchNextNode();
void dispatchProcess(t_process *process, t_stack *currentProcessStack);
queueNodeADT getNode(pid_t pid, queueADT queue);
queueNodeADT getNodeReadyQueue(pid_t pid);
queueNodeADT getNodeWaitingQueue(pid_t pid);
void removeProcess(queueNodeADT processNode, queueADT queue);
void moveNode(queueNodeADT processNode, queueADT fromQueue, queueADT toQueue);

// Public
// void initializeScheduler(t_process *firstProcess, t_priority priority) {
	// addProcess(firstProcess, priority);
void initializeScheduler(t_process *firstProcess) {
	idleProcess = createProcess("idle", idleFunction, SYSTEM_PID, SYSTEM_PID, 0, NULL, NULL);

	waitingQueue = pmalloc(sizeof(queueCDT), SYSTEM_PID);
	waitingQueue->firstProcessNode = waitingQueue->lastProcessNode = NULL;

	readyQueue = pmalloc(sizeof(queueCDT), SYSTEM_PID);
	readyQueue->firstProcessNode = readyQueue->lastProcessNode = NULL;

	initialized = 1;
}

void runScheduler(t_stack *currentProcessStack) {
	if (!initialized || readyQueue->count == 0) {
		return;
	}

	if (currentProcessNode != NULL) {
		if (currentProcessNode->process->state == RUNNING && ticks_elapsed() - currentProcessNode->executedOnTicks < quantumSlice[currentProcessNode->priority]) return;
	}

	loadNextProcess(currentProcessStack);
}

uint8_t addProcess(t_process *process, t_priority priority) {
	queueNodeADT processNode = pmalloc(sizeof(queueNodeCDT), SYSTEM_PID);
	if (processNode == NULL) {
		// TODO: Error
		return 0;
	}
	processNode->priority = priority;
	processNode->process = process;
	processNode->executedOnTicks = 0;
	processNode->next = NULL;

	if (readyQueue->firstProcessNode == NULL) {
		readyQueue->firstProcessNode = readyQueue->lastProcessNode = processNode;
		processNode->previous = NULL;
	} else {
		processNode->previous = readyQueue->lastProcessNode;
		readyQueue->lastProcessNode = readyQueue->lastProcessNode->next = processNode;
	}

	readyQueue->count++;	
	return 1;
}

void killProcess(pid_t pid) {
	queueNodeADT processNode = getNodeReadyQueue(pid);
	if (processNode == NULL) {
		processNode = getNodeWaitingQueue(pid);
		if (processNode == NULL) return;

		removeProcess(processNode, waitingQueue);
		return;
	}

	if (processNode == currentProcessNode) {
		processNode->process->state = DEAD;
	} else {
		removeProcess(processNode, readyQueue);
	}
}

t_process *getCurrentProcess() {
	if (currentProcessNode != NULL) return currentProcessNode->process;
	return idleProcess;
}

pid_t getCurrentProcessPid() {
	if (currentProcessNode != NULL) return currentProcessNode->process->pid;
	return 0;
}

void lockProcess(pid_t pid) {
	queueNodeADT processNode = getNodeReadyQueue(pid);
	if (processNode == NULL) return;
	processNode->process->state = LOCKED;
	
	if (processNode != currentProcessNode) {
		moveNode(processNode, readyQueue, waitingQueue);
	}
}

void unlockProcess(pid_t pid) {
	queueNodeADT processNode = getNodeWaitingQueue(pid);
	if (processNode == NULL) return;
	processNode->process->state = READY;
	
	moveNode(processNode, waitingQueue, readyQueue);
}

t_state getCurrentProcessState() {
	if (currentProcessNode == NULL) return INVALID;
	return currentProcessNode->process->state;
}





processListADT createProcessList() {
	processListADT processList = pmalloc(sizeof(processListCDT), SYSTEM_PID);
	processList->currentProcessNode = NULL;
	processListNodeADT processListNode = NULL;
	processList->initialized = 0;
	processList->length = 0;

	queueNodeADT processNode = readyQueue->firstProcessNode;
	while (processNode != NULL) {
		if (processListNode == NULL) {
			processListNode = processList->firstProcessNode = pmalloc(sizeof(processListNodeCDT), SYSTEM_PID);
		} else {
			processListNode = processListNode->next = pmalloc(sizeof(processListNodeCDT), SYSTEM_PID);
		}

		processListNode->process = pmalloc(sizeof(t_process), SYSTEM_PID);
		memcpy(processListNode->process, processNode->process, sizeof(t_process));
		processList->length++;

		processNode = processNode->next;
	}
	if (processListNode != NULL) {
		processListNode->next = NULL;
	}

	return processList;
}

uint8_t hasNextProcess(processListADT processList) {
	if (processList != NULL) {
		if (!processList->initialized) {
			return processList->firstProcessNode != NULL;
		} else {
			return processList->currentProcessNode->next != NULL;
		}
	}
	return 0;
}

uint64_t getProcessListLength(processListADT processList) {
	if (processList != NULL) return processList-> length;
	return 0;
}


t_process *getNextProcess(processListADT processList) {
	if (!hasNextProcess(processList)) return NULL;

	if (!processList->initialized) {
		processList->initialized = 1;
		processList->currentProcessNode = processList->firstProcessNode;
	} else {
		if (processList->currentProcessNode == NULL) {
			processList->currentProcessNode = processList->firstProcessNode;
		} else {
			processList->currentProcessNode = processList->currentProcessNode->next;
		}
	}

	return processList->currentProcessNode->process;
}

void freeProcessesList(processListADT processList) {
	processListNodeADT processListNode = NULL;
	processListNodeADT auxProcessListNode;

	if (processList != NULL) {
		processListNode = processList->firstProcessNode;
		while (processListNode != NULL) {
			auxProcessListNode = processListNode->next;
			pfree(processListNode->process, SYSTEM_PID);
			pfree(processListNode, SYSTEM_PID);
			processListNode = auxProcessListNode;
		}
		pfree(processList, SYSTEM_PID);
		processList = NULL;
	}
}

// Private
void loadNextProcess(t_stack *currentProcessStack) {
	queueNodeADT nextProcessNode = fetchNextNode();
	if (nextProcessNode == NULL) {		
		if (currentProcessNode != NULL) {
			currentProcessNode->process->state = READY;
			printf("s: ");
			updateStack(currentProcessNode->process->stackPointer, currentProcessStack);
		}

		printf("i\n");
		dispatchProcess(idleProcess, currentProcessStack);
		return;
	}

	if (nextProcessNode == currentProcessNode) {
		currentProcessNode->executedOnTicks = ticks_elapsed();
	} else {
		if (currentProcessNode != NULL) {
			currentProcessNode->process->state = READY;
			printf("s: ");
			updateStack(currentProcessNode->process->stackPointer, currentProcessStack);
		}

		currentProcessNode = nextProcessNode;
		currentProcessNode->executedOnTicks = ticks_elapsed();
		printf("d %s: ", currentProcessNode->process->name);
		dispatchProcess(currentProcessNode->process, currentProcessStack);
	}
}

queueNodeADT fetchNextNode() {
	queueNodeADT currentNode = currentProcessNode;
	if (currentNode == NULL) { // Solo pasa esto cuando no hay ningun proceso corriendo
		currentNode = readyQueue->firstProcessNode;
	} else {
		currentNode = currentProcessNode->next;
	}
	printf("Fetching next\n");

	while (currentNode != currentProcessNode && readyQueue->count >= 1) {
	printf("while\n");
		if (currentNode == NULL) {
			currentNode = readyQueue->firstProcessNode;
		} else {
			if (currentNode->process->state == DEAD) {
				removeProcess(currentNode, readyQueue);
			} else if (currentNode->process->state == READY) {
				break;
			} else if (currentNode->process->state == LOCKED) {
				moveNode(currentNode, readyQueue, waitingQueue);
			}

			currentNode = currentProcessNode->next;
		}
	}
	if (currentNode == currentProcessNode) {
	printf("Fetching same\n");

		return currentNode;
	}

	if (readyQueue->count == 0 || currentNode == NULL || currentNode->process->state != READY) {
	printf("Fetching nulll\n");
		return NULL;
	}

	printf("Fetching next\n");
	printf(currentNode->process->name);
	return currentNode;
}

void dispatchProcess(t_process *process, t_stack *currentProcessStack) {
	process->state = RUNNING;
	updateStack(currentProcessStack, process->stackPointer);
}

queueNodeADT getNode(pid_t pid, queueADT queue) {
	queueNodeADT processNode = queue->firstProcessNode;

	while (processNode == NULL || processNode->process->pid != pid) {
		processNode = processNode->next;
	}
	return processNode;
}

queueNodeADT getNodeReadyQueue(pid_t pid) {
	return getNode(pid, readyQueue);
}

queueNodeADT getNodeWaitingQueue(pid_t pid) {
	return getNode(pid, waitingQueue);
}

void removeProcess(queueNodeADT processNode, queueADT queue) {
	if (processNode->previous != NULL) {
		processNode->previous->next = processNode->next;
	} else {
		queue->firstProcessNode = processNode->next;
	}
	if (queue->lastProcessNode == processNode) {
		queue->lastProcessNode = NULL;
	}

	freeProcess(processNode->process);
	pfree(processNode, SYSTEM_PID);
	queue->count--;
}

void moveNode(queueNodeADT processNode, queueADT fromQueue, queueADT toQueue) {
	if (processNode->previous != NULL) {
		processNode->previous->next = processNode->next;
	} else {
		fromQueue->firstProcessNode = processNode->next;
	}
	if (processNode->next == NULL) {
		fromQueue->lastProcessNode = processNode->previous;
	}

	if (toQueue->firstProcessNode == NULL) {
		toQueue->firstProcessNode = processNode;
		toQueue->lastProcessNode = processNode;
		processNode->next = processNode->previous = NULL;
	} else {
		toQueue->lastProcessNode->next = processNode;
		processNode->previous = toQueue->lastProcessNode;
		toQueue->lastProcessNode = processNode;
	}

	processNode->next = NULL;
}
