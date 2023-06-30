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

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */
#define BUFFER_SIZE 1024
#define SOCKET_PATH "/tmp/mysocket"

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
 * 
 * ===================================================================*/
 
 
 int main(int argc, char *argv[]) 
 {

	struct sockaddr_un server_addr, client_addr;
	int addrlen;
	socklen_t clientaddrlen;
	struct sigaction old, new;
	int commsockfd; /* Socket fuer Kommunikation zum Reinschreiben */
	
	/* Serveradresse vorbereiten */
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_LOCAL;
	strncpy(server_addr.sun_path, SOCKET_PATH, 
		sizeof(server_addr.sun_path) -1);
	
	addrlen = strlen(server_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);
	
	/* Socket anlegen */
	if((sockfd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1) {
		perror("socket: ");
		exit(EXIT_FAILURE);
	}
	
	/* Entferne alten Dateieintrag fuer Socket */
	/* Sonst scheitert bind() */
	remove(SOCKET_PATH);
	
	/* Serveradresse an Socket binden */
	if(bind(sockfd, (struct sockaddr*)&server_addr, addrlen) == -1)
	{
		perror("bind: ");
		exit(EXIT_FAILURE);
	}
	
	/* Zugriffsrechte auf mysocket werden von umask abgeleitet */
	/* Jeder Nutzer soll diesen Socket verwenden koennen */
	if(chmod(SOCKET_PATH, 0777) == -1)
	{
		perror("chmod: ");
		exit(EXIT_FAILURE);
	}
	
	/* Auf Anfrage warten */
	/* 5 ist typischer Wert fuer backlog */
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
	
	/* Puffer fuer den Empfang des Dateinamens */
	char buffer[BUFFER_SIZE];
	
	/* Server-Schleife, Ende durch CTRL+C */
	while(1)
	{
		/* Laenge der Struktur fuer Client */
		clientaddrlen = sizeof(struct sockaddr_un);
		
		/* Auf Auftrag warten */
		/* Blockiert bis Verbindung durch Client */
		if((commsockfd = accept(sockfd, (struct sockaddr*)&client_addr,
			&clientaddrlen)) == -1)
		{
			perror("accept: ");
			exit(EXIT_FAILURE);
		}
		
		/* Auftrag lesen - Zielateinamen vom Client empfangen */
		char zieldatei[BUFFER_SIZE];
		ssize_t bytes_received = read(commsockfd, zieldatei, sizeof(zieldatei) -1);
		if(bytes_received == -1)
		{
			perror("read: ");
			close(commsockfd);
			exit(EXIT_FAILURE);
		}
		
		zieldatei[bytes_received] = '\0';
		
		/* Zieldatei erstellen oder ueberschreiben */
		int zielfd = open(zieldatei, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if(zielfd == -1)
		{
			perror("open: ");
			close(commsockfd);
			exit(EXIT_FAILURE);
		}
		
		/* Daten vom Client empfangen und in die Zieldatei schreiben */
		ssize_t bytes_received_total = 0;
		
		while((bytes_received = read(commsockfd, buffer, BUFFER_SIZE)) > 0)
		{
			ssize_t bytes_written = write(zielfd, buffer, bytes_received);
			if(bytes_written == -1)
			{
				perror("write: ");
				close(commsockfd);
				close(zielfd);
				remove(zieldatei); /* Loeschen, da Schreibvorgang
										fehlgeschlagen */
				exit(EXIT_FAILURE);
			}
			bytes_received_total += bytes_received;
		}
		
		if(bytes_received == -1)
		{
			perror("read: ");
			close(commsockfd);
			close(zielfd);
			remove(zieldatei); /* Loeschen, da Schreibvorgang
										fehlgeschlagen */
			exit(EXIT_FAILURE);
		}
		
		/* Verbindungen und Dateideskriptoren schliessen */
		close(commsockfd);
		close(zielfd);
		
	}
	
	close(sockfd);
	
	return EXIT_SUCCESS;

 }
