CC=g++
# CFLAGS=--std=c++11
SRC=src/genKeys.cpp src/numbers/encrypt.cpp src/numbers/decrypt.cpp src/text/encrypt.cpp src/text/decrypt.cpp src/attaques/factorisation.cpp
EXEC=bin/genKeys bin/numbers/encrypt bin/numbers/decrypt bin/text/encrypt bin/text/decrypt bin/attaques/factorisation
DIRS=bin bin/numbers bin/text bin/attaques

all: $(EXEC)

bin/genKeys: src/genKeys.cpp
	$(CC) -o bin/genKeys src/genKeys.cpp

bin/numbers/encrypt: src/numbers/encrypt.cpp
	$(CC) -o bin/numbers/encrypt src/numbers/encrypt.cpp

bin/numbers/decrypt: src/numbers/decrypt.cpp
	$(CC) -o bin/numbers/decrypt src/numbers/decrypt.cpp

bin/text/encrypt: src/text/encrypt.cpp
	$(CC) -o bin/text/encrypt src/text/encrypt.cpp

bin/text/decrypt: src/text/decrypt.cpp
	$(CC) -o bin/text/decrypt src/text/decrypt.cpp

bin/attaques/factorisation: src/attaques/factorisation.cpp
	$(CC) -o bin/attaques/factorisation src/attaques/factorisation.cpp

clean:
	rm -rf bin

# will create all necessary directories after the Makefile is parsed
$(shell mkdir -p $(DIRS))
