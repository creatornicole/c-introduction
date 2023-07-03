/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */
#define BUFFER_SIZE 1024
#define SOCKET_PATH "/tmp/mysocket"
#define MSG_SIZE 30

/* Global wegen Signalhandler */
int sockfd;

/* Schliessen nach Fehlern */
/* Server-Socket und zugehoerige Eintrag im Dateisystem loeschen */
void exithandler()
{
	if(close(sockfd) == -1)
	{
		perror("close: ");
		_exit(EXIT_FAILURE);
	}
	
	if(unlink(SOCKET_PATH) == -1)
	{
		perror("unlink: ");
		_exit(EXIT_FAILURE);
	}
	
	return;
}

/* Abfangen SIGINT ueber geeigneten Signalhandler */
void sighandler(int signo)
{
	exithandler();
	_exit(EXIT_SUCCESS);
}

/*=====================================================================
 * 
 * Client-Server Anwendung zum Kopieren beliebiger Dateien - Server
 * (Annahme: Server wird vor Client gestartet)
 * 
 * ===================================================================*/
 
 /*=====================================================================
 * 
 * - Server schreibt Daten in Zieldatei
 * - erhaelt Name der Zieldatei von Client uebermittelt
 * - iterativer Server, der mit Socket /tmp/socket arbeitet 
 * 
 * ===================================================================*/
 
 int main(int argc, char *argv[]) 
 {
	/*=================================================================
	 * Variablendeklaration
	 * ===============================================================*/
	struct sockaddr_un server_addr, client_addr;
	socklen_t clientaddrlen;
	struct sigaction old, new;
	int commsockfd; /* Socket fuer Kommunikation zum Reinschreiben */
	
	/*=================================================================
	 * Adressen vorbereiten
	 * ===============================================================*/
	/* Serveradresse vorbereiten */
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_LOCAL;
	strncpy(server_addr.sun_path, SOCKET_PATH, 
		sizeof(server_addr.sun_path) -1);
	
	/*=================================================================
	 * Verbindung herstellen
	 * ===============================================================*/
	/* Socket anlegen */
	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket: ");
		exit(EXIT_FAILURE);
	}
	
	/* Pruefen, ob SOCKET_PATH bereits existiert */
	/* Entfernen wenn ja, sonst scheitert bind() */
	struct stat sock_info;
	if(stat(SOCKET_PATH, &sock_info) == 0)
	{
		if(unlink(SOCKET_PATH) == -1)
		{
			perror("unlink: ");
			exit(EXIT_FAILURE);
		}
	}
	
	/* Serveradresse an Socket binden */
	if(bind(sockfd, (struct sockaddr*)&server_addr, 
		sizeof(server_addr)) == -1)
	{
		perror("bind: ");
		if(close(sockfd) == -1)
		{
			perror("close: ");
		}
		exit(EXIT_FAILURE);
	}
	
	/* Zugriffsrechte auf mysocket werden von umask abgeleitet */
	/* Jeder Nutzer soll diesen Socket verwenden koennen */
	if(chmod(SOCKET_PATH, 0777) == -1)
	{
		perror("chmod: ");
		exit(EXIT_FAILURE);
	}
	
	/* Warteschlange initialisieren */
	/* Max. 5; 5 ist typischer Wert fuer backlog */
	if(listen(sockfd, 5) == -1)
	{
		perror("listen: ");
		exit(EXIT_FAILURE);
	}
	
	/* Exit-Handler installieren */
	atexit(exithandler);
	
	/* Signalbehandlung vorbereiten und installieren */
	sigemptyset(&(new.sa_mask));
	new.sa_handler = sighandler;
	new.sa_flags = 0;
	
	if(sigaction(SIGINT, &new, &old) == -1)
	{
		perror("sigaction: ");
		exit(EXIT_FAILURE);
	}
	
	/*=================================================================
	 * Daten empfangen
	 * ===============================================================*/
	/* Puffer fuer den Empfang des Dateinamens */
	char buffer[BUFFER_SIZE];
	
	/* Server-Schleife, Ende durch CTRL+C */
	while(1)
	{
		/* Laenge der Struktur fuer Client */
		clientaddrlen = sizeof(server_addr);
		
		/* Auf Auftrag warten */
		/* Blockiert bis Verbindung durch Client */
		if((commsockfd = accept(sockfd, (struct sockaddr*)&client_addr,
			(socklen_t*)&clientaddrlen)) == -1)
		{
			perror("accept: ");
			exit(EXIT_FAILURE);
		}
		
		/*==============================================================
		 * Zieldateinamen empfangen
		 * ===========================================================*/
		/* Auftrag lesen - Zielateinamen vom Client empfangen */
		char status_msg[MSG_SIZE]; /* Status zwischenspeichern */
		char filename[BUFFER_SIZE];
		
		/* Filename Array mit 0 initialisieren */
		memset(filename, 0, sizeof(filename));
		
		/* Zieldateinamen von Client lesen */
		int bytes_received = recv(commsockfd, filename, 
			sizeof(filename), 0);
		if(bytes_received == -1)
		{
			perror("recv: ");
			/* Socket schliessen */
			if(close(commsockfd) == -1)
			{
				perror("close: ");
				exit(EXIT_FAILURE);
			}
			continue;
		}
		
		/* Rueckmeldung zu Client, wenn Dateiname zu lang ist */
		if(strlen(filename) >= sizeof(filename) - 1)
		{
			strncpy(status_msg, "TOOLONGFILENAME", sizeof(status_msg));
			/* Client Rueckmeldung geben */
			if(send(commsockfd, status_msg, strlen(status_msg) + 1, 0)
				== -1)
			{
				perror("send: ");
			}
			/* Serverausgabe */
			fprintf(stderr, "Zieldateiname zu lang.\n");
			if(close(commsockfd) == -1)
			{
				perror("close: ");
			}
			continue;
		}
		
		/*==============================================================
		 * Zieldateinamen verarbeiten
		 * ===========================================================*/
		/* Oeffnen Zieldatei zum Schreiben */
		FILE *fp = fopen(filename, "wxb");
		if(fp == NULL)
		{
			if(errno == EEXIST)
			{
				/* Datei existiert bereits */
				strncpy(status_msg, "FILEEXISTS", sizeof(status_msg));
			}
			else
			{
				/* Anderer Fehler ist aufgetreten */
				strncpy(status_msg, "FILEERROR", sizeof(status_msg));
			}
			
			/* Client ueber aufgetretenen Fehler informieren */
			if(send(commsockfd, status_msg, strlen(status_msg) + 1, 0)
				== -1)
			{
				perror("send: ");
			}
			fprintf(stderr, "Datei konnte nicht geoeffnet werden.\n");
			close(commsockfd);
			continue;
		}
		
		/*==============================================================
		 * Status Code an Client senden
		 * ===========================================================*/
		strncpy(status_msg, "FILESUCCESS", sizeof(status_msg));
		if(send(commsockfd, status_msg, strlen(status_msg) + 1, 0)
			== -1)
		{
			perror("write: ");
		}
		
		/*==============================================================
		 * Quelldatei Daten empfangen und verarbeiten
		 * ===========================================================*/
		/* Daten vom Client empfangen und in die Zieldatei schreiben */		
		while((bytes_received = recv(commsockfd, buffer, 
			BUFFER_SIZE, 0)) > 0)
		{
			if(fwrite(buffer, sizeof(char), bytes_received, fp) 
				< bytes_received)
			{
				perror("fwrite: ");
				break;
			}
		}
		
		/*==============================================================
		 * Nachbereitung
		 * ===========================================================*/
		/* Socket schliessen */
		if(close(commsockfd) == -1)
		{
			perror("close: ");
			exit(EXIT_FAILURE);
		}
		/* Datei schliessen */
		if(fclose(fp) == -1)
		{
			perror("fclose: ");
			exit(EXIT_FAILURE);
		}
		
		printf("Zieldatei erfolgreich erstellt.\n");
		
	}
	
	/* Verbindung schliessen */
	if(close(sockfd) == -1)
	{
		perror("close: ");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;

 }
