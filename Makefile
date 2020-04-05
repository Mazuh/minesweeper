.PHONY: build-n-run dir build run clean

build-n-run: build run

dir:
	mkdir ./build/

build:
	g++ -Wall -pedantic -ansi -std=c++17 -lm ./src/main.cpp -o ./build/minesweeper.bin

run:
	./build/minesweeper.bin

clean:
	rm -rf ./build/
