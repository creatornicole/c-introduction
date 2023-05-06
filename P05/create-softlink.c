/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> /* to get access to used flags */
#include <sys/stat.h> /* for struct stat usage */
#include <unistd.h> /* for symlink() usage */

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */
char *file;
char softlink[100]; /* char *softlink not working */

/*===================================================================
 * 
 * Programm zum Erzeugen eines Softlinks der uebergebenen Datei
 * 
===================================================================*/

/*===================================================================
 * argv[0] Aufruf Programm
 * argv[1] ist Datei fuer die Softlink erstellt werden soll
 * argv[2] ggf Path zu Softlink
 * 
 * if argv[2] == NULL, so wird automatisch Name fuer Softlink erstellt
 * zudem Softlink dann im aktuellen Verzeichnis
 * 
 * jegliche andere Parameteranzahl fuehrt zu Fehlergenerierung und
 * Beendigung des Programms
===================================================================*/


int main(int argc, char *argv[])
{
	if((argc > 1) && (argc < 4)) 
	{
		file = argv[1];
		
		if(argc == 2) 
		{
			/* Namen fuer Softlink erstellen */
			char *str1 = "softlink-";
			char *str2 = file;
			
			strcpy(softlink, str1); /* kopiere str1 in softlink */
			strcat(softlink, str2); /* fuege str2 an softlink an */
		}
		
		if(argc == 3)
		{
			/* Dritter Aufrufparameter wird Name fuer Softlink */
			strcpy(softlink, argv[2]);
		}
		
		/* Softlink von file mit uebergebenen oder automatisch erzeugten Namen erstellen */
		symlink(file, softlink);
	} 
	else 
	{
		fprintf(stderr, "Fehlerhafte Parameteranzahl.\n");
		printf("./create-softlink [file] [softlink] oder ./create-softlink [file]\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Softlink wurde erstellt: %s -> %s\n", softlink, file);
	return EXIT_SUCCESS;
}
