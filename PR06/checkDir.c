/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* for stat usage */

#include "checkDir.h"

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*===================================================================
 * 
 * Funktion int isdir(char *dateiname), die mit Hilfe von stat() 
 * ueberprueft, ob eine uebergegebene Datei ein Verzeichnis ist und
 * einen entsprechenden Wahrheitswert zurückgibt.
 * 
===================================================================*/

/*===================================================================
 * return-Values:
 * 
 * return value == 1 -> dateiname ist Verzeichnis
 * return value == 0 -> dateiname ist kein Verzeichnis
===================================================================*/

int isdir(char *dateiname) {
	/* Vorbereiten Auslesen Dateityp aus struct stat */
	struct stat buf;
	
	if(stat(dateiname, &buf) == -1) {
		perror("struct stat: ");
		exit(EXIT_FAILURE);
	}
	
	/* Testet ob dateiname Verzeichnis ist */
	if((buf.st_mode & S_IFMT) == S_IFDIR) {
		return 1;
	} else {
		return 0;
	}
}
