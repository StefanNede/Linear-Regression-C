CCFLAGS  := -g

all: simple

simple: simple.c Makefile
	gcc simple.c -o simple $(CCFLAGS)

clean:
	rm simple