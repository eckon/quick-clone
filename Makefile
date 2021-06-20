all: format binary

format: src/*
	clang-format -i --style=Google src/*

binary:
	g++ src/* -lncurses -lcurl -o bin/qc
