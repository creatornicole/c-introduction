/* Einfuegen benoetigter Headerdatein in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h> /* for stat usage */
#include <unistd.h> /* use bit values struct stat */
#include <time.h>
#include <fcntl.h> /* to use flags */
#include <pwd.h> /* to use struct passwd */


/* TODO: Eigentlich durch Bib-Einbindung bei Uebersetzung!: */
#include "checkDir.h"

/* Deklaration von Funktionskoepfen */
void printstatusinfo(char *dateiname);
/* Deklaration von globalen Variablen */

/*====================================================================
 * 
 * Wird Verzeichnis uebergeben, so wird Namen und Statusinformationen
 * fuer alle Dateien des verzeichnisses angezeigt.
 * 
 * Wird kein Verzeichnis uebergeben, so erfolgt die Ausgabe einer
 * passenden Hinweisnachricht.
 * 
 * ==================================================================*/
 
/*====================================================================
 * Unter Verwendung libmylib.a, die beim Uebersetzen eingebunden wird.
 * ==================================================================*/

int main(int argc, char *argv[]) 
{	
	/*================================================================
	* Aufrufabsicherung
	* ===============================================================*/
	if(argc != 2) {
		fprintf(stderr, "Fehlerhafte Parameteranzahl. ");
		fprintf(stderr, "Programm erfordert folgenden Aufrufform:\n");
		fprintf(stderr, "./getDirInfo [file]\n");
		exit(EXIT_FAILURE);	
	} 
	
	char *verzeichnisname = argv[1];
	
	/*================================================================
	* Test ob Verzeichnis
	* ===============================================================*/
	if(isdir(verzeichnisname) == 0) {
		printf("Uebergebene Datei %s ist kein Verzeichnis.\n", 
			verzeichnisname);
		exit(EXIT_SUCCESS);
	}
	
	/*================================================================
	* Ausgabe Name und Statusinformationen aller Dateien des
	* Verzeichnisses (wenn Verzeichnis uebergeben wurde)
	* ===============================================================*/
	DIR *dirStream; /* opendir return pointer to directory 
							stream */
	if((dirStream = opendir(verzeichnisname)) == NULL) { 
		perror("opendir: ");
		exit(EXIT_FAILURE);
	}
	
	struct dirent *dirEntry; /* dirent structure represents next 
								directory entry */
	/* readdir returns NULL if error occured or end of dir is reached */
	/* filenames can be read from directory stream with readdir */
	/* readdir returns pointer to dirent structure */	
	while((dirEntry = readdir(dirStream)) != NULL) {
		printstatusinfo(dirEntry->d_name);
	}
	
	/*================================================================
	* Verzeichnis wieder schliessen
	* ===============================================================*/
	closedir(dirStream);

	return EXIT_SUCCESS;
}

void printstatusinfo(char *dateiname) {
	struct stat buf; /* struct zum Auslesen der Statusinformationen der 
						uebergebenen Datei */
	char *buffer; /* zum einzelnen Zeichenauslesen aus Datei */
	char temp[100]; /* zwischenspeichern eingelesene Dateizeilen */
	struct passwd pData;
	struct passwd *pDataResult;
	char passBuf[1024];
	
	/* struct zum Auslesen i-Node Quelldatei */
	if(stat(dateiname, &buf) == -1)
	{
		perror("stat: ");
		exit(EXIT_FAILURE);
	}
	
	/* Ausgabe Statusinformationen der uebergebenen Datei */
	printf("Geraetenummer: %i\n", buf.st_dev);
	printf("i-Node Nummer: %i\n", buf.st_ino);
	printf("Linkzaehler: %i\n", buf.st_nlink); /* Anzahl Hard-Links */
	
	printf("Dateityp: ");
	switch(buf.st_mode & S_IFMT) {
		case S_IFREG: printf("Regulaere Datei\n"); break;
		case S_IFDIR: printf("Verzeichnis\n"); break;
		case S_IFCHR: printf("Charakter Geraet\n"); break;
		case S_IFBLK: printf("Block Datei\n"); break;
		case S_IFIFO: printf("FIFO\n"); break;
		case S_IFLNK: printf("Softlink\n"); break;
		case S_IFSOCK: printf("Socket\n"); break;
		default: printf("Unbekannt\n"); break;
	}
	
	/* Ueberpruefung Zugriffsrechte mit Masken */
	printf("Zugriffsrechte: ");
	printf((S_ISDIR(buf.st_mode)) ? "d" : "-");
	/* Ausgabe Owner Permission */
	printf((buf.st_mode & S_IRUSR) ? "r" : "-");
	printf((buf.st_mode & S_IWUSR) ? "w" : "-");
	printf((buf.st_mode & S_IXUSR) ? "x" : "-");
	/* Ausgabe Group Permission */
	printf((buf.st_mode & S_IRGRP) ? "r" : "-");
	printf((buf.st_mode & S_IWGRP) ? "w" : "-");
	printf((buf.st_mode & S_IXGRP) ? "x" : "-");
	/* Ausgabe Others Permission */
	printf((buf.st_mode & S_IROTH) ? "r" : "-");
	printf((buf.st_mode & S_IWOTH) ? "w" : "-");
	printf((buf.st_mode & S_IXOTH) ? "x" : "-");
	/* Zeilenumbruch */
	printf("\n");
	
	/* UID in Klartext aus struct passwd */
	if(getpwuid_r(buf.st_uid, &pData, passBuf, sizeof passBuf, 
		&pDataResult) != 0) {
		perror("struct passwd: ");
		exit(EXIT_FAILURE);
	}
	printf("Besitzer: %ld, %s, %s\n", (long) buf.st_uid, pData.pw_name,
		pData.pw_gecos);
	printf("Gruppe: %ld\n", (long) buf.st_gid);
	
	printf("Dateigroesse (in Bytes): %ld\n", (long) buf.st_size);

	/* Ausgaben Zeiteintraege in Klartext mit ctime() */
	printf("Letzter Zugriff: %s", ctime(&buf.st_atime));
	printf("Letzte Veraenderung: %s", ctime(&buf.st_mtime));
	printf("Letzter Statuswechsel: %s", ctime(&buf.st_ctime));
}
