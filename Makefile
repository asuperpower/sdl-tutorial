test:
	make build;
	make run;

build:
	gcc -Wall -c -g main.c;
	gcc -lSDL main.o -o main;

run:
	./main;

clean:
	-rm main;
	-rm main.o;

