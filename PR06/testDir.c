/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* for stat usage */

#include "libmylib.h"

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*===================================================================
 * 
 * Testprogramm, welches einen kurzen Text darueber ausgibt, ob eine
 * Datei ein Verzeichnis ist oder nicht.
 * 
===================================================================*/

int main(int argc, char *argv[])
{
	/*================================================================
	 * Aufrufabsicherung
	 * ==============================================================*/
	if(argc != 2) 
	{
		fprintf(stderr, "Fehlerhafte Parameteranzahl. ");
		fprintf(stderr, "Programm erfordert folgende Aufrufform:\n");
		fprintf(stderr, "./testDir [file] \n");
		exit(EXIT_FAILURE);
	}
	
	if(isdir(argv[1]) == 0) 
	{
		printf("Uebergebene Datei %s ist kein Verzeichnis.\n", argv[1]);
	} else 
	{
		printf("Uebergebene Datei %s ist Verzeichnis.\n", argv[1]);
	}
	
	
	return EXIT_SUCCESS;
}
