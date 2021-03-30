#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "pcb.h"

class PCB;

class Elem {
public:
	PCB* pcb;
	Elem* sled;
};

class Queue {
public:
	friend class System;
	friend class Thread;

	int velicina();
	void dodaj(PCB* pcb);
	PCB* uzmi();
    void izbaci(PCB* p);

	Queue();
	~Queue();

private:
	Elem* prvi, *posl;
	int duzina;
};


#endif
