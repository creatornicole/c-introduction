/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>

/*===================================================================
 * 
 * Programm zur Untersuchung des internen Aufbaus einer Struktur und
 * der binaeren Darstellung von Zahlen in unterschiedlichen
 * Datentypen.
 * 
===================================================================*/

/*===================================================================
 * Zur Analyse der Struktur im Speicher Anzeige des internen Aufbaus
 * inklusive belegter Adressen und zugehoeriger Speicherinhalte:
 * 
 * - Groesse der Struktur und Anfangsadresse bestimmen
 * - Auswertung des zugehoerigen Speichers mit Debugger 
 * 	 (siehe beigefuegter Screenshot)
===================================================================*/

struct test {
	char c;
	int x;
	float z;
};

int main()
{
	/* Initialisierung Variable der struct test*/
	struct test testDaten = { 'z', 1000000, 1000000};
		
	printf("Größe der struct test: %ld\n", sizeof(testDaten));	
	printf("Anfangsadresse der struct: %p\n", &testDaten);
	
	printf("============================\n");
	
	/* Ausgabe Adressen der struct Komponenten zur Speicherauswertung mit Debugger */
	printf("Adresse testDaten.c: %p\n", &testDaten.c);
	printf("Adresse testDaten.x: %p\n", &testDaten.x);
	printf("Adresse testDaten.z: %p\n", &testDaten.z);
	
	return EXIT_SUCCESS;
}
