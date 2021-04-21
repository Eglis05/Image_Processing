all: ex1 ex2 ex3

ex1: compile ../output/crystal_4.txt
	python3 plotting.py ../output/crystal_4.txt ../output/crystal_4.png

../output/crystal_4.txt:
	./executable ../input/crystal.jpeg 4 ../output/crystal_4.txt

ex2: compile ../output/time_7.txt
	python3 plotting.py ../output/time_7.txt ../output/time_7.png

../output/time_7.txt:
	./executable ../input/time.jpeg 7 ../output/time_7.txt

ex3: compile ../output/onepiece_3.txt ../output/onepiece_8.txt ../output/onepiece_40.txt ../output/onepiece_gauss_3.txt ../output/onepiece_gauss_8.txt ../output/onepiece_gauss_40.txt ../output/onepiece_gauss_sp_3.txt ../output/onepiece_gauss_sp_8.txt ../output/onepiece_gauss_sp_40.txt 
	python3 plotting.py ../output/onepiece_3.txt ../output/onepiece_3.png
	python3 plotting.py ../output/onepiece_8.txt ../output/onepiece_8.png
	python3 plotting.py ../output/onepiece_40.txt ../output/onepiece_40.png
	python3 plotting.py ../output/onepiece_gauss_3.txt ../output/onepiece_gauss_3.png
	python3 plotting.py ../output/onepiece_gauss_8.txt ../output/onepiece_gauss_8.png
	python3 plotting.py ../output/onepiece_gauss_40.txt ../output/onepiece_gauss_40.png
	python3 plotting.py ../output/onepiece_gauss_sp_3.txt ../output/onepiece_gauss_sp_3.png
	python3 plotting.py ../output/onepiece_gauss_sp_8.txt ../output/onepiece_gauss_sp_8.png
	python3 plotting.py ../output/onepiece_gauss_sp_40.txt ../output/onepiece_gauss_sp_40.png
	python3 plot_all.py ../output/onepiece_all.png

../output/onepiece_3.txt:
	./executable ../input/onepiece.jpg 3 ../output/onepiece_3.txt

../output/onepiece_8.txt:
	./executable ../input/onepiece.jpg 8 ../output/onepiece_8.txt

../output/onepiece_40.txt:
	./executable ../input/onepiece.jpg 40 ../output/onepiece_40.txt

../output/onepiece_gauss_3.txt:
	./executable ../input/onepiece_gauss.jpg 3 ../output/onepiece_gauss_3.txt

../output/onepiece_gauss_8.txt:
	./executable ../input/onepiece_gauss.jpg 8 ../output/onepiece_gauss_8.txt

../output/onepiece_gauss_40.txt:
	./executable ../input/onepiece_gauss.jpg 40 ../output/onepiece_gauss_40.txt

../output/onepiece_gauss_sp_3.txt:
	./executable ../input/onepiece_gauss_sp.jpg 3 ../output/onepiece_gauss_sp_3.txt

../output/onepiece_gauss_sp_8.txt:
	./executable ../input/onepiece_gauss_sp.jpg 8 ../output/onepiece_gauss_sp_8.txt

../output/onepiece_gauss_sp_40.txt:
	./executable ../input/onepiece_gauss_sp.jpg 40 ../output/onepiece_gauss_sp_40.txt

compile: Homework_3.cpp
	g++ -Wall Homework_3.cpp `pkg-config --libs --cflags opencv4` -o executable

clean:
	rm ../output/*