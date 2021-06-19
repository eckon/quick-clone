all: main

main: src/main.cpp
	g++ src/main.cpp -lncurses -o bin/main
