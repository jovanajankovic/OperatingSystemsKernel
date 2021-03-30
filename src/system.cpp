#include "system.h"
#include "SCHEDULE.h"
#include <dos.h>
#include <stdlib.h>
#include "queuesem.h"
#include "kernelse.h"

Thread* System::starting_nit = 0;
volatile PCB* System::running = 0;
IdleThread* System::idle_nit = 0;
volatile int System::dispatched = 0;
Queue *System::pcb_lista = new Queue(); //treba da se dealocira kod gasenja sistema
QueueSem *System::semafori_lista = new QueueSem(); //treba da se dealocira kod gasenja sistema
void interrupt (*System::oldRoutine)(...) = 0;

void System::set() {
	lock();
	oldRoutine = getvect(0x08);
	setvect(0x08, timer_prekid);
	unlock();
}

void System::restore() {
	lock();
	setvect(0x08, oldRoutine);
	unlock();
}

void System::load() {
	lock();
	starting_nit = new Thread(256, 1);
	starting_nit->myPCB->stanje = PCB::SPREMNA;
	running = (volatile PCB*)starting_nit->myPCB;
	idle_nit = new IdleThread();
	idle_nit->start();
	unlock();
}


void System::unload() {
	lock();
	delete starting_nit;
	delete idle_nit;
	//delete svi semafori, svi niti
	unlock();
}

void System::update_semafore() {
	ElemSem *tekElemSem = semafori_lista->prvi;

	while(tekElemSem!=0){
		Elem *tek = tekElemSem->semafor->myImpl->vremenski_red->prvi;
		Elem* stari = 0;
		Elem *pret = 0;

		Queue *vremenski_red = tekElemSem->semafor->myImpl->vremenski_red;

		while (tek) {
			tek->pcb->vreme_spavanja--;
			if (tek->pcb->vreme_spavanja > 0) {
				pret = tek;
				tek = tek->sled;
			} else {
				vremenski_red->duzina--;
				tek->pcb->stanje = PCB::SPREMNA;
				tek->pcb->povratna_vrednost = 0;
				Scheduler::put(tek->pcb);
				//if (tek->pcb->myThread->getId() == 0) syncPrintf("*** Main se probudila\n");

				stari = tek;
				tek = tek->sled;
				if (!pret)
					vremenski_red->prvi = tek;
				else
					pret->sled = tek;
				delete stari;
				tekElemSem->semafor->myImpl->vrednost++;
			}
		}
		tekElemSem = tekElemSem->sled;
	}
}

void interrupt System::timer_prekid(...) {
	if(!dispatched ) {
		tick(); 
		(*oldRoutine)(); 
		update_semafore();
	} 
	if (!dispatched) {
		if(running->tajm_slajs != 0) running->proteklo_vreme++;
		if (running->proteklo_vreme < running->tajm_slajs || running->tajm_slajs == 0 ) return;
	}

	dispatched = 0; 

	if( running->stanje == PCB::SPREMNA && running != idle_nit->myPCB )
		Scheduler::put((PCB*)running); 

	running->sp = _SP;
	running->ss = _SS;

	running = Scheduler::get();
	if (running == 0) running = idle_nit->myPCB;

	_SP = running->sp;
	_SS = running->ss;
		
	running->proteklo_vreme = 0;
}

void System::wrapper() {
	running->myThread->run();

	lock();
	running->stanje = PCB::ZAVRSENA;

	PCB *temp = running->red->uzmi();
	while (temp != 0) {
		temp->stanje = PCB::SPREMNA;
		Scheduler::put(temp);
		temp = running->red->uzmi();
	}

	dispatch();
}

//----------------------------
/*#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "system.h"

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock();
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock();
	return res;
}*/
