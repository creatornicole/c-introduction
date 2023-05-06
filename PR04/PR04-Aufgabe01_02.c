/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>

/* Deklaration von Funktionskoepfen */
void zeigeFeld(int *feld, int anzahl);
void sortiereFeldAbsteigend(int **pointer, int anzahl); 
void zeigeZeigerfeld(int **pointer, int anzahl); 

/*===================================================================
 * 
 * Programm legt Feld von N int-Zahlen an und zeigt dieses an.
 * 
===================================================================*/

/*===================================================================
 * Umfasst:
 * Anzahl N Uebergabe als Parameter auf der Kommandozeile
 * Dynamische Erzeugung des Feldes
 * Initialisierung mit Zufallszahlen
 * Hilfsfunktion zur Anzeige des Feldes
 * Mit Erweiterung, dass Feld nach der Anzeige in absteigender Folge
 *  sortiert nochmals ausgegeben wird
===================================================================*/



int main(int argc, char *argv[])
{	
	int i;			/* Zaehlvariable fuer for-Schleifen */
	time_t t;		/* zur Initialisierung rand() zur Zufallszahlenerzeugung */

	/* Ueberpruefung der Parameteruebergabe */
	if(argc == 1) {
		/* Keine Kommandozeilenparameter wurden uebergeben */		
		printf("Es wurden keine Kommandozeilenparameter übergeben.\n");	
	} else {
		/* Umwandlung Kommandozeilenparameter zu int */
		int anzahl = atoi(argv[1]);
		
		if(anzahl <= 0) 
		{
			/* Kommandozeilenparameter <= 0 oder nicht von Typ int */
			fprintf(stderr, "Es werden keine Zahlen generiert. Tätige dafür eine Eingabe > 0.\n");
			return EXIT_FAILURE;
		} 
		else 
		{
			/* Kommandozeilenparameter > 0 und somit gueltig */
			printf("Es werden %d Zahlen generiert...\n", anzahl);
			
			/* Dynamische Felderzeugung */
			int *numArray = (int*) malloc(anzahl * sizeof(int)); //Adresse von int
			/* Dynamische Zeigerfelderzeugung */
			int **pointerArray = (int**) malloc(anzahl * sizeof(int*)); //Adresse einer integer-Adresse
			
			/* Initialisierung Random Number Generator */
			srand((unsigned) time(&t));
			
			/* Initialisierung int-Feldes mit Zufallszahlen */
			for(int i = 0; i < anzahl; i++) 
			{
				numArray[i] = rand();
			}
			
			/* Initialisierung des Zeigerfeldes mit Zeigern auf Elementen 
			 * des int-Feldes */
			for(int i = 0; i < anzahl; i++)
			{
				pointerArray[i] = &numArray[i]; 
			}
			
			/* Aufruf Hilfsfunktionen zur Anzeige, Sortierung und 
			 * sortierten Anzeige des Feldes*/
			zeigeFeld(numArray, anzahl);
			sortiereFeldAbsteigend(pointerArray, anzahl);	
			zeigeZeigerfeld(pointerArray, anzahl);
			
			/* Eingabepuffer wird wieder geleert */
			free(numArray);
			free(pointerArray);
			
			return EXIT_SUCCESS;
		}
	} 
	
}

/* 
 * Hilfsfunktion zur Ausgabe eines int-Arrays
 * 
 * Parameter:
 * Anfangsadresse des Feldes, Laenge des Feldes
*/
void zeigeFeld(int *feld, int anzahl) 
{
	printf("\nIndex \t Wert\n");
	printf("====================\n");
	
	for(int i = 0; i < anzahl; i++) 
	{
		printf("%d \t %d\n", i+1, feld[i]);		
	}
}

/* 
 * Sortierung des Zeigerfeldes auf die int-Elemente mittels Bubble Sort
 * 
 * Parameter:
 * Zeigerfeld auf int-Elemente, Laenge des Feldes
*/
void sortiereFeldAbsteigend(int **pointer, int anzahl) 
{	
	int i, j;
	int *tmp;
	
	for(i = 0; i < anzahl-1; i++)
	{
		for(j = i+1; j < anzahl; j++)
		{
			if (*pointer[i] < *pointer[j])
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
 * Zeigerfeld auf int-Elemente, Laenge des Feldes
*/
void zeigeZeigerfeld(int **pointer, int anzahl) 
{
	int i;
	
	printf("\nIndex \t Adresse \t Wert\n");
	printf("=============================================\n");
	
	for(i = 0; i < anzahl; i++) 
	{
		printf("%d \t %p \t %d\n", i+1, pointer[i], *pointer[i]);		
	}
}


