#include "pdp_mem.h"

byte mem [MEMSIZE];

void test_mem() {
	byte b0 = 0x0a;
	//test: write byte, read byte
	b_write(2, b0);
	byte bres = b_read(2);
	printf("bw/br \t %02hhx = %02hhx\n", b0, bres);
	assert(b0 == bres);

	//write 2 bytes, read word
	Adress a = 4;
	byte b = 0xcb;
	b0 = 0x0a;
	word w = 0xcb0a;
	b_write(a, b0);
	b_write(a + 1, b);
	word wres = w_read(a);
	printf("ww/br \t %04hx=%02hhx%02hhx\n", wres, b, b0);
	assert(w == wres);

	//read word, write word
	Adress ad = 10;
	word wo = 0xcb0a;
	w_write(ad, wo);
	word wreso = w_read(ad);
	printf("ww/wr \t %04hx=%04hx\n", wo, wreso);
	assert(wo == wreso);

	//write word, read 2 bytes
	Adress adr = 20;
	word wor = 0xaaba;
	w_write(adr, wor);
	byte b1 = 0xaa;
	byte b2 = 0xba;
	byte bres2 = b_read(adr);
	byte bres1 = b_read(adr + 1);
	printf("ww/rb \t %04hx=%02hhx%02hhx\n", wor, bres1, bres2);
	assert((bres1 == b1) && (bres2 == b2));

	//ww wr отрицательные числа
	word neg_w = -1;//255
	w_write(100, neg_w);
	word res = w_read(100);
	printf("ww/wr \t %016hx=%016hx\n", neg_w, res);
	assert(neg_w == res);

	mem_dump(0x40, 4);
}

byte b_read(Adress adr) {
	return mem[adr];
}

void b_write (Adress adr, byte b) {
	mem[adr] = b;
}

word w_read(Adress a) {
	assert(a % 2 == 0);
	word w = ((word)mem[a + 1]) << 8;
	w = w | mem[a];
	return w;
}

void w_write(Adress adr, word w) {
	assert(adr % 2 == 0);
	mem[adr] = w & 0xFF;//извлекаем первые 8 бит
	mem[adr + 1] = (w >> 8) & 0xFF;//извлекаем следующие 8 бит
}

void load_file(int argc, char const * argv[]) {
	FILE *fin;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	fin = fopen(argv[1], "r");
	if (fin == NULL) {
		perror("File not opened");
		exit(1);
	}

	trace("\t load_file trace:\n");
	Adress a;
	byte c;
	word n;

	while (2 == fscanf(fin, "%hx%hx", &a, &n)) {
		trace("read of size %u\n", n);
		for (unsigned int i = 0; i < n; i++) {
				fscanf(fin, "%hhx", &c);
				b_write(a + i, c);
				trace("byte read: %02hhx\n", c);
		}
	}
	trace("\n");
	fclose(fin);
}

void mem_dump(Adress start, word n) {
	for (unsigned int i = 0; i < n; i += 2) {
		trace("%06o : %06o\n", start + i, w_read(start + i));
	}
}