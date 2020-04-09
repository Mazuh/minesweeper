.PHONY: build-n-run dir build run clean

build-n-run: dir build run

dir:
	mkdir -p ./build/

build: dir
	g++ -Wall -pedantic -ansi -std=c++17 ./src/*.cpp -o ./build/minesweeper.bin


columns=16
rows=9
mines=15
run:
	./build/minesweeper.bin $(columns) $(rows) $(mines)

clean:
	rm -rf ./build/
