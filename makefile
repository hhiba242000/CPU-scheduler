
#Use GNU compiler
cc = gcc -g
CC = g++ -g

all: lab4
#all: test

main.o: main.cpp
	$(CC) -c main.cpp

ProcessScheduler.o: ProcessScheduler.cpp
	$(CC) -c ProcessScheduler.cpp

Process.o: Process.cpp
	$(CC) -c Process.cpp

lab4: Process.o ProcessScheduler.o  main.o
	$(CC) -o lab4 Process.o ProcessScheduler.o main.o -ll

clean:
	rm -f lex.yy.c y.tab.c  y.tab.h lab4 *.o
