CCFLAGS  := -g

# all: main simple multi
all: plots main simple multi simple_regression

plots: pbPlots.c supportLib.c Makefile
	gcc -c pbPlots.c $(CCFLAGS)
	gcc -c supportLib.c $(CCFLAGS)

simple: simple.c Makefile
	gcc -c simple.c $(CCFLAGS)

simple_regression: simple.o pbPlots.o supportLib.o Makefile
	gcc simple.o pbPlots.o supportLib.o -lm -o simple

main: main.c Makefile
	gcc main.c -o main $(CCFLAGS)

multi: multi.c Makefile
	gcc multi.c -o multi $(CCFLAGS)

clean:
	rm main simple multi *.o 