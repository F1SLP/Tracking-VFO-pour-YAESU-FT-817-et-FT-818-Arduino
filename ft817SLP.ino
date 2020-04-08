/*
    Code de tracking de VFO pour deux Yaesu FT-817 ou FT-818

    F1SLP - Avril 2020

    Version 1.00
*/

#include <SoftwareSerial.h>       // Appel de la bibliothèque RS232

const int RX = 2;                 // Pin d'arduino pour le Port série 1
const int TX = 3;                 // Pin d'arduino pour le Port série 1
const int RX2 = 4;                // Pin d'arduino pour le Port série 2
const int TX2 = 5;                // Pin d'arduino pour le Port série 2

SoftwareSerial RS232(RX, TX);     //Création de l'objet RS232
SoftwareSerial RS232B(RX2, TX2);  //Création de l'objet RS232

byte FINDQRG[] = {0x00, 0x00, 0x00, 0x00, 0x03};  // Commande de questionnement pour obtenir les QRG

bool demarrage = false;
int interPin = 8;                 // Interrupteur trancking inverse
int etatInter;                    // Position de l'interrupteur de tracking
int mot;                          // Ce qui est lu par la RS232
int qrgHexa[5];                   // Tableau de résultat en Hexa de la Fréquence reçu en 5 groupe de 2
String qrgTxt[5];                 // Tableau de l'Hexa transformé en chaine de caractères
long QRGint[5];                   // Tableau de la chaine de caractère transformé en décimale
int qrgTx[4];                     // Chaine de code Hexa à transmettre vers le TRX
long qrgFt817;                    // Valeur long de la QRG du premier FT-817
long qrgFt818;                    // Valeur long de la QRG du deuxième FT-817
String resultat;                  // Fréquence en chaine de caractère reconstituée
long memFreq817;                  // Mémoire de la fréquence du premier FT-817
long memFreq818;                  // Mémoire de la fréquence du premier FT-817
long difference;                  // Calcul du différenciel entre les deux QRG
long upQrg;                       // Différenciel fixe 28600000 Hz entre les deux fréquences
int mhz2;                         // Fréquence de RX Satellite découpée en 2 décimales pour l'injecter dans le qrgHexa[]
String mhz;                       // Fréquence de RX Satellite découpée en 2 décimales pour l'injecter dans le mhz2
int INT;                          // QRG découpée en deux décimales pour l'envoyer dans qrgHexa[]
long offset = 0;
long qrgDepart;


void lireQrg() {                  // Réception en Hexa de la QRG du transceiver d'émission
  RS232.begin(9600);
  for (byte i = 0; i < sizeof(FINDQRG); i++)(RS232.write(FINDQRG[i]));
  //delay(200);

  for (int i = 0; i < 5; i++) {
    while (!RS232.available());
    mot = RS232.read();
    qrgHexa[i] = (mot);
    qrgTxt[i] = String(qrgHexa[i], HEX);
    QRGint[i] = qrgTxt[i].toInt();
  }

  RS232.end();
  qrgFt818 = (QRGint[0] * 1000000) + QRGint[1] * 10000 + QRGint[2] * 100 + QRGint[3];

  if (QRGint[0] < 9) qrgTxt[0] = "0" + qrgTxt[0];
  if (QRGint[1] < 9) qrgTxt[1] = "0" + qrgTxt[1];
  if (QRGint[2] < 9) qrgTxt[2] = "0" + qrgTxt[2];
  if (QRGint[3] < 9) qrgTxt[3] = "0" + qrgTxt[3];

  String resultat;

  for (int i = 0; i < 4; i++) {
    resultat = resultat + (qrgTxt[i]);
  }
}

