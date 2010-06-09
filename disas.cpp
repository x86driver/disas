#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
using namespace std;

struct INSTR {
	string opc;
	int decode_type;
};

map<unsigned char, INSTR> opcode;
struct INSTR instr[] = {
	{"add", 0},
	{"sub", 0},
	{"ldr", 1},
	{"str", 1},
};

void init_table()
{
	for (unsigned int i = 0; i < sizeof(instr)/sizeof(instr[0]); ++i) {
		opcode[i] = instr[i];
	}
}

void parse0(unsigned char opc, unsigned char regdst,
	unsigned char regsrc1, unsigned char regsrc2)
{
	cout << "\t";

	map<unsigned char, INSTR>::iterator it = opcode.find(opc);
	if (it != opcode.end()) {
		cout << (it->second).opc << "\t";
	} else {
		cout << "unknown opcode" << endl;
		return;
	}

	cout << "r" << static_cast<int>(regdst) << ", r" <<
		static_cast<int>(regsrc1) << ", r" << static_cast<int>(regsrc2) << endl;
}

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
		parse0(buf[i], buf[i+1], buf[i+2], buf[i+3]);
	}
	return 0;
}
