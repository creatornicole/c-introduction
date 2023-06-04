/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*=====================================================================
 * 
 * Programm testet Umsetzung des Starts eines Kindprozesses wie folgt:
 * 
 * Programm welches einen Kindprozess erzeugt, seine eigene PID und die
 * seines Elternteiles anzeigt, danach bis 1 Milliarde zaehlt und sich
 * dann beendet.
 * 
 * Der Elternteil soll seine eigene und die PID des Kindes ausgeben und
 * in einer Schleife jeweils nach 1 Sekunde (sleep(1)) pruefen, ob
 * Kind schon fertig ist.
 * Zustand "läuft" oder "fertig" ist jedes mal auf Konsole auszugeben.
 * Sobald das Kind beendet ist, soll der Elternteil den Rueckgabewert
 * des Kindes auswerten und ausgeben.
 * Danach soll er sich beenden.
 * 
 * ===================================================================*/

int main(int argc, char *argv[])
{
	int npid;
	npid = fork(); /* ab hier zwei gleich laufende Prozesse */
	
	if (npid == -1) { /* Elternprozess -1 im Fehlerfall */
		/* Code im Fehlerfall */
		perror("fork: ");
		exit(EXIT_FAILURE);
		
	} else if (npid == 0) { /* Kindprozess immer 0 */
		/* Code fuer Kindprozess */
		
		/* Anzeige eigene PID und die seines Elternteiles */
		printf("Ich bin ein Kindprozess.\n");
		printf("Meine PID: %d\n", getpid());
		printf("Die PID meines Elternteiles (PPID): %d\n", getppid());
		
		printf("==================================================\n");
		
		/* Zaehlt bis 1 Milliarde und beendet sich dann */
		int i;
		int counter = 1000000000;
		for(i = 0; i <= counter; i++) {
			/* Zaehlt bis 1 Milliarde ohne weitere Aktion */
		}
		
		/* Systemruf zur Prozessbeendigung */
		/* exit() stellt geordneten Prozessabschluss mit korrekter
			Ressourcenfreigabe sicher */	
		/* Sendet Signal SIGCHLD an Elternprozess - signalisiert Ende */	 
		exit(EXIT_SUCCESS); 
		
	} else { /* Elternprozess PID des Kindes bei erfolgreichen fork() */
		/* Code fuer Elternprozess */
		 
		 /* Anzeige eigenes PID und die des Kindes */
		 printf("Ich bin ein Elternprozess.\n");
		 printf("Meine PID: %d\n", getpid());
		 printf("Die PID meines Kindes: %d\n", npid);
		 
		 printf("==================================================\n");
		 
		 /* Jede Sekunde pruefen, ob Kind schon fertig ist */
		 
		 /* wait() wartet auf ersten beendeten Kindprozess */
		 /* waitpid() gezielte Ueberwachung bestimmten Kindprozesses */
		 
		 int wstatus;
		 while(1) {
			 
			 sleep(1); /* aller einer Sekunde ueberpruefen */
			 
			 /* WNOHANG - mit Option unterbricht waitpid() Ausfuehrung
					nicht, sollte Kind nicht fertig gewurden sein */
			 /* npid ist PID Kindprozess bei erfolgreichem fork() */
			int exitStatus = waitpid(npid, &wstatus, WNOHANG);
			
			if(exitStatus == -1) { /* Fehler waitpid() */
				perror("waitpid: ");
				exit(EXIT_FAILURE);
				
			} else if(exitStatus == 0) { /* Kindprozess nicht fertig */
				printf("läuft\n");
				
			} else { /* Kindprozess fertig/beendet */
				printf("===========================================\n");
				printf("fertig\n");
				
				/* Rueckgabewert des Kindes auswerten */
				/* Wenn Kindprozess (normal) beendet wurde -> true */
				/* "normal" = exit(), _exit(), return from main() */
				if(WIFEXITED(wstatus)) {
					printf("Rueckgabewert des Kindprozesses: %d\n", 
					WEXITSTATUS(wstatus));
				}
				
				/* Schleife verlassen */
				break;
			}
		 }
	}
		
	return EXIT_SUCCESS;
}
