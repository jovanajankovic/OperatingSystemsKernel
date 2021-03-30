#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

class IVTEntry {
	friend class KernelEv;
private:
	static IVTEntry* IVTTable[];//to mi je ovaj globalni niz pokazivaca na ivtentry
	
	unsigned ivtNo;
	KernelEv* ker_event;
	void interrupt (*oldIR)(...);

public:
	IVTEntry(int,void interrupt(*)(...));
   ~IVTEntry();
	void signal();
	void callOldInterrupt();
};

#endif
