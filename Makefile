CC      ?= gcc
CFLAGS  ?= -std=c++11 -O2 -Wall -Wextra -Werror
LFLAGS  ?= -lstdc++

BIN     ?= bin
SRC     ?= src
TEST    ?= test
FILES   ?= files

.PHONY: main test clean

main: $(BIN)/main

test: $(BIN)/test

clean:
	rm -f $(BIN)/*
	rm -f $(FILES)/input $(FILES)/intermediate $(FILES)/output

$(BIN)/main: $(BIN)/main.o $(BIN)/huffman.o $(BIN)/bitio.o $(BIN)/constants.o
	$(CC) $(BIN)/main.o $(BIN)/huffman.o $(BIN)/bitio.o $(BIN)/constants.o $(LFLAGS) -o $(BIN)/main

$(BIN)/test: $(BIN)/test.o $(BIN)/autotest.o $(BIN)/huffman_test.o $(BIN)/huffman.o $(BIN)/bitio.o $(BIN)/constants.o
	$(CC) $(BIN)/test.o $(BIN)/autotest.o $(BIN)/huffman_test.o $(BIN)/huffman.o $(BIN)/bitio.o $(BIN)/constants.o $(LFLAGS) -o $(BIN)/test

$(BIN)/main.o: $(BIN) $(SRC)/main.cpp
	$(CC) -c $(SRC)/main.cpp $(CFLAGS) -o $(BIN)/main.o

$(BIN)/huffman.o: $(BIN) $(SRC)/huffman.cpp
	$(CC) -c $(SRC)/huffman.cpp $(CFLAGS) -o $(BIN)/huffman.o

$(BIN)/bitio.o: $(BIN) $(SRC)/bitio.cpp
	$(CC) -c $(SRC)/bitio.cpp $(CFLAGS) -o $(BIN)/bitio.o

$(BIN)/constants.o: $(BIN) $(SRC)/constants.cpp
	$(CC) -c $(SRC)/constants.cpp $(CFLAGS) -o $(BIN)/constants.o

$(BIN)/test.o: $(BIN) $(TEST)/test.cpp
	$(CC) -c $(TEST)/test.cpp $(CFLAGS) -o $(BIN)/test.o

$(BIN)/autotest.o: $(BIN) $(TEST)/autotest.cpp
	$(CC) -c $(TEST)/autotest.cpp $(CFLAGS) -o $(BIN)/autotest.o

$(BIN)/huffman_test.o: $(BIN) $(TEST)/huffman_test.cpp
	$(CC) -c $(TEST)/huffman_test.cpp $(CFLAGS) -o $(BIN)/huffman_test.o

$(BIN):
	mkdir $(BIN)
