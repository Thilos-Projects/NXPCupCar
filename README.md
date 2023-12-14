# NXPCupCar

Wir lassen Auto fahren  

## Zeitplanung

```mermaid
gantt
dateFormat DD.MM.YYYY
axisFormat  %d.%m.
# excludes 24.12.2023, 25.12.2023, 26.12.2023, 27.12.2023, 28.12.2023, 29.12.2023, 30.12.2023, 31.12.2023, 01.01.2024, 02.01.2024

title Zeitplanung Autonomes Fahrzeug [Team TFT]

%% This only works on mermaid.live:

# tickInterval 1week
# weekday monday
# todayMarker off

section 0. Vorbereitung
    Kick-Off : crit, milestone, done, p0, 2023-10-06, 1d

    Meilenstein 0 - Vorbereitung : done, m0, 2023-10-06, 24d

    Konzeptvorstellung : crit, done, milestone, p1, 2023-10-30, 0d


section 1. Fahren I
    Meilenstein 1 - Fahren I : done, m1, after m0, 4w

    Bau Fahrzeug : done, a11, after m0, 2w
    Ansteuerung der Motoren : done, a12, after a11, 2w


section 2. Fahren II
    Meilenstein 2 - Fahren II : done, m2, after m1, 2w

    Ansteuerung Servo : done, a21, after m1, 1w
    Hall-Sensoren & Geschwindigkeitsmessung : done, a22, after a21, 1w

    Demo 1 - Irgendwie fahren : done, crit, milestone, p4, 2023-12-08, 0d


section 3. Kamera I
    Meilenstein 3 - Kamera I : done, m3, after m0, 43d

    Spike Pixy Firmware : done, a31, after m0, 4w
    Kantenerkennung : done, a32, after a31, 15d


section 4. Kamera II
    Meilenstein 4 - Kamera II : active, m4, 2023-12-06, 6w

    Entscheidung Erkennungsalgorithmus : done, a41, 2023-12-06, 6d

    Umsetzung der Erkennung : active, a42, 2023-12-11, 4w

    Erkennung Geradeaus : done, a42a, 2023-12-11, 1w
    Erkennung Kurven : active, a42b, after a42a, 1w
    Erkennung Kreuzung : a42c, after a42b, 1w
    Erkennung Ziellinie : a42d, after a42c, 1w


section 5. Fahren III
    Meilenstein 5 - Fahren III : active, m5, after m2, 6w

    Sichere 0 fahren : done, milestone, a52, 2023-12-13, 1d
    Sichere 8 fahren : milestone, a53, after a42c, 1d
    Sicher alles fahren : milestone, a54, after a42d, 1d

    Demo 2 - 0 fahren : crit, milestone, p5, 2023-12-19, 1d
    [Platzhalter] Demo 3 - 8 fahren : crit, milestone, p6, after a53, 1d
    [Platzhalter] Demo 4 - Alles fahren: crit, milestone, p7, after a54, 1d


section 6. Optimierung
    Meilenstein 6 - Optimierung : m6, after m5, 5w


section Sonstiges
    Weihnachtsferien : p2, 2023-12-23, 11d
    Prüfungsphase HSD : p8, 2024-01-31, 29d
    [Platzhalter] Prüfung Autonomes Fahren : crit, milestone, p9, 2024-03-10, 0d
    Ender der Vorlesungszeit : milestone, p3, 2024-01-28, 1d


```
