


main : clean checker.cpp serial.o parallel.o
	g++ checker.cpp -o checker.o
serial.o : serialHull/quickserial.cpp
	g++ serialHull/quickserial.cpp -o serial.o
parallel.o : quickHull/quickhullpara.cpp
	g++ -fopenmp quickHull/quickhullpara.cpp -o parallel.o  

clean :
	rm -f *.o

runTests :
	bash check.sh
