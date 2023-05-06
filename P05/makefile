# Make mehrere Programme, die im Rahmen des Praktikum 05 in SysProg erstellt wurden
all: prog-softlink copy create-softlink

# Make Softlink Programm
prog-softlink: softlink-main.o softlink.o
	gcc -o prog-softlink softlink-main.o softlink.o

softlink-main.o: softlink-main.c
	gcc -Wall -c softlink-main.c

softlink.o: softlink.c softlink.h
	gcc -Wall -c softlink.c
	
# Make sonstige Programme
copy: copy.c
	gcc -g -Wall -o copy copy.c

create-softlink: create-softlink.c
	gcc -g -Wall -o create-softlink create-softlink.c



