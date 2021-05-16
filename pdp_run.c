#include "pdp_run.h"

word reg[8];
#define pc reg[7]

Arg ss, dd;

byte byte_or_word = 0;

void do_halt() {
	trace_reg();
	trace("HALT END ^_^\n");
	exit(0);
}

void do_mov() {
	if(byte_or_word == W)
		w_write(dd.adr, ss.val);
	else if(byte_or_word == B)
		b_write(dd.adr, ss.val);
}

void do_add() {
	word tmp = w_read(dd.adr);
	tmp += ss.val;
	w_write(dd.adr, tmp);
	return;
}

void do_nothing() {
	trace("do_nothingggggggg\n");
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
			if ((w & cmd[i].mask) == cmd[i].opcode) {
				trace("%s ", cmd[i].name);

				byte_or_word = ((cmd[i].opcode) >> 15) & 1; //byte or word cmd
				if(cmd[i].opcode != 0000000) {
					ss = get_mr(w >> 6);
					dd = get_mr(w);
				}
				trace("\n");
				cmd[i].do_func();
				trace_reg();
				break;
			}
			i++;
		} while (cmd[i - 1].mask != 0000000);  //пока не наткнемся на unknown
	}
}

Arg get_mr(word w) {
	Arg res;
	int r = w & 7;//номер регистра (последние 3 бита)
	int mode = (w >> 3) & 7; //номер моды

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
	return res;
}