/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*====================================================================
 * 
 * Programm zum Kopieren von Dateien mit Memory Mapped I/O.
 * Basierend auf ChatGPT Antwort.
 * 
 * !ACHTUNG!: Unvollstaendige Fehlerabsicherung!
 * 
 * ==================================================================*/

int main(int argc, char *argv[]) 
{
	char *quelldatei = argv[1];
	char *zieldatei = argv[2];
	
	/*===============================================================
	 * Quelldatei Vorbereitung
	 * =============================================================*/
	
	/* File Pointer Quelldatei */
	int fdQuelle;
	if((fdQuelle= open(quelldatei, O_RDONLY)) == -1) 
	{
		perror("open: ");
		exit(EXIT_FAILURE);
	}
	
	/* Struct stat Quelldatei */
	struct stat bufQuelle;
	if(stat(quelldatei, &bufQuelle) == -1) 
	{
		perror("stat: ");
		exit(EXIT_FAILURE);
	}
	
	off_t file_size = bufQuelle.st_size;
	
	/*===============================================================
	 * Zieldatei Vorbereitung
	 * =============================================================*/
	
	/* File Pointer Zieldatei */
	int fdZiel;
	if((fdZiel = open(zieldatei, O_RDWR | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR))  == -1)
	{
		perror("open: ");
		exit(EXIT_FAILURE);
	}
	
	/* Zieldatei auf gleiche Groesse wie Quelldatei festlegen */
	if(ftruncate(fdZiel, file_size) == -1) 
	{
		perror("ftruncate: ");
		exit(EXIT_FAILURE);
	}
	
	/*===============================================================
	 * Abbildung Quell- und Zielbereich mmap
	 * =============================================================*/
	 char *quelldaten = mmap(NULL, file_size, PROT_READ, 
		MAP_PRIVATE, fdQuelle, 0);
	 if(quelldaten == MAP_FAILED)
	 {
		perror("mmap: ");
		exit(EXIT_FAILURE);
	 }
	 
	 char *zieldaten = mmap(NULL, file_size, PROT_WRITE, 
		MAP_SHARED, fdZiel, 0);
	 if(quelldaten == MAP_FAILED)
	 {
		perror("mmap: ");
		exit(EXIT_FAILURE);
	 }
	 
	 /*===============================================================
	 * Kopieren der Daten
	 * =============================================================*/
	 memcpy(zieldaten, quelldaten, file_size);

	/*===============================================================
	 * Freigabe der Ressourcen
	 * =============================================================*/
	 munmap(quelldaten, file_size);
	 munmap(zieldaten, file_size);
	 close(fdQuelle);
	 close(fdZiel);
	 
	 printf("Datei wurde erfolgreich kopiert.\n");

	return EXIT_SUCCESS;
}

