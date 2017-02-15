CC = gcc
SRC = $(wildcard src/*.c)
INCLUDE = $(wildcard include/)
BIN = utfconverter
CFLAGS = -g -Wall -Werror -pedantic -Wextra 
REQ = $(SRC) $(wildcard include/*.h)

all:
	mkdir -p build/
	mkdir -p bin/
	make utfconverter.o
	make $(BIN)


utfconverter.o: src/utfconverter.c
	$(CC) $(CFALGS) -c $^ -I $(INCLUDE) -o build/$@

#utfconverter: utfconverter.o
#	gcc $(CFLAGS) -I $(INCLUDE) -o bin/$@
	
utfconverter: $(REQ)
	$(CC) src/utfconverter.c $(CFLAGS) -I $(INCLUDE) -o bin/utf

.PHONY: clean

clean:
	rm -rf bin build
#BU = build/
#BI = bin/
#all: 
#	mkdir -p $(BU)
#	mkdir -p $(BI)
#	make utf
#utf: ../include/utfconverter.h
#	gcc $(CFLAGS) utfconverter.c -o utf
#utfconverter.o: ../src/utfconverter.h
#	gcc $(CFLAGS) -c $^ -I ./include -o $(BU)$@

#utfconverter: $(BU)utfconverter.o
#	gcc $(CFLAGS) $^ -o $(BI)$@ -lm
