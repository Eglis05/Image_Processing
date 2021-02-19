all: ex1 ex2 ex3 ex4 ex5 ex6 ex7 ex8 ex9 ex10 ex11 

ex11: compile ex5
	./executable d ../input/SE5.txt ../output/ef3_e4.txt ../output/of3_o4.txt

ex10: compile ex4
	./executable d ../input/SE4.txt ../output/ef3_e3.txt ../output/of3_o3.txt

ex9: compile
	./executable d ../input/SE6.txt ../input/f3.txt ../output/df3_d5.txt

ex8: compile
	./executable d ../input/SE5.txt ../input/f3.txt ../output/df3_d4.txt

ex7 : compile
	./executable d ../input/SE4.txt ../input/f3.txt ../output/df3_d3.txt

ex6: compile
	./executable e ../input/SE6.txt ../input/f3.txt ../output/ef3_e5.txt

ex5: compile
	./executable e ../input/SE5.txt ../input/f3.txt ../output/ef3_e4.txt

ex4: compile
	./executable e ../input/SE4.txt ../input/f3.txt ../output/ef3_e3.txt

ex3: compile
	./executable e ../input/SE3.txt ../input/f3.txt ../output/ef3_e2.txt

ex2: compile
	./executable e ../input/SE2.txt ../input/f2.txt ../output/ef2_e2.txt

ex1: compile
	./executable e ../input/SE1.txt ../input/f1.txt ../output/ef1_e1.txt

compile: src.cpp
	g++ -Wall -std=c++11 src.cpp `pkg-config --libs --cflags opencv` -o executable 