CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -g -w
DEPS = access.h
G_OBJ = get.o access.o
P_OBJ = put.o access.o

all: get put
	chmod 4755 get put
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
get: $(G_OBJ)
	gcc $(CFLAGS) -o $@ $^
put: $(P_OBJ)
	gcc $(CFLAGS) -o $@ $^
clean:
	rm -f *.o get put
tar:
	tar -czvf %TARFILE_NAME% *.c makefile README.txt
