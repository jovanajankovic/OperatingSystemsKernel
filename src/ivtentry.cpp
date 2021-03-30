#include "ivtentry.h"

#include <dos.h>
#include "system.h"
#include "kernelev.h"

IVTEntry* IVTEntry::IVTTable[256] = {0};

IVTEntry::IVTEntry(int num,void interrupt (*newIR)(...)) {
	ker_event = 0;
	ivtNo=num;
	IVTTable[ivtNo]=this;
	lock();
	oldIR = getvect(ivtNo);//sacuvam staru rutinu
	setvect(ivtNo, newIR);//dodam novu rutinu
	unlock();
}

IVTEntry::~IVTEntry() {
	lock();
	oldIR(); //reko uros da se zove ako ima problema
	setvect(ivtNo, oldIR);
	unlock();
}

void IVTEntry::callOldInterrupt() { oldIR(); }

void IVTEntry::signal() {
	if (ker_event != 0) {
		ker_event->signal();
	}
}


