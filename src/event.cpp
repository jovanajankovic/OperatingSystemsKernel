#include "event.h"
#include "IVTEntry.h"
#include "kernelev.h"
#include "system.h"
#include "kernelse.h"
#include "SCHEDULE.H"

Event::Event (IVTNo ivtNo) {
	lock();
	myImpl = new KernelEv(ivtNo);
	unlock();
}

Event::~Event () {
	lock();
	delete myImpl;
	unlock();
}

void Event::signal() {
	lock();
	myImpl->signal();
	unlock();
}

void Event::wait () {
	lock();
	myImpl->wait();
	unlock();
}


KernelEv::KernelEv (IVTNo ivtNo) {
	lock();
	semafor = new KernelSem(0);
	this->ivtNo=ivtNo;
	this->vlasnik=(PCB*)System::running;
	IVTEntry::IVTTable[ivtNo]->ker_event = this;
	unlock();
}

KernelEv::~KernelEv () {
	lock();
	if (IVTEntry::IVTTable[ivtNo] != 0)
		IVTEntry::IVTTable[ivtNo]->ker_event = 0;
	delete semafor;
	unlock();
}

void KernelEv::signal() {
	lock();

	if (semafor->val() == 1) {
		unlock();
		return;
	}
	semafor->signal();

	unlock();
}

void KernelEv::wait () {
	lock();
	if (System::running != vlasnik) {
		unlock();
		return;
	}

	semafor->wait(0);
	unlock();
}


