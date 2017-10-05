all:
	gcc -o cpu_load.out main.c

clean:
	rm *.out

run:
	make clean
	make all
	./cpu_load.out


