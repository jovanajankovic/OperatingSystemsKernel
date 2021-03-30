#include "system.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	System::load();
	System::set();
	int value = userMain(argc, argv);
	System::restore();
	System::unload();
	return value;
}
