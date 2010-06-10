#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
#include "opcode.h"

using namespace std;

struct INSTR instr[] = {
        {"add", 0},	// opcode: 0
        {"sub", 0},
        {"ldr", 1},
        {"str", 1},
};

map<unsigned char, INSTR> opcode;

void init_table()
{
	for (unsigned int i = 0; i < sizeof(instr)/sizeof(instr[0]); ++i) {
		opcode[i] = instr[i];
	}
}

/* For add,sub */
void parse0(unsigned char opc, unsigned char regdst,
	unsigned char regsrc1, unsigned char regsrc2)
{
	cout << "\t";

	map<unsigned char, INSTR>::iterator it = opcode.find(opc);
	if (it != opcode.end()) {
		cout << (it->second).opc << "\t";
	} else {
		cout << "undefined instruction" << endl;
		return;
	}

	cout << "r" << static_cast<int>(regdst) << ", r" <<
		static_cast<int>(regsrc1) << ", r" << static_cast<int>(regsrc2) << endl;
}

/* For ldr, str */
void parse1(unsigned char opc, unsigned char regdst,
        unsigned char addrL, unsigned char addrH)	/* little endian */
{
	cout << "\t";

	map<unsigned char, INSTR>::iterator it = opcode.find(opc);
	if (it != opcode.end()) {
		cout << (it->second).opc << "\t";
	} else {
		cout << "undefined instruction" << endl;
		return;
	}

	unsigned short addr = (addrH << 8) | addrL;
	cout << "r" << static_cast<int>(regdst) << ", 0x" << hex << addr << endl;
}

typedef void (*parse_func)(unsigned char, unsigned char, unsigned char, unsigned char);
parse_func parse[] = {
	parse0,
	parse1,
};

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s [input file]\n", argv[0]);
		exit(1);
	}

	unsigned char buf[512];		// 512 bytes for input file
	FILE *fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("Can't open %s\n", argv[1]);
		perror("fopen");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread((void*)&buf[0], sizeof(buf), 1, fp);
	fclose(fp);

	file_size &= 0x0fc;	// Align 4 bytes

	init_table();

	for (int i = 0; i < file_size; i+=4) {
		map<unsigned char, INSTR>::iterator it = opcode.find(buf[i]);
		if (it != opcode.end()) {
			parse[(it->second).decode_type](buf[i], buf[i+1], buf[i+2], buf[i+3]);
		}
	}
	return 0;
}
