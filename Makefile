default:	a.out

cwd.o: src/cwd.c src/cwd.h
	cc -I. -c src/cwd.c

main.o: src/main.c
	cc -I. -c src/main.c

a.out: main.o cwd.o
	cc main.o cwd.o

clean:
	rm *.o a.out
