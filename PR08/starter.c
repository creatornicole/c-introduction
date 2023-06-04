/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*=====================================================================
 * 
 * Programm, mit welchem andere Programme mit beliebiger Anzahl von
 * Argumenten gestartet werden koennen.
 * 
 * Aufruf:
 * ./starter [Befehl/Programm] [Argumente]
 * 
 * ===================================================================*/

int main(int argc, char *argv[]) {
	
	/* Abfangen fehlerhaften Programmaufrufs */
	if(argc < 2) {
		fprintf(stderr, "Error: Fehlerhafter Programmaufruf.\n");
		fprintf(stderr, "Starte Programm wie folgt: ");
		fprintf(stderr, "./starter <Befehl/Programm> [Argumente]\n");
		exit(EXIT_FAILURE);
	}
	
	/* Uebergebenen Parameter fuer execvp() in Zeigerfeld speichern */
	char *params[argc];
	for (int i = 1; i < argc; i++) {
		params[i - 1] = argv[i];
	}
	params[argc - 1] = NULL; /* Null-terminieren */
	
	/* Kindprozess starten */
	int npid;
	npid = fork(); /* ab hier zwei gleich laufende Prozesse */
	
	if (npid == -1) { /* Elternprozess -1 im Fehlerfall */
		/* Code im Fehlerfall */
		perror("fork: ");
		exit(EXIT_FAILURE);
		
	} else if (npid == 0) { /* Kindprozess immer 0 */
		/* Code fuer Kindprozess */
		
		/* Ausfuehrung execvp() in Kindprozess, da nach Aufruf
			weiterer Code ausgefuehrt werden soll */
		
		/* exec-Familie Funktionen in C, die dazu dienen, aktuellen
			Prozess durch neues Programm zu ersetzen */
		/* execvp() ersetzt aktuellen Prozess durch neue Programm */
		
		/* execvp() erwartet relativen Pfad */
		/* execv() erwartet absoluten Pfad - BS sucht Datei in 
			Verzeichnissen der PATH-Umgebungsvariable */
		
		/* argv[1] auszufuehrende Programm
			params uebergebene Parameter fuer zu startende Programm */
		execvp(argv[1], params);
		
		/* Wenn execvp() erfolgreich, wird nachfolgende Code nicht
			mehr erreicht */
		perror("execvp: ");
		exit(EXIT_FAILURE);
		
	} else { /* Elternprozess PID des Kindes bei erfolgreichen fork() */
		/* Code fuer Elternprozess */
		
		/* Wartet auf Beendigung des Kindprozesses */
		int wstatus;
		int exitStatus;
		
		exitStatus = waitpid(npid, &wstatus, 0);
		
		if(exitStatus == -1) { /* Fehler waitpid() */
			perror("waitpid: ");
			exit(EXIT_FAILURE);		
		}
		
		/* Gibt PID und Returnwert des Kindprozesses aus */
		if (WIFEXITED(wstatus)) {
			printf("===============================================\n");
			printf("Kindprozess mit PID %d beendet.\n", npid);
			printf("Beendet mit Returnwert: %d\n", 
				WEXITSTATUS(wstatus));
			
		} else {
			fprintf(stderr, "Fehler ist aufgetreten: Kindprozess mit ");
			fprintf(stderr, "PID %d wurde nicht normal beendet.\n",
				npid);
			
		}
	}
	
	return EXIT_SUCCESS;
}
