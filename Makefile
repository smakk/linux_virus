all:
	gcc -nostdlib -nostartfiles -fPIC -fomit-frame-pointer virus.c -o virus
clean:
	rm virus
	rm *~
