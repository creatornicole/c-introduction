# Verbesserungen PR11

- statt BUFFER_SIZE PATH_MAX nutzen
- nicht fopen, ... nutzen
    - komplizierteres Fehler-Handling
    - Ausnahme: Nutzen, wenn man diese richtig anwenden kann
    - Bessere Alternative: open, ...
- falsches Fehlerhandlung mit fxxxx
    - extra Funktion und nicht errno
- Hinweis: statt Abfrage in Client von bspw. "FILEEXISTS", Nummer vergeben und diese überprüfen