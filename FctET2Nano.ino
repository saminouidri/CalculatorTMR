/*  FoctET2Nano.ino pour utilisation du circuit ET2Nano
 *  Les fonctions propres au projets sont a mettre dans les fichiers "Fonction.h" et .ino 
 *  Fred Sculati & Jean-Pierre Kolly
 *  mai 2016
 */
#include "FctET2Nano.h"

// Fonction Antirebond ******************************************************************
byte Antirebond (int iEntree,byte bRead){
  boolean boInput = 0;
  
  if(bRead & ACTIF_A_1){        // Si l'entrée utilise une pulldown (actif à 1)
    boInput = digitalRead(iEntree);
    bRead = bRead ^ ACTIF_A_1;  // Remet à 0 le bit indiquant "actif à 1)
  }
  else{
    boInput = !digitalRead(iEntree);
  }
  
  if(boInput){ // Si l'entrée est active
    switch (bRead){
      case ACTIF          : 
      case FLANC_ACTIF    : return ACTIF;
      
      case INACTIF        : return JUSTE_ACTIF;
      case JUSTE_ACTIF    : return FLANC_ACTIF;
    }
  }
  else{       // Donc l'entrée est inactive
    switch (bRead){
      case INACTIF        :
      case FLANC_INACTIF  : return INACTIF;
      
      case ACTIF          : return JUSTE_INACTIF;
      case JUSTE_INACTIF  : return FLANC_INACTIF;
    }
  }
}

// Fonctions InitAff ********************************************************************
void InitAff()
{
  byte bCpt;  //Compteur pour l'initialisation
  
  pinMode(STCP, OUTPUT);
  pinMode(DS,   OUTPUT);
  pinMode(SHCP, OUTPUT);
  
  digitalWrite(STCP,LOW);
  digitalWrite(SHCP,LOW);
  digitalWrite(DS, HIGH); // Affichage à anode commune : un HIGH etient le segment
  for(bCpt = 0; bCpt < 8; bCpt++) // Efface l'affichage
  {
    digitalWrite (SHCP, HIGH);  // Impulsions d'horloge pour 8 décalages
    digitalWrite (SHCP, LOW);
  }
  digitalWrite (STCP, HIGH);
  digitalWrite (STCP, LOW);
}

// Fonctions Aff ************************************************************************
void Aff(char cAff) // envoie le code 7 segment correspondant sur l'affichage
{
  byte bMasque = 0b10000000;  // Masque pour isoler 1 sur 8 bits
  const byte t7seg[16] =      // Table code 7 segments
  {//   abcdefgp
      0b00000011, // 0
      0b10011111, // 1
      0b00100101, // 2
      0b00001101, // 3
      0b10011001, // 4
      0b01001001, // 5
      0b01000001, // 6
      0b00011111, // 7
      0b00000001, // 8
      0b00001001, // 9
      0b00010001, // A
      0b11000001, // b
      0b01100011, // C
      0b10000101, // d
      0b01100001, // E
      0b01110001  // F    
  };
  static bool boTmpSHCP;  // Pour sauver temporairement la sortie SHCP
  
  boTmpSHCP = digitalRead(SHCP);  // Sauve la sortie utilisee par le latch
  digitalWrite(SHCP,LOW);
  while(bMasque)  // Envoi des 8 bits en serie
  {
    if(bMasque & t7seg[cAff])   // Si le bit courant est a 1 :
    {
      digitalWrite(DS,HIGH);   // on met 1 sur la sortie de donnee
    }
    else
    {
      digitalWrite(DS,LOW);    // sinon on met 0
    }
    digitalWrite(SHCP,HIGH);
    digitalWrite(SHCP,LOW);
    bMasque = bMasque >> 1;      // Pour passer au bit suivant
  }
  digitalWrite(STCP,HIGH);
  digitalWrite(STCP,LOW);
  
  digitalWrite(DS,LOW);
  digitalWrite(SHCP,boTmpSHCP);  // Restaure la sortie utilisee par le latch
}

//Fonction pour tester la carte ET2Nano **************************************************
void TstET2Nano(void)
{
  static byte bCompteur = 0;
  static byte b20ms = 0;

  pinMode(A0,INPUT_PULLUP); // Configuration des 8 entrees
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  pinMode(A3,INPUT_PULLUP);
  pinMode(A4,INPUT_PULLUP);
  pinMode(A5,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(2,OUTPUT);        // Configuration des 8 sorties
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(2,!digitalRead(A0));  // Les leds = l'etat des boutons (sans anti-rebond)
  digitalWrite(3,!digitalRead(A1));
  digitalWrite(4,!digitalRead(A2));
  digitalWrite(5,!digitalRead(A3));
  digitalWrite(6,!digitalRead(A4));
  digitalWrite(7,!digitalRead(A5));
  digitalWrite(8,!digitalRead(11));
  digitalWrite(9,!digitalRead(12));
  
  b20ms++;
  if (b20ms > 25)     // 25 * 20 = toutes les 500 ms on incrémente le compteur
  {
    Aff(bCompteur);     // Affiche les valeurs de 0 a F sur le 7 segments
    digitalWrite(9,!digitalRead(12)); // Par ce que Aff modifie la sortie 9
    b20ms = 0;
    if(bCompteur < 15)
    {
      bCompteur ++;
    }
    else
    {
      bCompteur = 0;
    }
  }
}
