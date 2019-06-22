all:
	gcc -nostdlib -nostartfiles -fPIC -pie -fomit-frame-pointer virus.c -o virus
clean:
	rm virus
	rm *~
