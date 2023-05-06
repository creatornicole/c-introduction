/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
/* Einfuegen benoetigter Funktion kreisFlaeche() */
#include "flaeche.h"

/* Definition globaler Konstanten */
#define PI 3.1415926

/*===================================================================
 * Programm zur Berechnung am Kreis
===================================================================*/

/*===================================================================
 * Programmanforderungen:
 * Funktion kreisFlaeche in eigener Quelldatei flaeche.c
===================================================================*/

int main()
{
	/* Definition lokaler Variablen */
	double r, u, flaeche;
	char jn;
	int erg;
	
	/* Anweisungsteil */
	do {
		printf("Runde: %d\n", anzahl);
		printf("Geben Sie den Radius ein: ");
		erg = scanf("%lg", &r); //TODO: erg sollte ueberprueft werden!
		
		u = 2 * PI * r;
		flaeche = kreisFlaeche(r);
		
		printf("Umfang: %lg Flaeche: %lg \n", u, flaeche);
		printf("Nochmal (j/n)? ");
		
		erg = scanf(" %c", &jn);
		
	} while(jn == 'j');
	
	return EXIT_SUCCESS; /* EXIT_SUCCESS (0) alles ok */
}





