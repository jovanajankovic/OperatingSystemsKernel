#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"

class Queue;
class Thread;

class PCB
{
public:
	PCB(Thread* myT, StackSize stackSize, Time timeSlice);
	~PCB();

	Thread* myThread;
	unsigned int sp, ss;

	enum Stanje { NOVA, SPREMNA, BLOKIRANA, ZAVRSENA};
	volatile Stanje stanje;

	friend class Thread;

	volatile Time proteklo_vreme;
	Time tajm_slajs;
	Time vreme_spavanja;
	int povratna_vrednost;//potrebna mi je za wait

	StackSize velicina_steka;

	unsigned* stek;
	ID id;
	static ID pcbGlobalId;
	Queue* red;

	void start();
	void waitToComplete();
	void createStack();
};

#endif
