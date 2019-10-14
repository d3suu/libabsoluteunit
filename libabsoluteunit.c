#include <stdio.h> //printf
#include <string.h> //memset, memcpy
#include <inttypes.h> //uint16_t

int IsBigEndian(){
	int i=1;
	return ! *((char *)&i);
}

// TODO move struct, typedef, function names to header
struct __m4096struct {
	unsigned char n[4096];
};

typedef struct __m4096struct __m4096;

void m4096zero(__m4096* variable){
	/*
	 * set variable to zero
	 * alternative: memset(&variable, 0x00, 4096);
	 */
	for(int i = 0; i<4096; i++)
		variable->n[i] = 0x00;
}

void m4096printhex(__m4096* variable){
	/*
	 * print variable in hex
	 */
	printf("0x");
	for(int i = 0; i<4096; i++){
		printf("%02X", variable->n[i]);
	}
}

// TODO
// char* m4096tohex(__m4096* variable)
// __m4096 hextom4096(char* string)
// __m4096 uitom4096(unsigned int value)
// __m4096 ulltom4096(unsigned long long value)

void m4096add(__m4096* a1, __m4096* a2){ 
	/*
	 * add a1 to a2, store value in a1
	 * TODO if output > 4096^2, set a1 to zero
	 */
	// variables for addition
	char carry;
	int BigEndian = IsBigEndian();
	__m4096 buffer;
	union Value {
		char cval[2]; // value[1] == value, value[0] == carry
		uint16_t ival;
	} value;
	
	// zero every value
	memset(value.cval, 0x00, 2); //value[0,1] = 0x00;
	carry = 0x00;
	m4096zero(&buffer);
	
	// add loop
	for(int i = 4095; i >= 0; i--){
		value.ival = a1->n[i] + a2->n[i]; // add a1 and a2
		
		if(BigEndian){
			buffer.n[i] = value.cval[1] + carry;
			carry = value.cval[0];
		}else{
			buffer.n[i] = value.cval[0] + carry; // add new byte value + carry
			carry = value.cval[1]; // new carry
		}

		// clear values
		memset(value.cval, 0x00, 2);
	}

	// write buffer to a1
	memcpy(a1->n, buffer.n, 4096);
}

int main(){
	// test
	__m4096 apple, banana;
	m4096zero(&apple);
	m4096zero(&banana);
	//m4096print(&apple); printf("\n");
	
	apple.n[4095] = 0xfa;
	banana.n[4095] = 1;
	//m4096printhex(&apple);
	
	m4096add(&apple, &banana);
	m4096printhex(&apple);// printf("\n");

	return 0;
}