void lireQrg2() {                   // Réception en Hexa de la QRG du transceiver de réception
  RS232B.begin(9600);
  for (byte i = 0; i < sizeof(FINDQRG); i++)(RS232B.write(FINDQRG[i]));

  for (int i = 0; i < 5; i++) {
    while (!RS232B.available());
    mot = RS232B.read();
    qrgHexa[i] = (mot);
    qrgTxt[i] = String(qrgHexa[i], HEX);
    QRGint[i] = qrgTxt[i].toInt();
  }

  RS232B.end();

  // Construction des fréquences complètes
  qrgFt817 = (QRGint[0] * 1000000) + QRGint[1] * 10000 + QRGint[2] * 100 + QRGint[3];

  if (QRGint[0] < 9) qrgTxt[0] = "0" + qrgTxt[0];
  if (QRGint[1] < 9) qrgTxt[1] = "0" + qrgTxt[1];
  if (QRGint[2] < 9) qrgTxt[2] = "0" + qrgTxt[2];
  if (QRGint[3] < 9) qrgTxt[3] = "0" + qrgTxt[3];

  String resultat;
  resultat = "";

  for (int i = 0; i < 4; i++) {
    resultat = resultat + (qrgTxt[i]);
  }
}

void envoyerQrg() {                   // Envoi au transceiver RX la fréquence de réception calculée
  RS232B.begin(9600);

  INT = qrgHexa[0];
  RS232B.write(((INT / 10) << 4) + (INT % 10));
  INT = qrgHexa[1];
  RS232B.write(((INT / 10) << 4) + (INT % 10));
  INT = qrgHexa[2];
  RS232B.write(((INT / 10) << 4) + (INT % 10));
  INT = qrgHexa[3];
  RS232B.write(((INT / 10) << 4) + (INT % 10));
  INT = 1;
  RS232B.write(((INT / 10) << 4) + (INT % 10));

  RS232B.end();
}

void tableauQRG() {                    // Calcul de la fréquence de RX calculé et préparation de l'envoi en Hexa
  
  if (etatInter == LOW) {
  upQrg = (qrgFt818 + difference);
  Serial.println(difference);
  }
  else
  {
  upQrg = (difference);
  }
  
  String stringUpQrg = String(upQrg);

  mhz = String(upQrg).substring(0, 2);
  mhz2 = mhz.toInt();
  qrgHexa[0] = (mhz2);

  mhz = String(upQrg).substring(2, 4);
  mhz2 = mhz.toInt();
  qrgHexa[1] = (mhz2);

  mhz = String(upQrg).substring(4, 6);
  mhz2 = mhz.toInt();
  qrgHexa[2] = (mhz2);

  mhz = String(upQrg).substring(6, 8);
  mhz2 = mhz.toInt();
  qrgHexa[3] = (mhz2);
}

void testDemarrage() {
  if (demarrage == false) {
    difference = qrgFt817 - qrgFt818;
    qrgDepart = qrgFt818;
     //Serial.println(qrgDepart);
    demarrage = true;
  }
}

void setup() {                          // Déclaration des ports de l'Arduino
  Serial.begin(9600);
  pinMode(RX, INPUT);
  pinMode(RX2, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(TX2, OUTPUT);
  pinMode(interPin, INPUT);
}

void loop() {                           // Programme principal

  etatInter = digitalRead(interPin);
  lireQrg();
  delay(10);
  lireQrg2();
  delay(10);

  testDemarrage();

  if (etatInter == LOW) {                 // Mode tracking normal
    
    if (memFreq817 != qrgFt817) {         // Vérification si la fréquence du Ft817 a changé
      memFreq817 = qrgFt817;
    }

    if (memFreq818 != qrgFt818) {         // Vérification si la fréquence du Ft818 a changé
        memFreq818 = qrgFt818;
        tableauQRG();
        delay(10);
        envoyerQrg();
    }
    else {
      lireQrg2();
      difference = qrgFt817 - qrgFt818;
    }
    
  }

  else {                                  // Mode tracking inverse
    
    if (memFreq818 != qrgFt818) {         // Vérification si la fréquence du Ft818 a changé
        memFreq818 = qrgFt818;    
        lireQrg();
        offset = qrgDepart - qrgFt818;
        difference = qrgFt817 - qrgFt818 + qrgFt818 + offset;
        Serial.println(difference);
        tableauQRG();
        delay(10);
        envoyerQrg();
        qrgDepart = qrgFt818;
        }   
      }
      
delay(90);
}
