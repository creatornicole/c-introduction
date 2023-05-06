/* Einfuegen zugehoeriger Headerdatei */
#include "flaeche.h" 

/*===================================================================
 * Programmteil zur Berechnung am Kreis mit Funktion kreisFlaeche()
===================================================================*/

/* Hilfsvariable zum Zaehlen der Aufrufe pro Runde */
int anzahl = 1;

/* Definition der Funktion kreisFlaeche() */
double kreisFlaeche(double radius)
{
	anzahl++;
	return PI * radius * radius;
}

