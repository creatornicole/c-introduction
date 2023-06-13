# Das Dateisystem von UNIX

## Worüber werden alle I/O-Operationen ausgeführt?

über Dateien

### Wie lautet die Schlussfolgerung daraus?

da alle I/O-Operationen über Dateien ausgeführt werden, heißt das, dass Platten, Bänder, Terminals, … mit den gleichen Systemrufen arbeiten

---

## Was ist eine Datei?

eine unstrukturierte, geordnete Folge von Bytes (Bytestrom)

---

## Wo sind die Verwaltungsdaten der Datei enthalten?

im sogenannten i-node

### Welche Verwaltungsdaten sind bspw. im i-node enthalten?

Besitzer, Gruppe, Zugriffsrechte, welche Plattenblöcke gehören zur Datei

---

## Welche verschiedenen Dateiarten existieren im Dateisystem von UNIX?

| Dateiart | Beschreibung | Hinweise/Anmerkungen |
| --- | --- | --- |
| Gewöhnliche Dateien | Dienen Speicherung von Daten und Programmen |  |
| Verzeichnisse | enthalten Listen anderer Dateien (Dateinamen und zugehörige i-node-Nummer | Dateinamen meist bis zu 255 Zeichen lang |
| Gerätedateien (spexial files) | Zugriff auf Gerätetreiber (z.B. für Platten, Terminals, RS232 usw) als Datei |  |
| Pipes | spezielle Dateiform, FIFO-Speicher | Schreiben füllt FIFO mit Daten, Lesen verbraucht Daaten |
| (Soft-)Links | enthalten Verweise auf andere Dateien |  |
| Sockets | Schnittstelle für die Kommunikation zwischen Prozessen |  |

### Welche Geräte unterscheidet man?

- blockorientiert Geräte
- zeichenorientierte Geräte

### Wo stehen die Gerätedateien?

im Verzeichnis */dev*

### Was gehört zu jeder Gerätedatei?

- major device number
    - unterscheidet Gerätetyp und damit zugehörigen Treiber
- minor device number
    - konkrete Gerätenummer (bei gleichem Gerätetyp)

---

## Wie ist ein Plattenspeicher aufgebaut?

- Festplatten speichern Daten in Sektoren (i.Allg. 512 Byte, neu: 4096 Byte)
- Sektoren sind in konzentrischen Kreisen (Zylindern) angeordnet
- mehrere solche Kreise (Plattenoberflächen) werden gestapelt (Kopf)
- ein bestimmter Sektor auf der Platte wird adressiert über CHS (veraltet) (Zylinder, Kopf, Sektor) oder fortlaufender Sektornummer n (Logical Block Adressing)

---

## Wie wird ein Plattenspeicher verwaltet?

zwei unterschiedliche Varianten:

- Reservieren des für eine Datei maximal benötigten Speichers als zusammenhängenden Block (BS für Mainframes, z.B. OS/360)
- Einteilen des Plattenspeichers in Blöcke
    - Zuordnung einer Anzahl Blöcke zu einer Datei
    - Blöcke müssen dabei nicht zusammenhängen

### Welchen Ansatz verwenden alle modernen Dateisysteme?

das Einteilen des Plattenspeichers in Blöcke und der dementsprechenden Zuordnung einer Anzahl Blöcke zu einer Datei

### Welche Probleme/Fragen ergeben sich jedoch aus diesem Ansatz?

1. Wahl der Blockgröße
2. Organisation der Zuordnung der Blöcke zu einer Datei

### Was bedeutet die Wahl von großen Blockgrößen?

- große Blöcke bedeuten schnelleren Zugriff
    - ein Block besteht aus mehreren aufeinanderfolgenden Sektoren
- große Blöcke bedeuten trotz eingeschränkter Blockzahl (z.B. 32 Bit) große mögliche Dateigrößen
- große Blöcke bedeuten viel Verschnitt bei kleinen Dateien

### Wie hat man das Problem der Organisation der Zuordnung der Blöcke zu einer Datei gelöst?

unterschiedliche Ansätze bei den gebräuchlichen Dateisystemen, unterschiedliche Begriffe

### Wie wird dieses Problem der Organisation der Zuordnung der Blöcke zu einer Datei beim FAT-Dateisystem gelöst?

- Blöcke heißen Cluster
- Zuordnung der Cluster (Blöcke) zu Dateien über verkettete Liste
- Verwaltung über File Allocation Table
    - ein Eintrag pro Cluster
    - enthält Nummer des Folgeclusters

### Was enthält ein Verzeichnis im FAT-Dateisystem?

Dateiname und -Attribut sowie Startcluster

---

## Wie ist ein UNIX-Dateisystem aufgebaut?

Platz auf Speichermedium (i.a. Festplatte) in eine oder mehrere Partitionen (logische Platten, Volumes) unterteilt

### Was gehört zu jeder Partition?

eine Gerätedatei (Bsp. Linux: /dev/sda1, /dev/sda2, /dev/sdb5)

### Was enthält jede Partition?

ein Filesystem mit folgendem Aufbau:

| Block-Nr. | Funktion | Hinweise |
| --- | --- | --- |
| Block 0 | unbenutzt (Bootblock) |  |
| Block 1 | Superblock (Zahl i-Nodes, Zahl der Blöcke, Information über freie Blöcke, freie i-Nodes etc | ist bei unterschiedlichen Filesystemen unterschiedlich aufgebaut, wird meist mehrfach gespiegelt |
| ab Block 2 | i-nodes | Anzahl bei Anlegen Filesystem festgelegt |
| ab Block n | Datenblöcke | n je nach i-Node-Anzahl |

### Wieso ist die maximal mögliche Dateianzahl durch den Aufbau des UNIX-Dateisystems beschränkt?

- da die Anzahl der i-nodes beim Anlegen des Filesystems festgelegt wird
- ein i-Node pro Datei nötig (sodass Dateizahl pro Partition begrenzt ist) (pro Datei ein i-node mit Verwaltungsdaten)

![Untitled](Das%20Dateisystem%20von%20UNIX%20af03a74bf51d4e71a1811ec8ea15fa7b/Untitled.png)

### Wieso ist das UNIX-Dateisystem in eine oder mehrere Partitionen aufgeteilt?

---

## i-Nodes

### Was enthalten i-Nodes?

die Verwaltungsinformationen für Dateien

### Wie lautet eine i-Node Number?

ab 1 durnummeriert

### Durch was wird eine gestreute Speicherung ermöglicht?

durch die Zuordnung der Datenblöcke zu Dateien

### Welche beiden i-Nodes haben Sonderbedeutung?

- i-Node 1 unbenutzt (bzw. Bad-Block-Liste)
- i-Node 2 für Wurzelverzeichnis /

### Wie ist ein i-Node prinzipiell aufgebaut?

- Dateityp, Zugriffsrechte
- Anzahl der Links
- UID des Eigentümers
- GID bevorzugte Gruppe
- Dateigröße
- Zeitpunkt des letzten Zugriffs
- Zeitpunkt der letzten Änderung
- N direkte Verweise (ca. 10)
    - auf Datenblöcke (Nummer des Blocks)
- einfach indirekter Verweis
    - verweist auf direkten Verweis, welcher auf Datenblock zeigt
    - also auf Block mit Datenblocknummern
- doppelt indirekter Verweis
    - analog zu einfach indirekten Verweis
- dreifach indirekter Verweis
    - analog zu einfach indirekten Verweis

![Untitled](Das%20Dateisystem%20von%20UNIX%20af03a74bf51d4e71a1811ec8ea15fa7b/Untitled%201.png)

### Wie lauten die Bits für die Dateitypen im i-Node?

| Bitfolge | Dateityp |
| --- | --- |
| 1000 | Gewöhnliche Datei |
| 0100 | Verzeichnis |
| 0010 | Character-Gerätedatei |
| 0110 | Block - Gerätedatei |
| 0001 | FIFO |
| 1010 | SymLink |
| 1100 | Socket |

### Welche Bedeutung steckt hinter den Abkürzungen der Zugriffsrechte?

- r - Readable - Lesbar
- w - Writeable - Schreibbar
- x - eXecutable - Ausführbar

### Welche Bedeutung haben die Spezial-Bits suid, sgid und sticky?

- suid - set user id on execution
- sgid - set group id on execution
- sticky - Programm wird nicht aus Speicher entfernt

---

## Wie wird die maximal mögliche Dateisystemgröße bestimmt?

Bestimmen Anzahl maximal möglicher Blöcke:

L = N + M/4 + (M/4)^2 + (M/4)^3 

mit N - Anzahl der direkten Datenblöcke

M - Blockgröße in Byte

M/4 - Anzahl Verweise pro Block

### Wie sieht diese Berechnung bspw. für 1k Blockgröße und 10 direkte Blöcke aus?

10 kB + 256 kB + 64 MB + 16 GB

### Welche Größe ist für Blocknummern typisch?

- Blocknummern sind typisch unsigned long = 4 Byte/ 32 Bit
    - ermöglicht max. 4 249 976 296 Blöcke
    - bei 1k Blöcken - Dateisystemgröße 4 TByte
    - bei 4k Blöcken - 16 TByte

---

## Wie ist ein Verzeichnis (vereinfacht) aufgebaut?

als Folge von Einträgen in einen Verzeichnis-Block

### Wie lautet dies für Systeme mit max. Länge des Dateinamens von 14 Zeichen?

i-Nummer | Dateiname

### Wie lautet dies in aktuellen Systemen mit 255 Zeichen langen Dateinamen (z.B. Linux)?

i-Nummer | Länge des Eintrags | Länge des Namens | Dateiname

### Was wird ein Verzeichnis nie?

verkleinert

### Was bedeutet die i-Nummer 0 im Zusammenhang mit einem Verzeichnis?

ungenutzter Eintrag

---

## Hardlinks

### Was ist ein Hardlink?

mehrere Dateinamen verweisen auf gleichen i-Node/gleiche Datei

### Was bedeutet dies?

eine Datei kann mehrere Namen haben (stehen in Verzeichnis), aber nur einen Eigentümer und einen Satz Zugriffsrechte

### Was geschieht beim Löschen eines Links?

lediglich der Linkzähler wird dekrementiert

### Was geschieht, wenn der Link-Zähler gleich 0 ist?

i-Node wird freigegeben und Datei wird gelöscht

### Kann unmittelbar festgestellt werden, welche Links eine Datei besitzt?

Nein

### Können Links über Filesystemgrenzen hinausreichen?

Nein

---

## Symbolische Links (Soft-Links)

### Was ist ein Soft-Link?

eine spezielle Dateiart mit Verweis auf eine andere Datei (absoluter oder relativer Pfad)

### Können Soft-Links über Filesystemgrenzen hinwegreichen?

Ja (aufgrund des Verweises auf eine Datei über den absoluten oder relativen Pfad)

### Was geschieht, wenn die Datei, auf die der Softlink verweist, gelöscht wird?

Softlink bleibt weiterhin bestehen (dangling Softlink)

### Welche Funktionen beziehen sich auf die durch den Link referenzierte Datei?

Öffnen, Lesen, Schreiben, Positionieren, Ändern der Zugriffsrechte, Ändern des Besitzers oder der Gruppe, Abfrage des Dateistatus

### Welche Funktionen beziehen sich dahingegen auf den symbolischen Link?

Löschen, Umbenennen, Abfrage Link-Status

### Welche Eigentümer und welche Zugriffsrechte besitzen Soft-Links?

- eigenen Eigentümer
- immer Zugriffsrechte: rwxrwxrwx