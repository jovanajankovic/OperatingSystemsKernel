#include <stdlib.h>
#include "queue.h"
#include "system.h"
#include "queuesem.h"

Queue::Queue() {
	lock();
	prvi = 0;
	posl=0;
	duzina= 0;
	unlock();
}

Queue::~Queue() {
	lock();
	Elem* stari;
	while( prvi != 0 )
	{
		stari = prvi;
		prvi = prvi->sled;
		delete stari;
		duzina--;
	}
	unlock();
}


int Queue::velicina() {
	return duzina;
}


PCB* Queue::uzmi() {
	if (prvi == 0 ) return 0;
	lock();
	duzina--;
	Elem* pom = prvi;
	PCB *p = pom->pcb;
	prvi = prvi->sled;
	delete pom;
	unlock();
	return p;
}


void Queue::dodaj(PCB* pcb) {
	lock();
	Elem *novi = new Elem();
	novi->pcb = pcb;
	novi->sled = 0;
	if (prvi == 0)
		prvi = novi;
	else {
		Elem *tek;
		for (tek=prvi; tek->sled; tek=tek->sled);
		tek->sled = novi;
	}
	duzina++;
	unlock();

	/*lock();
	Elem *novi=new Elem(pcb);
		if(prvi==0)
			prvi=novi;
		else
			posl->sled=novi;
		posl=novi;

		duzina++;
		unlock();
*/

}

void Queue::izbaci(PCB* p){
	lock();
		if(p==0){
			unlock();
			return;
		}

		Elem* temp=Queue::prvi;

		Elem* pret=0;

		if(temp!=0 && temp->pcb==p){
			prvi=temp->sled;
			delete temp;
			duzina--;
			unlock();
			return;
		}

		while(temp!=0 && temp->pcb!=p){
			pret=temp;
			temp=temp->sled;
		}

		if(temp==0){
			unlock();
			return;
		}

		pret->sled=temp->sled;
		delete temp;
		duzina--;
		unlock();

		return;
}

QueueSem::QueueSem() {
	lock();
	prvi = 0;
	duzina = 0;
	unlock();
}

QueueSem::~QueueSem() {
	lock();
	ElemSem* stari;
	while( prvi != 0 )
	{
		stari = prvi;
		prvi = prvi->sled;
		delete stari;
		duzina--;
	}
	unlock();
}

int QueueSem::velicina(){
	return duzina;
}

Semaphore* QueueSem::uzmi() {
	if (prvi == 0 ) return 0;
	lock();
	duzina--;
	ElemSem* pom = prvi;
	Semaphore *s = pom->semafor;
	prvi = prvi->sled;
	delete pom;
	unlock();
	return s;
}


void QueueSem::dodaj(Semaphore *s) {
	lock();
	ElemSem *novi = new ElemSem();
	novi->semafor = s;
	novi->sled = 0;
	if (prvi == 0)
		prvi = novi;
	else {
		ElemSem *tek;
		for (tek=prvi; tek->sled; tek=tek->sled);
		tek->sled = novi;
	}
	/*
	ElemSem **dp = &prvi;
	while((*dp) != 0)
	{
		if( (*dp)->semafor == s ) { unlock(); return; }
		dp = &((*dp)->sled);
	}
	(*dp) = new ElemSem();
	(*dp)->sled = 0;
	(*dp)->semafor = s;*/
	duzina++;
	unlock();

}
