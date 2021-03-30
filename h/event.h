#ifndef _EVENT_H_
#define _EVENT_H_

#include "ivtentry.h"


#define PREPAREENTRY(ivtno,old) \
void interrupt routine##ivtno(...); \
IVTEntry ent##ivtno(ivtno,routine##ivtno); \
void interrupt routine##ivtno(...) { \
	if (old) ent##ivtno.callOldInterrupt(); \
	ent##ivtno.signal(); \
	dispatch(); \
}

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait ();
protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;

};


#endif
