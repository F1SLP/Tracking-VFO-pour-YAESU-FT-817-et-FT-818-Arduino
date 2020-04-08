# Tracking-VFO-FT-817-FT818-Arduino
Tracking de VFO pour YAESU FT-817 et/ou YAESU FT-818

https://youtu.be/T-83W6KOSfE

Fonctionnement simple et directement  fonctionnel en branchant les deux fiches ACC.

L'arduino peut-etre alimenter directement par le + 13,8 V du connecteur ACC si les diodes 1N4001 sont monté comme sur le schéma.

La resistance de 10K est optionnelle, mais il est préférable de la monter.

Branchement des connecteurs :


Connecteur ACC 1 :


ACC       Arduino

TX--Broche  D2

RX - Broche D3

GND - GND



Connecteur ACC 2 :

ACC Arduino
TX - Broche 4

RX - Broche 5

GND - GND


L'interrupteur permet d'inverser le sens de Tracking .
