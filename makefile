
#Use GNU compiler
cc = gcc -g
CC = g++ -g

#all: lab4
all: test
#lex.yy.o: lab4.l
#	lex lab4.l
#	$(cc) -c lex.yy.c
#
#y.tab.o: lab4.y
#	yacc -d lab4.y
#	$(CC) -c y.tab.c

main.o: main.cpp
	$(CC) -c main.cpp

test: main.o
	$(CC) -o test main.o
#lab4: y.tab.o lex.yy.o
#	$(CC) -o lab4 lex.yy.o y.tab.o

clean:
	rm -f lex.yy.c y.tab.c  y.tab.h lab4 *.o
