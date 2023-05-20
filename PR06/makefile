# Make mehrere Programme, die im Rahmen des Praktikum 06 in SysProg
# erstellt wurden

# Make CheckDir Programm
prog-checkDir: checkDir.o testDir.o
	gcc -o prog-checkDir checkDir.o testDir.o

tellIfDir.o: testDir.c
	gcc -Wall -c tellIfDir.c

checkDir.o: checkDir.c
	gcc -Wall -c checkDir.c

# Make sonstige Programme
status-prog: statusinfo.c
	gcc -g -Wall -o status-prog statusinfo.c
