#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "pcb.h"
#include "thread.h"
#include "queuesem.h"

#define lock() {asm{pushf; cli;}}
#define unlock() {asm{popf}}

class IdleThread;
class QueueSem;
class Queue;

class System {
public:
	static void set();
	static void restore();
	static void load();
	static void unload();
private:
	friend class Thread;
	friend class PCB;
	friend class KernelSem;
	friend class Semaphore;
	friend class KernelEv;
	friend void dispatch();

	static Thread* starting_nit;
	static volatile PCB* running;
	static IdleThread* idle_nit;
	static volatile int dispatched;//sa laba, da li je zahtevana promena konteksta
	static Queue *pcb_lista;
	static QueueSem *semafori_lista;


	static void update_semafore();
	static void wrapper();
	static void interrupt (*oldRoutine)(...);
	static void interrupt timer_prekid(...);

};

#endif
