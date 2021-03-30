#ifndef _KERNELSE_H_
#define _KERNELSE_H_

#include "queue.h"
#include "system.h"
#include "kernelse.h"

typedef unsigned int Time;

class KernelSem {
public:
	friend class Semaphore;

	int wait(Time maxTimeToWait);
	int signal(int n=0);

	int val() const;

	KernelSem(int init=1);
	~KernelSem();
	friend class System;
private:
	int vrednost;
	Queue *obican_red;
	Queue *vremenski_red;
};

#endif
