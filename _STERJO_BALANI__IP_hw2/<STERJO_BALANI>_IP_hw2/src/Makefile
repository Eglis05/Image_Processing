all: ex1 ex2 ex3 ex4 ex5 ex6 ex7

ex1: compile
	./executable ../input/f1_dinv.txt 4 ../output/f1_dinv_4.txt

ex2: compile
	./executable ../input/f1_dinv.txt 8 ../output/f1_dinv_8.txt

ex3: compile
	./executable ../input/f2.txt 4 ../output/f2_4.txt

ex4: compile
	./executable ../input/f2.txt 8 ../output/f2_8.txt

ex5: compile
	./executable ../input/IMG1.png 8 ../output/E5.txt d

ex6: compile
	./executable ../input/IMG2.png 8 ../output/E6.txt d

ex7: compile
	./executable ../input/IMG3.png 8 ../output/E7.txt d


compile: src.cpp
	g++ -Wall -std=c++11 src.cpp `pkg-config --libs --cflags opencv` -o executable 