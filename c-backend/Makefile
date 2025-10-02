# Compiler and flags
CC := gcc
CCFLAGS  := -g
# used for creating shared library objects (.so)
PICFLAGS := -fPIC

all: main simple simple_export multi multi_export

# Object files for the standard executables
pbPlots.o: pbPlots.c Makefile
	$(CC) -c pbPlots.c $(CCFLAGS) -o pbPlots.o

supportLib.o: supportLib.c Makefile
	$(CC) -c supportLib.c $(CCFLAGS) -o supportLib.o

simple.o: simple.c Makefile
	$(CC) -c simple.c $(CCFLAGS) -o simple.o

multi.o: multi.c Makefile
	$(CC) multi.c $(CCFLAGS) -o multi.o

linalg.o: linalg.c Makefile
	$(CC) linalg.c $(CCFLAGS) -o linalg.o

# Object files for the shared libraries (Position Independent)
pbPlots_pic.o: pbPlots.c Makefile
	$(CC) -c pbPlots.c $(CCFLAGS) $(PICFLAGS) -o pbPlots_pic.o

supportLib_pic.o: supportLib.c Makefile
	$(CC) -c supportLib.c $(CCFLAGS) $(PICFLAGS) -o supportLib_pic.o

linalg_pic.o: linalg.c Makefile
	$(CC) linalg.c $(CCFLAGS) $(PICFLAGS) -o linalg_pic.o

simple_pic.o: simple.c Makefile
	$(CC) -c simple.c $(CCFLAGS) $(PICFLAGS) -o simple_pic.o

multi_pic.o: multi.c Makefile
	$(CC) multi.c -o multi $(CCFLAGS) $(PICFLAGS) -o multi_pic.o

# Build targets
# Original simple executables
simple: simple.o pbPlots.o supportLib.o linalg.o Makefile
	$(CC) simple.o pbPlots.o supportLib.o linalg.o -lm -o simple

multi: multi.o linalg.o Makefile
	$(CC) multi.o linalg.o -lm -o multi

# Shared library for simple/multiple linear regression
simple_export: simple_pic.o pbPlots_pic.o supportLib_pic.o linalg_pic.o Makefile
	$(CC) simple_pic.o pbPlots_pic.o supportLib_pic.o linalg_pic.o -shared -lm -o simple_export.so

multi_export: multi_pic.o linalg_pic.o Makefile
	$(CC) multi_pic.o linalg_pic.o -shared -lm -o multi_export.so


main: main.c Makefile
	$(CC) main.c -o main $(CCFLAGS)

clean:
	rm main simple multi *.o *.so