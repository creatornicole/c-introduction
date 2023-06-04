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
 * auf das Ende des Kindprozesses mit wait() warten.
 * Sobald das Kind beendet ist, soll der Elternteil den Rueckgabewert
 * des Kindes auswerten und ausgeben.
 * Danach soll er sich beenden.
 * 
 * ===================================================================*/

int main(int argc, char *argv[])
{
	int npid;
	npid = fork(); /* ab hier zwei gleich laufende Programme */
	
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
		 
		 /* Wartet auf das Ende eines Kindprozess mit wait(), um 
				Exit-Status Kindprozess auswerten zu koennen */
		 /* Ermittelt dabei Status des ersten beendeten Kindprozesses */
		 /* Schleife -> auf Beenden aller Kindprozesse warten 
				Rueckgabe -1 -> alle Kindprozesse beendet */
		 
		 int wstatus;
		 while(wait(&wstatus) > 0) {
			/* Rueckgabewert des Kindes auswerten */
			/* Wenn Kindprozess (normal) beendet wurde return true */
			/* "normal" = exit(), _exit(), return from main() */
			if(WIFEXITED(wstatus)) {
				printf("Rueckgabewert des Kindprozesses: %d\n", 
				WEXITSTATUS(wstatus));
			}
		 }
	}
		
	return EXIT_SUCCESS;
}
