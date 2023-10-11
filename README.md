# NXPCupCar

Wir lassen Auto fahren  

## Zeitplanung

```mermaid
gantt
dateFormat DD.MM.YYYY
axisFormat  %d.%m.
# excludes 24.12.2023, 25.12.2023, 26.12.2023, 27.12.2023, 28.12.2023, 29.12.2023, 30.12.2023, 31.12.2023, 01.01.2024, 02.01.2024

title Zeitplanung Autonomes Fahrzeug

section 0. Vorbereitung
    Kick-Off : crit, milestone, done, p0, 2023-10-06, 1d

    Meilenstein 0 - Vorbereitung : active, m0, 2023-10-06, 3w

    # Zeitplanung : done, a00, 2023-10-06, 4d

    Konzeptvorstellung : crit, milestone, p1, 2023-10-23, 1d



section 1. Fahren I
    Meilenstein 1 - Fahren I : m1, after m0, 3w

    Bau Fahrzeug : a1, after m0, 2w
    Ansteuerung der Motoren : a2, after a1, 1w

    # Demo 1 - Irgendwie fahren : crit, milestone, p4, 2023-11-17, 1d


section 2. Fahren II
    Meilenstein 2 - Fahren II : m2, after m1, 2w


section 3. Kamera I
    Meilenstein 3 - Kamera I : m3, after m0, 3w


section 4. Kamera II
    Meilenstein 4 - Kamera II : m4, after m3, 6w

    Entscheidung Erkennungsalgorithmus : a41, after m3, 1w

    Umsetzung der Erkennung : a42, after a41, 5w

    Erkennung Geradeaus : a42a, after a41, 2w
    Erkennung Kurven : a42b, after a42a, 1w
    Erkennung Kreuzung : a42c, after a42b, 1w
    Erkennung Ziellinie : a42d, after a42c, 1w


section 5. Fahren III
    Meilenstein 5 - Fahren III : m5, after m2, 6w

    Sichere 0 fahren : milestone, a52, after a42a a42b, 1d
    Sichere 8 fahren : milestone, a53, after a42c, 1d
    Sicher alles fahren : milestone, a54, after a42d, 1d

    # Demo 2 - 0 fahren : crit, milestone, p5, after a52, 1d
    # Demo 3 - 8 fahren : crit, milestone, p6, after a53, 1d
    # Demo 4 - Alles fahren: crit, milestone, p7, after a54, 1d


section 6. Optimierung
    Meilenstein 6 - Optimierung : m6, after m5, 5w


section Sonstiges
    Weihnachtsferien : p2, 2023-12-23, 2024-01-02
    Semesterende : milestone, p3, 2024-01-28, 1d

```
