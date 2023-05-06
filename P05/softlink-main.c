/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Einfuegen benoetigter Funktion isSoftlink() */
#include "softlink.h"

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */
char *paramFile;
char *fileOfSoftlink;

/*===================================================================
 * 
 * Programm zum Testen und Ausgabe, ob eine Datei ein Softlink ist.
 * 
===================================================================*/

int main(int argc, char *argv[])
{	
	if(argc == 2) 
	{
		paramFile = argv[1];
		fileOfSoftlink = isSoftlink(paramFile); /* Testen, ob Param Softlink ist */
		
		if(fileOfSoftlink != NULL) 
		{
			/* Uebergebene Datei ist Softlink */
			printf("Uebergebene Datei ist Softlink zu %s. %s -> %s\n", fileOfSoftlink, paramFile, fileOfSoftlink);
			printf("Zieldatei: %s\n", fileOfSoftlink);
		} 
		else
		{
			/* Uebergebene Datei ist kein Softlink */
			fprintf(stderr, "Uebergebene Datei ist kein Softlink. %s -> /\n", paramFile);
		} 
		
		return EXIT_SUCCESS;
	} 
	else 
	{
		fprintf(stderr, "Fehlerhafte Parameteranzahl. ./softlink-main.c [file] erforderlich.\n");
		return EXIT_FAILURE;
	}
	
}

