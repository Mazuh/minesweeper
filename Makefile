.PHONY: build-n-run dir build run clean

build-n-run: dir build run

dir:
	mkdir -p ./build/

build:
	g++ -Wall -pedantic -ansi -std=c++17 ./src/main.cpp -o ./build/minesweeper.bin

run:
	./build/minesweeper.bin

clean:
	rm -rf ./build/
