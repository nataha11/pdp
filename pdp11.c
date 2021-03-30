typedef unsigned char byte;   //8 bit
typedef unsigned int word;    //16 bit
typedef word Adress;

#define MEMSIZE (64 * 1024)

byte mem [MEMSIZE];  //global

void b_write (Adress adr, byte b);
byte b_read(Adress adr);

int main() {
	
	return 0;
}