#include "pdp_run.h"

word reg[8];
#define pc reg[7]

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (* do_func)();
} Command;

void do_halt() {
	trace("\nHALT END ^_^\n");
	exit(0);
}

void do_mov() {
	return;
}

void do_add() {
	return;
}

void do_nothing() {
	return;
}

void do_unknown() {
	return;
}

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov},
	{0170000, 0060000, "add", do_add},
	{0177777, 0000000, "halt", do_halt},
	{0000000, 0000000, "unknown", do_unknown}
};

void run() {
	pc = 01000;

	while(1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w);
		pc += 2;

		for (int i = 0; cmd[i - 1].mask != 0000000; i++) {
			if ((w & cmd[i].mask) == cmd[i].opcode) {
				trace("%s", cmd[i].name);

				// тут потом будет разбор аргументов

				cmd[i].do_func();
				trace("\n");
				break;
			}
		}
	}
}