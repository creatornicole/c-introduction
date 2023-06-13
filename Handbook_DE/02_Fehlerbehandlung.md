# Fehlerbehandlung

## Wieso ist die korrekt Fehlerbehandlung notwendig?

erforderlich für stabile und korrekte Funktionalität eines Programms

## Was gilt für Systemrufe (und Bibliotheksrufe)?

### Was gilt für den Ergebniswert von System-/Bibliotheksrufen?

Ergebniswert ist entweder ganze Zahl (int) oder Zeiger

### Wie lautet der Rückgabewert im Fehlerfall?

entweder -1 oder NULL

### Wo steht die Fehlernummer?

in der globalen Variable *extern int erno* (codierte Fehlerursache) (#include <errno.h>

### Kann man errno als Fehlerindikator nutzen?

Nein, da erfolgreiche Systemrufe errno nicht zurücksetzen

## Wie lässt man sich die Fehlerursache im Klartext ausgeben?

| Funktion | Beschreibung |
| --- | --- |
| char *strerror(int errnum) | wandelt Fehlernummer in String |
| void perror(const char *msg) | gibt msg und Fehlerursache im Klartext aus |