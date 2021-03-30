#include "SCHEDULE.H"
#include "semaphor.h"
#include "kernelse.h"
#include "system.h"

Semaphore::Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	System::semafori_lista->dodaj(this);
	unlock();
}

Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}

int Semaphore::val() const {
	return myImpl->val();
}

int Semaphore::wait(Time maxTimeToWait) {
	int povratna_vrednost;
	lock();
	povratna_vrednost = myImpl->wait(maxTimeToWait);
	unlock();
	return povratna_vrednost;
}

int Semaphore::signal(int n) {
	int povratna_vrednost;
	lock();
	povratna_vrednost = myImpl->signal(n);
	unlock();
	return povratna_vrednost;
}

KernelSem::KernelSem(int init) {
	vrednost = init;
	obican_red = new Queue();
	vremenski_red = new Queue();
}

KernelSem::~KernelSem() {
	delete obican_red;
	delete vremenski_red;
}

int KernelSem::val() const { return vrednost; }

int KernelSem::signal(int n) {

	int probudjeno = 0;

	if (n < 0) {
		return n;
	}

	if (n == 0){
		if (++vrednost <= 0) {
			PCB* pcb = obican_red->uzmi();
			if (pcb == 0)
				pcb = vremenski_red->uzmi();
			pcb->stanje = PCB::SPREMNA;
			Scheduler::put(pcb);
		}
	}
	else if(n>0){
		probudjeno=0;
		int i=0;
		while(i<n){
			if (++vrednost <= 0) {
							PCB* pcb = obican_red->uzmi();
							if (pcb == 0)
								pcb = vremenski_red->uzmi();
							pcb->stanje = PCB::SPREMNA;
							Scheduler::put(pcb);
							probudjeno++;
						}
			i++;
		}
	}
	return probudjeno;//trazili su u zadatku da se vrati broj probudjenih
}

int KernelSem::wait(Time maxTimeToWait) {
	System::running->povratna_vrednost = 1;
	if (--vrednost < 0){
		if (maxTimeToWait == 0){
			obican_red->dodaj((PCB*)System::running);
			System::running->stanje = PCB::BLOKIRANA;
		}
		else {
			vremenski_red->dodaj((PCB*)System::running);
			System::running->vreme_spavanja = maxTimeToWait;
			System::running->stanje = PCB::BLOKIRANA;
		}
		dispatch();
	}
	return System::running->povratna_vrednost;
}



