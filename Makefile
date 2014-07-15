PROG = hamlib.exe
CC = gcc
CFLAGS = -std=c11 -Wall -o3 -g

$(PROG): main.o lexer.o parser.o list.o generator.o HtmlElement.o HtmlAttribute.o
	$(CC) $(CFLAGS) $^ -o $@

lexer.o: lexer.h

parser.o: parser.h lexer.h HtmlElement.h

list.o: list.h

generator.o: generator.h list.h HtmlElement.h HtmlAttribute.h

HtmlElement.o: HtmlElement.h HtmlAttribute.h

HtmlAttribute.o: HtmlAttribute.h

clean:
	del *.o *.exe