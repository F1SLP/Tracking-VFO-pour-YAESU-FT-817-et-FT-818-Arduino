# Tracking-VFO-FT-817-FT818-Arduino
Tracking de VFO pour YAESU FT-817 et/ou YAESU FT-818

https://youtu.be/T-83W6KOSfE

Fonctionnement simple et directement  fonctionnel en branchant les deux fiches ACC.

L'arduino peut-etre alimenté par l'arduino et s'assurant d'alimenter l'arduino en dessous de 12V.

La resistance de 10K est optionnelle, mais il est préférable de la  monter.


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


L'interrupteur permet d'inverser le sens du tracking.

Disponible également, le schéma et le fichier arduino pour gérer un afficheur LCD en I2C.
