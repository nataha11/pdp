#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;   //8 bit
typedef unsigned int word;    //16 bit
typedef word Adress;

#define MEMSIZE (64 * 1024)

byte mem [MEMSIZE];

byte b_read(Adress adr);
void b_write (Adress adr, byte b);
word w_read(Adress adr);
void w_write(Adress adr, word w);

void load_file();


void test_mem();

int main() {
	test_mem ();
	load_file("test.o");
	return 0;
}

void test_mem() {
	byte b0 = 0x0a;
	//test: write byte, read byte
	b_write(2, b0);
	byte bres = b_read(2);
	printf("bw/br \t %02hhx = %02hhx\n", b0, bres);
	assert(b0 == bres);

	//write 2 bytes, read word
	Adress a = 4;
	byte b1 = 0xcb;
	b0 = 0x0a;
	word w = 0xcb0a;
	b_write(a, b0);
	b_write(a + 1, b1);
	word wres = w_read(a);
	printf("ww/br \t %04hx=%02hhx%02hhx\n", wres, b1, b0);
	assert(w == wres);

	//read word, write word
	Adress ad = 10;
	word wo = 0xcb0a;
	w_write(ad, wo);
	word wreso = w_read(ad);
	printf("ww/wr \t %04hx=%04hx\n", wo, wreso);
	assert(wo == wreso);
}

byte b_read(Adress adr) {
	return mem[adr];
}

void b_write (Adress adr, byte b) {
	mem[adr] = b;
}

word w_read(Adress a) {
	word w = ((word)mem[a + 1]) << 8;
	w = w | mem[a];
	return w;
}

void w_write(Adress adr, word w) {
	mem[adr + 1] = (byte)(w >> 8);
	mem[adr] = (byte)w;
	
}

void load_file(const char * filname) {
	FILE * fil = fopen(filname, "rb");
	Adress a;
	byte c;
	word n;

	while(2 == fscanf(fil, "%x%x", &a, &n)) {
		printf("read of size %u\n", n);
		for (unsigned int i = 0; i < n; i++) {
				fscanf(fil, "%hhx", &c);
				b_write(a + i, c);
				printf("byte read: %hhx\n", c);
		}
	}
	printf("\n");
	fclose(fil);
}

