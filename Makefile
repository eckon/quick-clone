all: format main

format: src/*
	clang-format -i --style=Google src/*

main:
	g++ src/* -lncurses -lcurl -o bin/main
