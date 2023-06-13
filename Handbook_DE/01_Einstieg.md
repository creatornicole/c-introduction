# Einstieg

## Was bezeichnet man als Systemprogrammierung?

Als Systemprogrammierung bezeichnet man das Erstellen von Softwarekomponenten, die Teil des Betriebssystems sind oder die eng mit dem Betriebssystem bzw. mit der darunter liegenden Hardware kommunizieren müssen.

---

## Was steckt hinter dem POSIX-Standard?

- POSIX = Portable Operating Systems Interface
- verschiedene POSIX-Standards

### Welche POSIX-Standards existieren für welchen Anwendungsbereich?

| POSIX-Standard | Anwendungsbereich |
| --- | --- |
| IEEE 1003.1 | enthält API (Systemrufe + Bibliotheksfunktionen) |
| IEEE 1003.2 | Shells und Utilities |
| IEEE 1003.4 | Echtzeitprogrammierung |
| IEEE 1003.7 | System Administration |

## Aus welchen Komponenten besteht die UNIX-Programmierschnittstelle im wesentlichen?

| Funktion | Beschreibung |
| --- | --- |
| Bibliotheksfunktionen | bieten Support-Funktionen für Anwendungsprogrammierer,
kapseln Aufrufe von BS-Funktionen in Hochsprache - C |
| Betriebssystemdienste | elementare Dienste des Betriebssystems, durch Systemrufe (System Calls) ausgelöst,
wesentliche Teile: Dateisystem, Prozesssteuersystem |

### Wieso sind Systemrufe relativ langsam?

aufgrund der zwei benötigten Kontextwechsel (User ↔ Supervisor-Mode)

### Kommen Bibliotheksfunktionen ohne System Call aus?

Ja, dank Unterprogramm-/UP-Aufruf via CALL/RET → relativ schnell