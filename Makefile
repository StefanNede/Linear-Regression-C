CCFLAGS  := -g

all: main simple multi

main: main.c Makefile
	gcc main.c -o main $(CCFLAGS)

simple: simple.c Makefile
	gcc simple.c -o simple $(CCFLAGS)

multi: multi.c Makefile
	gcc multi.c -o multi $(CCFLAGS)

clean:
	rm main simple multi *.o 