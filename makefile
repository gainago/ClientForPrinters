
#COMPILER=gcc

all: main

main: main.o json.o
	gcc main.o json.o -o main -lcurl
main.o: json.o
	gcc main.c -c main.o 
json.o: 
	gcc json.c -c json.o
clear:
	rm -rf *.o
