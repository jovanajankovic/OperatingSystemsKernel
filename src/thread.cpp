#include "system.h"
#include "pcb.h"
#include "thread.h"
#include "schedule.h"
#include "queue.h"
#include <dos.h>
#include <stdlib.h>

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock();
}

Thread::~Thread() {
	lock();
	waitToComplete();
	delete myPCB;
	unlock();
}

void dispatch() {
	lock();
	System::dispatched = 1;
	System::timer_prekid();
	System::dispatched = 0;
	unlock();
}

void Thread::start() {
	lock();
	myPCB->start();
	unlock();
}

void Thread::waitToComplete() {
	lock();
	myPCB->waitToComplete();
	unlock();
}


ID Thread::getRunningId() {
	ID result = 0;
	lock();
	result = System::running->id;
	unlock();
	return result;
}

Thread *Thread::getThreadById(ID id) {
	lock();
	Thread *pronadjeno = 0;

	Elem *tek=System::pcb_lista->prvi;
	while(tek!=0){
		if(tek->pcb->id==id) {
			pronadjeno=tek->pcb->myThread;
		}
		tek=tek->sled;
	}
	unlock();
	return pronadjeno;
}

ID Thread::getId() { return myPCB->id; }
