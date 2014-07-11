PROG = hamlib.exe
CC = gcc
CFLAGS = -std=c11 -Wall -o3 -g

$(PROG): main.o hamlib.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	del *.o *.exe