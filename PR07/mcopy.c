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
#define LEN 120

/*====================================================================
 * 
 * Programm zum Kopieren von Dateien mit Memory Mapped I/O
 * 
 * ==================================================================*/

int main(int argc, char *argv[]) 
{
	/*================================================================
	 * Definition lokaler haeufig verwendeter  Variablen
	 *===============================================================*/
	char *quelldatei = argv[1]; /* Quelldatei */
	char *zieldatei = argv[2]; /* Zieldatei */
		
	int fdQuelldatei; /* File-Pointer Quelldatei */
	int fdZieldatei; /* File-Pointer Zieldatei */
	
	
	/*================================================================
	 * Absicherung Programmaufruf hinsichtlich Quelldatei
	 *===============================================================*/
	/* Ueberpruefung Parameteranzahl */
	if(argc != 3) {
		fprintf(stderr, "Fehlerhafte Parameteranzahl. Korrekte ");
		fprintf(stderr, "Parameteranzahl gleich 3: ");
		fprintf(stderr, "[mcopy quelldatei zieldatei]\n");
		exit(EXIT_FAILURE);
	}
	
	/* Ueberpruefung Existenz Quelldatei */
	if((fdQuelldatei = open(quelldatei, O_RDONLY)) == -1) {
		perror("open: ");
		exit(EXIT_FAILURE);
	}
	
	/* struct zum Auslesen i-Node Quelltext */
	struct stat buf;
	if(stat(quelldatei, &buf) == -1) {
		perror("stat: ");
		exit(EXIT_FAILURE);
	}
	
	/* Test, ob Quelldatei regulaere Datei ist */
	if((buf.st_mode & S_IFMT) != S_IFREG) {
		fprintf(stderr, "Error: Quelldatei ist keine regulaere ");
		fprintf(stderr, "Datei.\n");
		exit(EXIT_FAILURE);
	}
	
	/* Test, ob Quelldatei Leserechte besitzt */
	if((!buf.st_mode) & S_IRUSR) {
		fprintf(stderr, "Error: Sie besitzen keine Leserechte fuer ");
		fprintf(stderr, "die uebergebene Quelldatei.\n");
		exit(EXIT_FAILURE);
	}
	
	/*================================================================
	 * Absicherung Programmaufruf hinsichtlich Zieldatei
	 *===============================================================*/
	/* Warnung, falls Zieldatei bereits existiert */
	char ch; /* Hilfsvariable zum Zwischenspeichern eines Zeichens */
	if((fdZieldatei = open(zieldatei, O_WRONLY)) != -1) {
		/* Zieldatei existiert bereits */
		printf("Zieldatei existiert bereits. Moechten Sie die ");
		printf("bereits existierende Datei ueberschreiben?\n");
		
		scanf("%c", &ch);
		
		if(ch == 'j') {
			/* Zieldatei wird ueberschrieben */
		if((fdZieldatei = open(zieldatei, O_RDWR | O_CREAT 
			| O_TRUNC)) == -1) {
			perror("open: ");
			exit(EXIT_FAILURE);
		} 
		
		} else {
			/* User-Eingabe != j beendet Programm */
			printf("Zieldatei wird nicht ueberschrieben. ");
			printf("Programm wird geschlossen...\n");
			exit(EXIT_SUCCESS);
		}
		
	} else {
		/* Zieldatei muss erst noch erstellt werden */
		if((fdZieldatei = open(zieldatei, O_RDWR | O_CREAT)) == -1) {
			perror("open: ");
			exit(EXIT_FAILURE);
		} 
	}
	
	/*================================================================
	 * Quelldatei Vorbereitung
	 *===============================================================*/
	off_t file_size_quelle = buf.st_size;
	
	/*================================================================
	 * Zieldatei Vorbereitung
	 *===============================================================*/
	/* Zieldatei auf gleiche Groesse wie Quelldatei festlegen */
	if(ftruncate(fdZieldatei, file_size_quelle) == -1) {
		perror("ftruncate: ");
		exit(EXIT_FAILURE);
	}
	
	/*================================================================
	 * Abbildung Quell- und Zielbereich mmap
	 *===============================================================*/
	char *srcMap = mmap(0, file_size_quelle, PROT_READ, 
		MAP_PRIVATE, fdQuelldatei, 0);
		
	if(srcMap == MAP_FAILED){
		perror("mmap: ");
		exit(EXIT_FAILURE);
	}
	
	char *destMap = mmap(0, file_size_quelle, PROT_WRITE, 
		MAP_SHARED, fdZieldatei, 0);
		
	if(destMap == MAP_FAILED){
		perror("mmap: ");
		exit(EXIT_FAILURE);
	}
	
	/*================================================================
	 * Kopieren der Daten
	 *===============================================================*/
	memcpy(destMap, srcMap, file_size_quelle);
	
	/*================================================================
	 * Freigabe der Ressourcen
	 *===============================================================*/
	/* Mapping aufheben */
	if(munmap(srcMap, file_size_quelle) < 0) {
		perror("munmap: ");
		exit(EXIT_FAILURE);
	}
	
	if(munmap(destMap, file_size_quelle) < 0) {
		perror("munmap: ");
		exit(EXIT_FAILURE);
	}
	
	/* Quelldatei schließen */
	if(close(fdQuelldatei)) {
		perror("close: ");
		exit(EXIT_FAILURE);
	}
	/* Zieldatei schließen */
	if(close(fdZieldatei)) {
		perror("close: ");
		exit(EXIT_FAILURE);
	}
	
	printf("Quelldatei %s erfolgreich in Zieldatei %s kopiert.\n",
		quelldatei, zieldatei);

	return EXIT_SUCCESS;
}
