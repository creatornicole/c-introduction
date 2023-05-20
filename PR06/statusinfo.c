/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* for stat usage */
#include <unistd.h> /* use bit values struct stat */
#include <time.h>
#include <fcntl.h> /* to use flags */
#include <pwd.h> /* to use struct passwd */

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*===================================================================
 * 
 * Programm zur Ausgabe aller Statusinformationen einer
 * uebergebenen Datei im Klartext
 * 
===================================================================*/

int main(int argc, char *argv[])
{	
	struct stat buf; /* struct zum Auslesen der Statusinformationen der 
						uebergebenen Datei */
	char *buffer; /* zum einzelnen Zeichenauslesen aus Datei */
	char temp[100]; /* zwischenspeichern eingelesene Dateizeilen */
	struct passwd pData;
	struct passwd *pDataResult;
	char passBuf[1024];
	
	/* struct zum Auslesen i-Node Quelldatei */
	if(stat(argv[1], &buf) == -1)
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
	
	return EXIT_SUCCESS;
}

