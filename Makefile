build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o rasteriser
run:
	./rasteriser
clean:
	rm rasteriser
