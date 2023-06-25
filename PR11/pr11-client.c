/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */
#define CLIENT_SOCKET "/tmp/mysocket"
#define SOCKNAME_LEN 20

/*=====================================================================
 * 
 * Client-Server-Kopiersystem unter Verwendung lokaler Streamsockets
 * 
 * ===================================================================*/
 
 /*=====================================================================
 * 
 * Client: Liest Daten aus Quelldatei
 * 
 * Parameteruebergabe: Quell- und Zieldatei
 * Uebergabe an Server: Name der Zieldatei (auch Daten?)
 * 
 * ===================================================================*/

int main(int argc, char *argv[]) 
{
	/* Abfangen falscher Parameteranzahl */
	if(argc != 3) {
		fprintf(stderr, "Falsche Paramteruebergabe.\n");
		fprintf(stderr, "Erforderliche Aufrufsform: ");
		fprintf(stderr, "[Prog] [Quelldatei] [Zieldatei]\n");
		
		exit(EXIT_FAILURE);
	}
	
	/* Client erhaelt Quell- und Zieldatei als Kommandozeilenparams */
	char *quelldatei = argv[1];
	char *zieldatei = argv[2];
	
	/* Client legt Sockt an */
	/* Return Socketdeskriptor oder -1 im Fehlerfall */
	int sockfd;
	/* PF_LOCAL - Protokollfamilie fuer lokale Streamsockets */
	/* SOCK_DGRAM - Sockettyp zur Uebertragung von Datenpaketen */
	/* 0 - Protokoll entsprechend Typ, Familie */
	if((sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0)) == -1) {
		perror("socket: ");
		exit(EXIT_FAILURE);
	}
	
	/* Adresse des Clients erzeugen */
	struct sockaddr_un, client_addr;
	
	/* Adressstruktur fuer lokale Kommunikation entsprechend */
	client_addr.sun_family = AF_LOCAL;
	
	/* snprintf schreibt die formatierte Zeichenkette in den Puffer
		'str' und gibt die Anzahl der Zeichen zurueck, die
		geschrieben wurden (ohne abschliessende Null-Zeichen) 
		oder die Anzahl, die geschrieben worden waere, wenn der Puffer
		ausreichend gross gewesen waere */
	
	/* Zeiger auf Puffer, in den formatierte Zeichenkette geschrieben
		werden soll */
	/* Maximale Anzahl von Zeichen, die in Puffer geschrieben werden
		sollen, inkl. abschliessenden Null-Zeichen */
	/* Zeichenkette, die Format der Ausgabe angibt */
	/* Optionale Argumente, die in Format der Ausgabe eingefuegt
		werden (hier: string placeholder and decimal integer with
		a width of 5 digits, padded with zeros if necessary */
		
	/* Vermeidet zeitgleich Pufferueberlauf da Groesse des Puffers 
		explizit angegeben ist */
		
	snprintf(client_addr.sun_path,
			 sizeof(client_addr.sun_path) - 1,
			 "%s%05d",
			 CLIENT_SOCKET, getpid());
	
	addrlen = strlen(client_addr.sun_path) 
				+ offsetof(struct sockaddr_un, sun_path);
				
	char client_sock_name[SOCKNAME_LEN];
	strncpy(client_sock_name, client_addr.sun_path, SOCKNAME_LEN-1);	
	
	/* Client bindet angelegten Socket an eigene Adresse */
	
	/* bind() erfordern Schreibrechte auf das Verzeichnis! */
	
	/* Return 0 bei Erfolg, -1 im Fehlerfall */
	if((bind(sockfd, (struct sockaddr *)&client_addr, addrlen)) == -1) {
		perror("bind: ");
		exit(EXIT_FAILURE);
	}
	
	/* Zugriff auf lokale Sockets wird durch Zugriffsrechte geregelt */
	/* Rechte werden von umask abgeleitet */
	/* Jeder Nutzer soll Socket verwenden koennen */
	if(chmod(client_addr.sun_path, 0777) == -1) {
		perror("chmod: ");
		exit(EXIT_FAILURE);
	}
	
	/* Client liest Datei aus Quelldatei */
	
	/* Schreibt, sendet ausgelesene Datei und Name der Zieldatei */
	
	/* Aus Socket lesen, empfangen */
	/* recvfrom() blockiert bis Datagramme eintreffen ?! */
	
	
	

	
	return EXIT_SUCCESS;				
}
