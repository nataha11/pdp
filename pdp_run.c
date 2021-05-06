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
	trace("YOU DIED\n");
	exit(0);
}

void do_mov() {

}

void do_add() {

}

void do_nothing() {
	return;
}

void do_unknown() {

}

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov},
	{0170000, 0060000, "add", do_add},
	{0177777, 0000000, "halt", do_halt},
	{0000000, 0000000, "unknown", do_unknown}
};

void run() {
	pc = 01000;
	int i = 0;
	while(1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w);
		pc += 2;
		do {
			//trace("%s, cmd mask: %06o, word: %06o, opcode: %06o, w & mask: %06o\n", cmd[i].name, cmd[i].mask, w, cmd[i].opcode, (w & cmd[i].mask));
			if ((w & cmd[i].mask) == cmd[i].opcode) {
			trace("%s", cmd[i].name);
			cmd[i].do_func();
		}
		i++;
	} while ((w & cmd[i-1].mask) != cmd[i-1].opcode);
		
	}
}