ftm: main.o flighthashtable.o flightticket.o myList.o
	g++ main.o flighthashtable.o flightticket.o myList.o -o ftm
main.o: main.cpp
	g++ -c main.cpp
flighthashtable.o: flighthashtable.cpp flighthashtable.h
	g++ -c flighthashtable.cpp 
flightticket.o: flightticket.cpp flightticket.h
	g++ -c flightticket.cpp
myList.o: myList.cpp myList.h
	g++ -c myList.cpp
clean: 
	rm ftm main.o myList.o flighthashtable.o flightticket.o 