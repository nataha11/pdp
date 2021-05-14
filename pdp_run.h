#pragma once

#define W 0  //word cmd
#define B 1  //byte cmd

void run();

typedef struct { //argument SS or DD
	word val;
	word adr;
} Arg;
Arg ss, dd;

byte byte_or_word = 0;

Arg get_mr(word w); // get mode, register