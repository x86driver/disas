TARGET = disas

all: $(TARGET)

disas:disas.cpp opcode.h
	g++ -Wall -g -o $@ $<

clean:
	rm -rf $(TARGET)

