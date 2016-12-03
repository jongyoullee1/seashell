seashell:	shell.c
	gcc shell.c -o shell

run:	seashell
	./shell

clean:
	rm *~
	rm .out
