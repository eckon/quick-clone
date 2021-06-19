all: format main api

format: src/*
	clang-format -i --style=Google  src/*

main: src/main.cpp
	g++ src/main.cpp -lncurses -o bin/main

api: src/api.cpp
	g++ src/api.cpp -lcurl -o bin/api
