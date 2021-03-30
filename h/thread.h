
#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize=4096;
typedef unsigned int Time;
const Time defaultTimeSlice=2;
typedef int ID;


#include "thread.h"
#include "system.h"
#include "PCB.h"
#include "queue.h"

class PCB;

class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:
	friend class PCB;
	friend class System;
	Thread(StackSize stackSize=defaultStackSize,Time timeSlice=defaultTimeSlice);
	virtual void run(){}


	PCB* myPCB;

};

class IdleThread : public Thread {
public:
	IdleThread();
	virtual void run();
	void start();
};

void dispatch();

void tick();

#endif /* THREAD_H_ */
