all: ex1 ex2 ex3 ex4

ex1: compile
	./executable ../input/f1_dinv.txt 4 ../output/f1_dinv_4.txt

ex2: compile
	./executable ../input/f1_dinv.txt 8 ../output/f1_dinv_8.txt

ex3: compile
	./executable ../input/f2.txt 4 ../output/f2_4.txt

ex4: compile
	./executable ../input/f2.txt 8 ../output/f2_8.txt

compile: src.cpp
	g++ -Wall -std=c++11 src.cpp `pkg-config --libs --cflags opencv` -o executable 