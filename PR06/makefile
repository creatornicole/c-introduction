# Make mehrere Programme, die im Rahmen des Praktikum 06 in SysProg
# erstellt wurden
all: prog-checkDir status-prog prog-getDirInfo

# Make CheckDir Programm
prog-checkDir: checkDir.o testDir.o
	gcc -o prog-checkDir checkDir.o testDir.o

tellIfDir.o: testDir.c
	gcc -Wall -c testDir.c

checkDir.o: checkDir.c
	gcc -Wall -c checkDir.c libmylib.h

# Make getDirInfo Programm
prog-getDirInfo: checkDir.o getDirInfo.o
	gcc -o prog-getDirInfo checkDir.o getDirInfo.o

tellIfDir.o: getDirInfo.c
	gcc -Wall -c getDirInfo.c

checkDir.o: checkDir.c
	gcc -Wall -c checkDir.c libmylib.h

# Make sonstige Programme
status-prog: statusinfo.c
	gcc -g -Wall -o status-prog statusinfo.c libmylib.h
