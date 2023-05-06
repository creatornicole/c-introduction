/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> /* to get access to used flags */
#include <sys/stat.h> /* header defines structure of the data returned stat() */

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*===================================================================
 * 
 * Programm zum Kopieren von Dateien
 * 
===================================================================*/

int main(int argc, char *argv[])
{
	/* Definition lokaler Variablen */
	int checkInt; //Kontrollzahl zur Zuhilfenahme in Verzweigungen
	char *buffer; //Zum einzelnen Zeichenauslesen aus Datei
	struct stat buf; //struct zum Auslesen Daten aus i-Node
	char *quelldatei = argv[1]; //Path Quelldatei
	char *zieldatei = argv[2]; //Path Zieldatei
	int fdQuelldatei, fdZieldatei; //Filedeskriptoren der Dateien
	char ch; //Hilfsvariable zum Zwischenspeichern eines Zeichens
	
	/* Ueberpruefung Parameteranzahl */
	if(argc == 3) { /* Korrekte Parameteranzahl */
		
		/* Oeffnen Dateien zur Dateiarbeit */
		fdQuelldatei = open(quelldatei, O_RDONLY); //Hinweis: Nur zum Lesen
		fdZieldatei = open(zieldatei, O_WRONLY); //Hinweis: Nur zum Schreiben
		
		/* Quelldatei nicht existent, Kopiervorgang nicht moeglich */
		if(fdQuelldatei == -1) {
			fprintf(stderr, "Uebergebene Quelldatei existiert nicht. Programm wird beendet...\n");
			exit(EXIT_FAILURE);
		} 
		
		/* struct zum Auslesen i-Node Quelldatei */
		stat(quelldatei, &buf); 
		
		/* Test, ob Quelldatei regulaere Datei ist */
		if((buf.st_mode & S_IFMT) != S_IFREG) { /* Nicht Regulaere Datei */					
			fprintf(stderr, "Error: Quelldatei ist keine regulaere Datei.\n");
			exit(EXIT_FAILURE);	
		}
		
		/* Test, ob Quelldatei Leserechte besitzt */
		if(!buf.st_mode & S_IRUSR) { /* Keine Leserechte fuer Quelldatei */			
			fprintf(stderr, "Error: Sie besitzen keine Leserechte fuer die uebergebene Quelldatei.\n");
			exit(EXIT_FAILURE);	
		}
		
		/* Warnung falls Zieldatei bereits existiert */
		if(fdZieldatei != -1) {
			fprintf(stderr, "Zieldatei existiert bereits. Moechten Sie die bereits existierende Datei ueberschreiben? (j/n) ");
			scanf("%c", &ch);
			
			if(ch == 'j') { /* User-Eingabe 'j' ueberschreibt existierende Zieldatei */
				/* lesen+schreiben, anlegen nicht vorhandener Datei, kuerzen auf Laenge 0 vorhandener Datei */
				/* d.h. ueberschreiben Datei (erzeugen neuer Datei und loeschen Inhalt falls vorhanden*/
				fdZieldatei = open(zieldatei, O_RDWR | O_CREAT | O_TRUNC); 
			} else { /* User-Eingabe != 'j' beendet Programm */
				printf("Zieldatei wird nicht ueberschrieben. Programm wird geschlossen...\n");
				exit(EXIT_SUCCESS);
			}
		} else { /* Zieldatei muss erst noch erstellt werden */
			fdZieldatei = open(zieldatei, O_RDWR | O_CREAT); 
		}
		
		/* Warnung falls Zieldatei nicht geoeffnet bzw. angelegt werden konnte */
		if(fdZieldatei == -1) {
			fprintf(stderr, "Error: Zieldatei konnte nicht geoeffnet bzw. angelegt werden.");
			exit(EXIT_FAILURE);
		} 		
		
		/* Dynamischen Kopierpuffer fuer Kopiervorgang Quelldatei anlegen */
		buffer = malloc(sizeof(char));
		
		/* Dateiinhalt aus Quelldatei in 100 Byte-Bloecken lesen */
		while((checkInt = read(fdQuelldatei, buffer, 100)) != 0) {
			if(checkInt == -1) {
				perror("read: ");
				exit(EXIT_FAILURE);
			}
			buffer[checkInt] = '\0'; //Stringendkennung fuer printf (Nullterminierte char-Arrays)
		}
		
		/* Gelesenen Dateiinhalt in Zieldatei schreiben */
		write(fdZieldatei, buffer, buf.st_size);
		
		/* Zugriffsrechte von Quelldatei auf Zieldatei "uebertragen" */
		chmod(zieldatei, buf.st_mode);
		
	} else { /* Fehlerhafte Parameteranzahl */
		fprintf(stderr, "Fehlerhafte Parameteranzahl. Korrekte Parameteranzahl gleich 3: [copy quelldatei zieldatei]\n");
		exit(EXIT_FAILURE);
	}
	
	/* Benutzte Dateien wieder schliessen */
	if(close(fdQuelldatei) == -1) {
		perror("close: ");
		exit(EXIT_FAILURE);
	}
	
	if(close(fdZieldatei) == -1) {
		perror("close: ");
		exit(EXIT_FAILURE);
	}
	
	printf("Datei %s wurde erfolgreich in %s kopiert.\n", quelldatei, zieldatei);
	return EXIT_SUCCESS;
}
