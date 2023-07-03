/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */
#define BUFFER_SIZE 1024
#define SOCKET_PATH "/tmp/mysocket"
#define MSG_SIZE 30

/*=====================================================================
 * 
 * Client-Server Anwendung zum Kopieren beliebiger Dateien - Client
 * (Annahme: Server wird vor Client gestartet)
 * 
 * ===================================================================*/
 
 /*=====================================================================
 * 
 * - liest Daten aus einer Quelldatei
 * - werden beim Aufruf Namen von Quell- und Zieldatei als
 * 	 Kommandozeilenparameter mit gegeben
 * 
 * ===================================================================*/
 
 int main(int argc, char *argv[]) 
 {
	 /*=================================================================
	 * Variablendeklaration
	 * ===============================================================*/
	 int sockfd;
	 struct sockaddr_un server_addr;
	 
	 /*=================================================================
	 * Vorbereitung
	 * ===============================================================*/
	 /* Abfangen falscher Parameteranzahl */
	 if(argc != 3) 
	 {
		fprintf(stderr, "Falsche Parameteruebergabe.\n");
		fprintf(stderr, "Erforderliche Aufrufsform: ");
		fprintf(stderr, "[Prog] [Quelldatei] [Zieldatei]\n");
		
		exit(EXIT_FAILURE);
	 }
	 
	 /* Client erhaelt Quell- und Zieldatei als Kommandozeilenparam */
	 char *quelldateiname = argv[1];
	 char *zieldateiname = argv[2];
	 
	 /*=================================================================
	 * Daten senden - Vorbereitung Inhalt Quelldatei
	 * ===============================================================*/
	 /* Quelldateiarbeit vorbereiten */
	 FILE *fp = fopen(quelldateiname, "rb");
	 if(fp == NULL)
	 {
		perror("fopen: ");
		exit(EXIT_FAILURE);
	 }
	 
	 /*=================================================================
	 * Adressen vorbereiten
	 * ===============================================================*/
	 /* Zieladresse des Servers vorbereiten */
	 
	 /* Speicherplatz mit Konstante fuellen 
	    Fuellt sizeof(server_addr) Bytes in ersten Bytes des
	    Speicherbereichs der server_addr mit Konstante 0 */
	 memset(&server_addr, 0, sizeof(server_addr));
	 server_addr.sun_family = AF_UNIX;
	 strncpy(server_addr.sun_path, SOCKET_PATH, 
		sizeof(server_addr.sun_path) - 1);
	
	/*=================================================================
	 * Verbindung herstellen
	 * ===============================================================*/
	/* Socket anlegen */
	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket: ");
		exit(EXIT_FAILURE);
	}
	
	/* Mit Server verbinden */
	if(connect(sockfd, (struct sockaddr*)&server_addr,
		sizeof(server_addr)) == -1)
	{
		perror("connect: ");
		exit(EXIT_FAILURE);
	}
	
	/*=================================================================
	 * Daten senden - Zieldateiname
	 * ===============================================================*/
	/* Zieldateinamen an Serveradresse senden */
	if(send(sockfd, zieldateiname, strlen(zieldateiname) + 1, 0) == -1)
	{
		perror("write: ");
		exit(EXIT_FAILURE);
	}
	
	/*=================================================================
	 * Rueckmeldung bezueglich Zieldatei von Server erhalten
	 * ===============================================================*/
	char status_msg[MSG_SIZE];
	
	if(recv(sockfd, status_msg, strlen(status_msg) - 1, 0) == -1)
	{
		perror("read: ");
		exit(EXIT_FAILURE);
	}
	
	printf("Uebermittlung Zieldateiname: %s\n", status_msg);
	
	if(strcmp(status_msg, "FILEEXISTS") == 0)
	{
		fclose(fp);
		close(sockfd),
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(status_msg, "TOOLONGFILENAME") == 0)
	{
		fclose(fp);
		close(sockfd),
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(status_msg, "FILEERROR") == 0)
	{
		fclose(fp);
		close(sockfd),
		exit(EXIT_FAILURE);
	}
	
	/*=================================================================
	 * Daten senden - Inhalt Quelldatei
	 * ===============================================================*/
	/* Daten aus Quelldatei an Server senden */
	char buffer[BUFFER_SIZE]; /* Puffer zum Daten aus Quelldatei 
									lesen */
	int bytes_read;	
	while((bytes_read = fread(buffer, sizeof(char), 
		sizeof(buffer), fp)) > 0)
	{
		/* Dateiinhalt an Server senden */
		if(send(sockfd, buffer, bytes_read, 0) == -1)
		{
			perror("write: ");
			exit(EXIT_FAILURE);
		}
		
		/* Test, ob erwartete Bytes gelesen wurden */
		if(bytes_read < sizeof(buffer))
		{
			if(feof(fp))
			{
				printf("Dateiende erreicht.\n");
			}
			
			if(ferror(fp))
			{
				printf("Datei konnte nicht gelesen werden.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	/*=================================================================
	 * Nachbereitung
	 * ===============================================================*/
	/* Datei schliessen */
	if(fclose(fp) == -1)
	{
		perror("fclose: ");
		exit(EXIT_FAILURE);
	}
	/* Socket wieder schliessen */
	if(close(sockfd) == -1)
	{
		perror("close: ");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
 
 }
