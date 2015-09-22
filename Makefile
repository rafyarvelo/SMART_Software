#Top Level Makefile for SMART Software

all:
	cd BCI && qmake && make
	cd BCI/TM_EXTRACT && qmake && make

clean:
	cd BCI && make clean
	cd BCI/TM_EXTRACT && make clean
