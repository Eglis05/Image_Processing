all: ex1

ex1: compile
	./executable ../input/f1_dinv.txt 4 ../output/f1_dinv_4.txt

compile: src.cpp
	g++ -Wall -std=c++11 src.cpp `pkg-config --libs --cflags opencv` -o executable 