#include "system.h"
#include "thread.h"
#include "pcb.h"

IdleThread::IdleThread(): Thread(256, 1) {}

void IdleThread::run() {
	while (1);
}

void IdleThread::start() {
	lock();
	myPCB->stanje = PCB::SPREMNA;
	myPCB->createStack();
	unlock();
}
