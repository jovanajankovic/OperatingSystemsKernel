#include "pcb.h"
#include "system.h"
#include <stdlib.h>
#include <dos.h>
#include "SCHEDULE.H"

ID PCB::pcbGlobalId = 0;

PCB::PCB(Thread* myT, StackSize stackSize, Time timeSlice) {
	lock();
	myThread = myT;
	velicina_steka = stackSize;
	stek = 0;
	stanje = NOVA;
	povratna_vrednost= 1;
	proteklo_vreme = 0;
	tajm_slajs = timeSlice;
	vreme_spavanja= 1;
	red = new Queue();
	id = PCB::pcbGlobalId++;
	System::pcb_lista->dodaj(this);
	unlock();
}

PCB::~PCB() {
	lock();
	delete red;
	delete stek;
	unlock();
}

void PCB::start() {
	lock();
	stanje = SPREMNA;
	createStack();
	Scheduler::put(this);
	unlock();
}

void PCB::waitToComplete() {
	lock();
	if (stanje == ZAVRSENA || stanje == NOVA) { unlock(); return; }
	if ((this == (PCB*)System::running) || (this->myThread == System::starting_nit) || (this->myThread == System::idle_nit)) { unlock(); return; }

	System::running->stanje = BLOKIRANA;
	red->dodaj((PCB*)System::running);
	dispatch();
	unlock();
}

void PCB::createStack() {
  	lock();
	velicina_steka /= 2;
	stek = new unsigned[velicina_steka];

	stek[velicina_steka-1] = 0x200;
	stek[velicina_steka-2] = FP_SEG(&(System::wrapper));
	stek[velicina_steka-3] = FP_OFF(&(System::wrapper));

	sp = FP_OFF(stek+velicina_steka-12);
	ss = FP_SEG(stek+velicina_steka-12);
	unlock();
}
