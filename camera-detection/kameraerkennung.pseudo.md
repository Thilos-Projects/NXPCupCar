# Kameraerkennung

## Geradeaus

- `BASE_ROW`  
  Konstante Gewählte Basiszeile
- `getEdges(y)`  
  Gibt Kanten in einer bestimmten Zeile `y` in Form eines Arrays an `x`-Werten zurück 
- `getCenter(edges)`
  Gibt den `x`-Wert der Mitte zwischen den beiden `x`-Werten des `edges`-Arrays zurück
- `DESIGNATED_CENTER`  
  Konstante - `x`-Koordinate, wo das Fahrzeug exakt mittig steht.

```
edges = getEdges(BASE_ROW)
lines = getLines(edges)
if (lines.len === 2) {
    return (getCenter(edges) - DESIGNATED_CENTER) // Davon Abhängig die Lenkentscheidung treffen (liegt Punkt mehr links / rechts...)
} else {
    // TODO: Diesen Fall können wir noch nicht abdecken -> Wir brauchen eine andere Zeile oder befinden uns auf Abwegen
}
```


### Pseudocode der einzelnen Methoden

#### Erläuterung von `getEdges(y)`

- `edgePixel`
Array in welches die Pixel geschrieben werden, die Kante enthalten 
- `getRow(y)`
gibt eine gewünschte (y) Bildzeile zurück
Beispielhafte Bild Zeile die zur AUswertung genutzt wird

Zeile | 0 | 1 | 2 | 3 |  4  |  5  | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 20| 21|  22 |  23 |  24 | 25| 26| 27 
------|---|---|---|---|-----|-----|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|-----|-----|-----|---|---|---
Wert  | 0 | 0 | 0 | 0 | 255 | 255 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 255 | 255 | 255 | 0 | 0 | 0 

```
/**
 * Berechnet an welchen Stellen der Sprung zwischen zwei Pixel über gewähltem Schwellenwert liegt (Identifikation von Kante)
 * Es wird der Pixel zurück gegeben, der in Differenz zum vorherigen Pixel hoch war
 * @param y Zeile die geprüft wird
 * @return Übergänge an denen eine Kante vermutet wird (Tupel aus Pixel + Stärke des Übergangs)
 */
edgePixel getEdges(y) {

  edgePixel
  row = getRow(y) //Bestehende Rechteckanfrage / Kommbination aus 2 Anfagen um alle Pixel zu erhalten

  for (i=1; i < row.length, i++) {
    if( row[i] - row[i-1]) >= 100 || row[i] - row[i-1]) <= 100 ) { 
      //Nicht den Betrag, sodass man später auf die Farbe schließen kann
      //--> positiv = Von schwarz auf weiß gewechselt, negativ = von weiß auf schwarz gewechselt
      edgePixel += (i, (row[i] - row[i-1]))
    }
  }
  return edgePixel
}
```
**Anmerkung:** ggf. muss die Differenz auch über mehrere vorherige Pixel gebildet werden wenn Sprung zwischen direkt benachbarten Pixeln nicht so groß ist!  
**Beispielergebnis oben:** edgePixel = [(4, -255),(6,255),(22, -255),(25,255)]




#### Erläuterung `getLines(edges)`


- `edges`
Input sind die gefundenen Kanten
Beispielhafter Input: [(4, -255),(6,255),(22, -255),(25,255)]

Möglichkeiten an zuvor gefundenen Kanten:
- Idealfall: 4 Kanten 
- Weitere Möglichkeiten
  - 2 Kanten (nur eine Linie zu sehen)
  - 3 Kanten (eine Linie angeschnitten)
  - 1 Kante / 0 Kanten (vermutlich alles zu spät) --> fall erst spät betrachten

Was ist für uns Relevant 
--> Im Primzip gucken ob es zwei Kanten gibt, die von Schwarz auf weiß gehen
--> Inneren Kannten der Linien (die erste Kante muss von Schwarz auf weiß gehen, die zweite von weiß auf schwarz)

- `left`
Speicherung der linken Linien

- `right`
Speicherung der rechten Linie


```
/**
 * @param edges
 * @return 
 */
getLines(edges) {

  left
  right

  //ToDo: muss noch Code werden
  //Fall für 3 / 4 Kanten (kann jeweils angewendet werden)
  if(edges.lenth() == 3 || edges.lengt() == 4) {
    // Gehe durch alle Kanten, bis du die erste Kante Schwarz auf weiß findest! Merke dir den Pixcel als Linie
    for(i = 0; i < edges.lenth(); i++) {
      if (edges[i] < 0 ) {
        left = i 
      }
    }
    //Suche die nächste Kante, diese muss weiß auf schwarz sein Merke den Pixel als Linie
    for(i = left, i < edges.length(), i++) {
      if(edges[i] > 0) {
        right = i
      }
    }

  return (edges[left], edges[right])

}
  //Fall, das nur 2 Kanten gefunden wurden: Kurve / Schräg auf der Geraden
  Ist die Kante bei einer Position kleiner als die definierte Mitte 
    return der schwarz weiß Kante
  Ist die Kante bei einer Position größer als die definierte Mitte
    return der weiß schwarz Kante  
}
```

**Weitere Ideen:** ggf. können hier auch die Entfernungen zwischen Kanten genutzt werden um sicherer eine Linie zu identifizieren und falsches auszuschließen!
--> Linienbreite darf nicht X Pixel unterschreiten und auch nicht Y Pixel überschreiten (kann es eine Linie sein)




#### Erläuterung `getCenter(lines)`

- `lines`
Input Parameter der zuvor identifizierten Linien 

```
/**
 * @param lines die y Werte des Pixel, an welchem eine Linie registriert wurde
 * @return die Mitte der gefundenen Linien
 */
int getCenter(lines) {
  return = lines[0] + lines[1] / 2
}
```
