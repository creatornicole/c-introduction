/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* header defines structure of the data returned stat() */
#include <linux/limits.h> /* header for PATH_MAX */

#include "softlink.h"

/* Deklaration von Funktionskoepfen */
char *isSoftlink(char *dateiname);
/* Deklaration von globalen Variablen */

/*===================================================================
 * 
 * Hilfsfunktion zum Testen, ob eine Datei ein Softlink ist.
 * 
===================================================================*/

/*
 * Testet, ob uebergebene Dateiname ein Softlink ist.
 */
char *isSoftlink(char *dateiname) 
{
	char *buffer = malloc(PATH_MAX);
	size_t link_length;
	
	if((link_length = readlink(dateiname, buffer, PATH_MAX)) == -1) 
	/* PATH_MAX = im verwendeten System maximal moegliche Pfadlaenge
	 * in unserem Fall fuer Puffergroesse ausreichend */
	{
		/* Datei kein Softlink */
		return NULL;
	} 
	else 
	{
		/* Datei ist Softlink */
		buffer[link_length] = '\0'; /* Nullterminiertheit der Zeichenkette erforderlich */
		static char fileOfSoftlink[PATH_MAX];
		strcpy(fileOfSoftlink, buffer); /* buffer beinhaltet Dateinamen zu Softlink */
		
		return fileOfSoftlink; /* Rueckgabe Adresse */
	}
}
