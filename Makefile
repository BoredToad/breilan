# super duper simple makefile for now
# probably won't get much more complicated
# as I won't be using external dependencies
src/main src/sockets:
	gcc src/main.c src/sockets.c
