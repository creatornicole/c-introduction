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
 * Programm testet Umsetzung des Starts eines Kindprozesses
 * 
 * ===================================================================*/

/* TODO: Prozesswissen kurz zusammenfassen */
/* Tipp: Recherchieren man page zu Funktionen wait() und waitpid() 
 * deren Parameter und die verfuegbaren Optionen. */
/* wait wartet auf ersten beendeten Kindprozess */
/* waitpid() gezielte Ueberwachung bestimmten Kindprozesses */
/* zu jeder BS-Funktion gehoert Test*/
/* printf()-Ausgabe erfolgt erst, wenn Puffer voll ist oder 
 * Zeilenumbruch erfolgt */

int main(int argc, char *argv[])
{
	int npid;
	npid = fork(); /* ab hier zwei gleich laufende Programme */
	
	/* PID? Wie wird diese vergeben? */
	/* Fehlerbehandlung?! */
	
	if (npid == -1) { 
		/* Fehler ist aufgetreten */
		perror("fork: ");
		exit(EXIT_FAILURE);
		
	} else if (npid == 0) {
		/* Code fuer Kindprozess */
		/* Kindprozessbehandlung (Kindprozess immer 0) */
		
		/* Anzeige eigene PID und die seines Elternteiles */
		printf("Ich bin ein Kinprozess.\n");
		printf("Meine PID: %d\n", getpid());
		printf("Die PID meines Elternteiles (PPID): %d\n", getppid());
		
		/* Zaehlt bis 1 Milliarde und beendet sich dann */
		int i;
		int counter = 1000000000;
		for(i = 0; i <= counter; i++) {
			/* Zaehlt bis 1 Milliarde ohne weitere Aktion */
		}
		
		/* Systemruf zur Prozessbeendigung */
		/* Wirkungsweise: schliesse offene Dateien, Elternprozess
		 * erhaelt Signal SIGCHLD, falls Prozess Login-Shell ist,
		 * erhaelt gesamte Prozessgruppe SIGHUP, noch existierende
		 * Kinder werden vom Init-Prozess (PID 1) "adoptiert" */
		 
		 /* TODO: Unterschied exit() zu _exit() */
		_exit(EXIT_SUCCESS); 
		
	} else {
		/* Code fuer Elternprozess */
		/* Elternprozessbehandlung bei erfolgreichem fork() 
		 * Elternprozess PID des Kindes oder -1 bei Fehler */
		 
		 /* Anzeige eigenes PID und die des Kindes */
		 printf("Ich bin ein Elternprozess.\n");
		 printf("Meine PID: %d\n", getpid());
		 printf("Die PID meines Kindes: %d\n", npid);
		 
		 /* Wartet aktiv auf Ende des Kindprozesses */
		 /* Prueft in Schleife jeweils nach 1 Sekunde, ob
		  * Kind schon fertig ist */
		 int status;
		 while (1) {
			int result = waitpid(npid, &status, WNOHANG);
			if(result == -1) {
				/* Fehler beim waitpid() */
				/* Welche moegliche Fehler? */
				perror("waitpid: \n");
				exit(EXIT_FAILURE);
			} else if (result == 0) {
				/*Kindprozess noch nicht beendet */
				printf("läuft\n");
				sleep(1);
			} else {
				/* Kindprozess ist beendet */
				printf("fertig\n");
				/* Rueckgabewert des Kindes auswerten */
				 printf("Rueckgabewert des Kindprozesses: %d\n", 
					WEXITSTATUS(status));
				/* Schleife verlassen */
				break;
			}
		 }
	}
		
	return EXIT_SUCCESS;
}
