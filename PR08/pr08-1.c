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
		
	} else {
		/* Elternprozessbehandlung bei erfolgreichem fork() 
		 * Elternprozess PID des Kindes oder -1 bei Fehler */
		 
		 /* Anzeige eigenes PID und die des Kindes */
		 printf("Ich bin ein Elternprozess.\n");
		 printf("Meine PID: %d\n", getpid());
		 printf("Die PID meines Kindes: %d\n", npid);
		 
		 /* Wartet auf das Ende seines Kindprozess mit wait() */
		 /* Umsetzung stellt sicher, dass auf alle Kindprozese 
				gewartet wird - erst wenn alle zurueckgekehrt sind
				wird zu naechster Anweisung uebergegangen */
		 
		 /* ZU UEBERARBEITEN */
		 int status;
		 while(wait(&status) > 0);
		 printf("%d\n", status);
		 
		 /* Wertet und gibt Rueckgabewert des Kindes aus */
		 /* Beendet sich dann */
	}
		
	return EXIT_SUCCESS;
}
