#pragma once

typedef uint8_t byte;   //8 bit
typedef uint16_t word;    //16 bit
typedef word Adress;

byte b_read(Adress adr);
void b_write (Adress adr, byte b);
word w_read(Adress adr);
void w_write(Adress adr, word w);
void test_mem();

void load_file(int argc, char * argv[]);
void mem_dump(Adress start, word n);