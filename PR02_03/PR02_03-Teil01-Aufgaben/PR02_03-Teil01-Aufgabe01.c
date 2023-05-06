/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>

/*===================================================================
 * 
 * Programm nimmt Zeichen von der Tastatur entgegen und gibt
 * dieses in mehreren Ausgabeformaten wieder aus.
 * 
 * Nach Eingabe von Q wird das Programm beendet.
 * 
===================================================================*/

/*===================================================================
 * Anforderung an Ausgabeformate:
 * als Zeichen, als vorzeichenlose Integerzahl, als Hexadezimalzahl
 * 
 * Erweiterung der Ausgabe:
 * Abaenderung Formatangabe, sodass Ausgabe spaltenweise untereinander erfolgt
===================================================================*/

int main()
{
	char l; /* Lokale Variable fuer Zeichenentgegennahme */
	
	do {
		/* Entgegennahme Zeichen von Tastatur */
		printf("Geben Sie ein Zeichen ein: ");
		scanf(" %c", &l); 
				
		/* Spaltenweise Ausgabe des eingegebenen Zeichens in verschiedenen Formaten */	
		printf("Zeichen char int hex\n");
		printf("%c %7c %6i %#3x\n", l, l, l, l);
		
		printf("==================================================\n");
		
		/* Moeglichkeit der Beendung des Programmes */
		printf("Nochmal? (Q = Nein, alle anderen Eingaben = ja): ");
		scanf(" %c", &l);

	}while(l != 'Q');
	
	exit(EXIT_SUCCESS);
}
