/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Deklaration von Funktionskoepfen */
void sortiereZeigerfeld(char **pointer, int anzahl);
void zeigeZeigerfeld(int **pointer, int anzahl); 

/* Deklaration von globalen Variablen */
int i, j; /* Hilfsvariablen fuer Schleifendurchlaeufe */

/*===================================================================
 * 
 * Programm, welches seine eigenen Kommandozeilenparameter auswertet
 * 
===================================================================*/

/*===================================================================
 * Umfang:
 * Anzahl der Parameter, die dem Programmnamen folgen, ist auszugeben
 * Parameter werden in aufsteigender Laenge sortiert
===================================================================*/

int main(int argc, char *argv[])
{
	/* Ueberpruefung der Parameteruebergabe */
	if(argc == 1)
	{
		/* Keine Kommandozeilenparameter wurden uebergeben */	
		fprintf(stderr, "Geben Sie mindestens einen Kommandozeilenparameter ein.\n");
		return EXIT_FAILURE;
	}
	else
	{
		/* Ausgabe Anzahl uebergebener Parameter */
		printf("Anzahl übergebener Parameter: %d\n", argc-1);
		printf("=============================================\n");
		printf("Sortierung übergebener Parameter in aufsteigender Länge:\n");
		
		/* Dynamische Zeigerfelderzeugung fuer Adressen der Aufrufparameter */
		char **pointerArray = (char**) malloc(argc * sizeof(char*));
		
		/* Uebergabe Zeiger der Kommandozeilenparameter in Zeigerfeld */
		for(i = 0; i < argc; i++)
		{
			pointerArray[i] = argv[i];
		}
		
		/* Aufruf Hilfsfunktionen zur Sortierung und
		 * Ausgabe des uebergebenen Feldes */
		sortiereZeigerfeld(pointerArray, argc);
		zeigeZeigerfeld(pointerArray, argc); 
		
		/* Eingabepuffer wird wieder geleert */
		free(pointerArray);
		
		return EXIT_SUCCESS;
	}
}

/* 
 * Sortierung des Zeigerfeldes auf String-Elemente mittels Bubble Sort
 * 
 * Parameter:
 * Zeigerfeld auf String-Elemente, Laenge des Feldes
*/
void sortiereZeigerfeld(char **pointer, int anzahl)
{
	char *tmp;
	
	for(i = 1; i < anzahl-1; i++)
	{
		for(j = i+1; j < anzahl; j++)
		{
			if(strlen(pointer[i]) > strlen(pointer[j])) 
			{
				tmp = pointer[i];
				pointer[i] = pointer[j];
				pointer[j] = tmp;
			} 
		}
	}
}

/* 
 * Hilfsfunktion zur Ausgabe eines Zeigerfeldes
 * 
 * Parameter:
 * Zeigerfeld auf String-Elemente, Laenge des Feldes
*/
void zeigeZeigerfeld(int **pointer, int anzahl) 
{	
	printf("\nIndex \t Parameter \t Länge\n");
	printf("=============================================\n");
	
	for(i = 1; i < anzahl; i++) 
	{
		printf("%d \t %s \t %9d\n", i, pointer[i], strlen(pointer[i]));		
		
	}
}



