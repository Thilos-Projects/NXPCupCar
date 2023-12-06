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
if (edges.len === 2) {
    return (getCenter(edges) - DESIGNATED_CENTER)
} else {
    // TODO: Diesen Fall können wir noch nicht abdecken -> Wir brauchen eine andere Zeile oder befinden uns auf Abwegen
}
```
