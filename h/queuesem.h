#ifndef _QUEUESEM_H_
#define _QUEUESEM_H_

#include "semaphor.h"

class Semaphore;

class ElemSem {
public:
	Semaphore *semafor;
	ElemSem* sled;
};

class QueueSem {
public:
	friend class System;

	int velicina();
	void dodaj(Semaphore *s);
	Semaphore* uzmi();

	QueueSem();
	~QueueSem();

private:
	ElemSem* prvi;
	ElemSem* posl;
	int duzina;
};


#endif
