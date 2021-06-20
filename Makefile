all: format binary

format: src/*
	clang-format -i --style=Google src/**/*.cpp src/**/*.h

binary:
	g++ src/*.cpp src/**/*.cpp -lncurses -lcurl -o bin/qc
