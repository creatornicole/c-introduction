/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> /* to get access to used flags */
#include <sys/stat.h> /* for struct stat usage */

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*===================================================================
 * 
 * Programm zum Kopieren von Dateien
 * (ohne Fehlerabsicherung)
 * 
===================================================================*/


int main(int argc, char *argv[])
{
	/* Definition lokaler Variablen */
	int checkInt; //Kontrollzahl zur Zuhilfenahme in Verzweigungen
	char buffer[512]; //Zum einzelnen Zeichenauslesen aus Datei
	struct stat buf; //struct zum Auslesen Daten aus i-Node
	int sizeOfFile; //Hilfsvariable zum Ermitteln einer Dateigroesse
	char *quelldatei = argv[1]; //Path Quelldatei
	char *zieldatei = argv[2]; //Path Zieldatei
	int fdQuelldatei, fdZieldatei; //Filedeskriptoren der Dateien
	char ch; //Hilfsvariable zum Zwischenspeichern eines Zeichens
	
	/* Oeffnen Dateien zur Dateiarbeit */
	fdQuelldatei = open(quelldatei, O_RDONLY); //Hinweis: Nur zum Lesen
	fdZieldatei = open(zieldatei, O_WRONLY); //Hinweis: Nur zum Schreiben
	
	/* Warnung falls Zieldatei bereits existiert */
	if(fdZieldatei != -1) {
		printf("Zieldatei existiert bereits. Moechten Sie die bereits existierende Datei ueberschreiben? (j/n) ");
		scanf("%c", &ch);
		
		if(ch == 'j') { /* User-Eingabe 'j' ueberschreibt existierende Zieldatei */
			/* lesen+schreiben, anlegen nicht vorhandener Datei, kuerzen auf Laenge 0 vorhandener Datei */
			/* d.h. ueberschreiben Datei (erzeugen neuer Datei und loeschen Inhalt falls vorhanden*/
			fdZieldatei = open(zieldatei, O_RDWR | O_CREAT | O_TRUNC); 
		} else { /* User-Eingabe != 'j' beendet Programm */
			printf("Zieldatei wird nicht ueberschrieben. Programm wird geschlossen...");
			exit(EXIT_SUCCESS);
		}
	} else {
		fdZieldatei = open(zieldatei, O_RDWR | O_CREAT); 
	}
	
	/* Dateiinhalt aus Quelldatei in 100 Byte-Bloecken lesen */
	while((checkInt = read(fdQuelldatei, buffer, 100)) != 0) {
		if(checkInt == -1) {
			perror("read: ");
			exit(EXIT_FAILURE);
		}
		buffer[checkInt] = '\0'; //Stringendkennung fuer printf (Nullterminierte char-Arrays)
	}
	
	/* Quelldateigroesse fuer Schreiboperation bestimmen */
	stat(quelldatei, &buf);
	sizeOfFile = buf.st_size;
	
	/* Gelesenen Dateiinhalt in Zieldatei schreiben */
	write(fdZieldatei, buffer, sizeOfFile);
	
	/* Zugriffsrechte von Quelldatei auf Zieldatei "uebertragen" */
	chmod(zieldatei, buf.st_mode);
}
