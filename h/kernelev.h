#ifndef _KERNELEV_H_
#define _KERNELEV_H_

#include "event.h"

class Thread;
class PCB;
class KernelSem;

class KernelEv {
public:
	KernelEv (IVTNo ivtNo);
	~KernelEv ();
	void wait ();
	void signal();
private:
	IVTNo ivtNo;
	PCB* vlasnik;
	KernelSem *semafor;
};

#endif
