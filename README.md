# Labor Versuch 2: 7‐Segment‐Display im Zeitmultiplex

HAPO Labor bei Prof. Dr.‐Ing. Jens Altenburg an der TH-Bingen

## Aufgabenstellung:
Im Versuch 1 wurde die Zeichendarstellung des 7‐Segment‐Displays für ein einzelnes Digit realisiert. Die Anzeige hat jedoch 4 Stellen. Beim zweiten Laborversuch erfolgt nun die Ansteuerung der vier Digits im sogenannten Zeitmultiplexbetrieb. Das manuelle Durchschalten der Anzeigepositionen wird durch einen automatischen Vorgang ersetzt. Damit der Eindruck einer flimmerfreien Anzeige entsteht muss die „Bildwiederholfrequenz“ zur Anzeigeaktualisierung bestimmt werden. Klassische Leuchtstoffröhren werden mit einer Wechselspannung von 50 Hz betrieben. Da sowohl die positive als auch die negative Halbwelle des Stromflusses ein Leuchten erzeugt, beträgt die „Leuchtfrequenz“ demnach 100 Hz. Für das menschliche Auge ergibt dies ein „flimmerfreies“ Licht. Die 7‐Segment‐Anzeige wird deswegen mit dieser Frequenz betrieben. Da es vier Digits gibt bedeutet dies, dass die jeweilige Anzeigestelle nach 2,5 ms gewechselt wird. Im Versuch werden von den vier Stellen nur drei Position mit Zahlen beschrieben. Diese drei Stellen geben einen Zählwert in hundertstel Sekunden an. Zählbereich von 000 ... 999, also von 0 bis 9,99 Sekunden. Mit diesem Kurzzeitzähler wird die Reaktionszeit auf ein akustisches Signal gemessen. Die Versuchsaufgabe lautet deswegen:
- Realisieren Sie eine Stoppuhr mit einer Auflösung von 0,01 Sekunden im Bereich von 0 bis 9,99 Sekunden
- Nach einer zufälligen Zeit von min. 3 bis max. 5 Sekunden ertönt ein akustisches Signal von 50 ms Länge. Mit dem Ertönen des Signales startet die Uhr. Während der Wartezeit bis zum Signal werden die Dezimalpunkte der Anzeige als „Laufpunkte“ angezeigt. Die Anzeigezeit pro Punkt beträgt 125 ms.
- Von den vier LEDs des Bargraphen wird eine von drei LEDs aktiviert – ebenfalls per Zufall.
- Eine der drei Tasten auf dem Bord muss nun gedrückt werden. Die zu drückende Taste wird durch die LED gekennzeichnet. Zuordnung S1 – D1, S2 – D2, S3 – D3.
- Bei richtig betätigter Taste wird die Reaktionszeit in Sekunden mit Dezimalpunkt angezeigt. Eine falsche Tastenbetätigung wird mit einem Signalton von 500 ms signalisiert und es steht FFF im Display.
- Nach Ablauf von 3 Sekunden verlischt die Anzeige, die Laufpunkte signalisieren nun den Start eines neuen Tests.

## Bewertung:
Das Testat wird bei erfolgreicher Präsentation der Software im Labor erteilt. Jede Laborgruppe sendet mir Ihre main.cpp per Email bis 12.00 Uhr des Vortages des Labortermins zu. **Ohne Softwarezusendung kein Testat!**