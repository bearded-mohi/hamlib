PROG = hamlib.exe
CC = gcc
CFLAGS = -std=c11 -Wall -o3 -g

$(PROG): main.o hamlib.o lexer.o parser.o
	$(CC) $(CFLAGS) $^ -o $@

lexer.o: lexer.h

parser.o: parser.h lexer.h hamlib.h

hamlib.o: hamlib.h

clean:
	del *.o *.exe