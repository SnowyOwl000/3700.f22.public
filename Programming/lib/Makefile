CFLAGS = -c -I$(HOME)/Programming/include

libdataStructures.a: ../src/fraction.o
	ar r libdataStructures.a ../src/fraction.o

../src/fraction.o: ../src/fraction.cpp ../include/fraction.h
	g++ $(CFLAGS) -o ../src/fraction.o ../src/fraction.cpp

