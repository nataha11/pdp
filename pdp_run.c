#include <string.h>
#include "pdp_run.h"

word reg[8];
#define pc reg[7]

typedef struct {
	Arg ss;
	Arg dd;
	byte byte_or_word;
} Params_do;

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (* do_func)(void);
} Command;

void do_halt() {
	trace("\nHALT END ^_^\n");
	exit(0);
}

void do_mov() {
	//trace("dd.adr = %06o, ss.val = %06o, byte_or_word = %d\n", dd.adr, ss.val, byte_or_word);
	if(byte_or_word == W)
		w_write(dd.adr, ss.val);
	else if(byte_or_word == B)
		b_write(dd.adr, ss.val);
	return;
}

void do_add() {
	return;
}

void do_nothing() {
	return;
}

void do_unknown() {
	trace("do_unknownnnnnn\n");
	return;
}

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov},
	{0170000, 0060000, "add", do_add},
	{0177777, 0000000, "halt", do_halt},
	{0000000, 0000000, "unknown", do_unknown},
	
};

void run() {
	pc = 01000;

	while(1) {
		word w = w_read(pc);
		trace("[%06o] %06o: ", pc, w);
		pc += 2;
		
		int i = 0;

		do {
			trace("i1 = %d\n", i);
			if ((w & cmd[i].mask) == cmd[i].opcode) {
				trace("i2 = %d, %s ", i, cmd[i].name);

				byte_or_word = ((cmd[i].opcode) >> 15) & 1; //byte or word cmd
				ss = get_mr(w >> 6);
				dd = get_mr(w);
				trace("ss.val = %06hhx, ss.adr = %06hhx, dd.val = %06hhx, dd.adr = %06hhx\n", ss.val, ss.adr, dd.val, dd.adr);

				cmd[i].do_func();
				trace("\n");
				break;
			}
			i++;
		} while (cmd[i-1].mask != 0000000);  //пока не наткнемся на unknown
	}
}

Arg get_mr(word w) {
	Arg res;
	int r = w & 7; //номер регистра (последние 3 бита)
	int mode = (w >> 3) & 7; //номер моды
	//trace("\nmode = %d\n", mode);
	switch(mode) {
		case 0:			//R3
			res.adr = r;
			res.val = reg[r];
			trace("R%d ", r);
			break;

		case 1: 		//(R3)
			res.adr = reg[r];
			if(byte_or_word == W)
				res.val = w_read(res.adr);
			else if(byte_or_word == B)
				res.val = b_read(res.adr);
			trace("(R%d) ", r);
			break;

		case 2: 		//(R3)+		#3
			res.adr = reg[r];
			if(byte_or_word == W) {
				res.val = w_read(res.adr);
				reg[r] += 2; 
			} else if(byte_or_word == B) {
				res.val = b_read(res.adr);
				reg[r] += (r < 6) ? 1 : 2;
			}

			if (r == 7)
				trace("#%d ", res.val);
			else
				trace("(R%d)+ ", r);
			break;

		default:
			fprintf(stderr, "Mode %d not implemented yet\n", mode);
			exit(1);
	}
	//trace("\nArg: res.val = %o, res.adr = %o\n", res.val, res.adr);
	return res;
}