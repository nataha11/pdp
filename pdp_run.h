#pragma once

extern word reg[8];
#define pc reg[7]

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (* do_func)(void);
} Command;

#define W 0  //word cmd
#define B 1  //byte cmd

void run();

typedef struct { //argument SS or DD
	word val;
	word adr;
} Arg;

Arg get_mr(word w); // get mode, register