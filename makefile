all: mainC

mainC: globals.o listDir.o misc.o modes.o main.o
	g++ globals.o listDir.o misc.o modes.o main.o

main.o: main.cpp
	g++ -c main.cpp

globals.o: libraries/globals.cpp
	g++ -c libraries/globals.cpp

listDir.o: libraries/listDir.cpp
	g++ -c libraries/listDir.cpp

misc.o: libraries/misc.cpp
	g++ -c libraries/misc.cpp

modes.o: libraries/modes.cpp
	g++ -c libraries/modes.cpp

clean:
	rm *.o a.out