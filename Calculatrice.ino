/* Name : Multi-line arduino calculator with keypad and lcd screen
 * Author(s) : Sami N. & Luca D.C.
 * 
 */

#include <Keypad.h>
#include "FctET2Nano.h"
#include "Fonction.h"
#include <LiquidCrystal.h>

#define RS A1
#define EN A0
#define D4 12
#define D5 11
#define D6 10
#define D7 A2

#define DIVIDE byte (1)

#define MILLIS 10 //Duree entre deux tours de 10ms ; il permet de gerer l'anti rebond

String calcul = "";


enum Etat {INITIAL, ATTENTE, AFFICHE, CALCUL}; //definition des etats de la machine
enum Etat eEtat;
enum Etat eEtatOld;     // Pour gerer le premier passage dans un etat
boolean boEtatNew ;      // A chaque changement d'etat,se met a true

unsigned long lMillis;  // Compteur de millisecondes

const byte ROWS = 4; //4 Lignes (4 Lines, 4 rows)
const byte COLS = 4; //4 Colonnes (4 columns)
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte divide[8]={
  B00000,
  B00000,
  B00100,
  B00000,
  B11111,
  B00000,
  B00100,
  B00000
};

int iPos = 0;
byte colPins[ROWS] = {6,7,8,9};
byte rowPins[COLS] = {2,3,4,5};

String values[101];
String c = "";

int iCpt = 0;
bool exist = false;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal lcd(RS, EN, D4, D5, D6 , D7);


void setup() {
  clearTab();
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(1,divide);
  eEtat = ATTENTE;
  lMillis = millis() + MILLIS;
}

void loop() {

  while (lMillis >= millis()); // Attente de "MILLIS" ms
  lMillis = lMillis + MILLIS; // Actualise la variable pour le tour suivant

  
  char k = getKEY();
  if((k == 'A' || k == 'B' || k == 'C' || k == 'D') && (calcul.endsWith("x") || calcul.endsWith("/") || calcul.endsWith("+") || calcul.endsWith("-")))return;
  String key = translate(k);
  boEtatNew = (eEtat != eEtatOld) ? true : false;
  eEtatOld = eEtat;
  if(!key.equals("")){
    if(key.equals("*")){
      lcd.clear();
      clearTab();
      c = "";
      calcul = "";
      iPos=0;
   }
   else{
      eEtat=AFFICHE;
    }
  }

  switch (eEtat)
  {
    case INITIAL:
        break;
      case ATTENTE:
       iCpt++;
       if(iCpt>60){
        iCpt=0;
        if(!exist){
          write(iPos,0,"_");
        }
        else{
          write(iPos,0," ");
        };
        exist = !exist;
       }
        break;
      case AFFICHE:
          if(key.equals("/")){
            calcul += key;
            write(iPos,0,DIVIDE);
            iPos++;
          }
          else if(key.equals("=")){
            eEtat = CALCUL;
          }
          else{
            calcul += key;
            write(iPos,0,key);
            iPos++;
          }
          if(!key.equals("="))eEtat=ATTENTE;
        break;
      case CALCUL:
        if(boEtatNew){
          write(iPos,0," ");
          iPos=0;
          answer(calcul);     
        }
        break;

    }
}

char getKEY() {
    return keypad.getKey(); 
}

String translate(char Key){
  int i = 0;
  while(!values[i].equals("\0")){
    i++;
  }
  if(Key == 'A'){
    values[i] = c;
    c = "\0";
    values[i+1] = "+";
    return "+";
  }
  if(Key == 'B'){
    values[i] = c;
    c = "\0";
    values[i+1] = "-"; 
    return "-";
  }
  if(Key == 'C'){
    values[i] = c;
    c = "\0";
    values[i+1] = "x"; 
    return "x";
  }
  if(Key == 'D'){
    values[i] = c;
    c = "\0";
    values[i+1] = "/"; 
    return "/";
  }
  if(Key == '#'){        
    values[i] = c;
    c = "\0";
    return "=";
  }
  else{
    if(Key != 'A' && Key != 'B' && Key != 'C' && Key != 'D' && Key != '#' && Key!= '*' && Key != '\0'){
      c += Key;
    }
    return String(Key);
  }
}

void answer(String line){
  double iC;
  for(int i=1;i<100;i++){
    double value1=values[i-1].toDouble();
    double value2=values[i+1].toDouble();
    String value =values[i];
    if(value.equals("x") || value.equals("/")){
      if(value.equals("x"))iC = value1 * value2;
      if(value.equals("/"))iC = value1 / value2;
      values[i-1] = iC;
      for(int i2=i;i2<100;i2++){
        values[i2] = values[i2+1];
      }
      for(int i2=i;i2<100;i2++){
        values[i2] = values[i2+1];
      }
      i-=2;
    }
    iC=0;
  }
  for(int i=1;i<100;i++){
    double value1=values[i-1].toDouble();
    double value2=values[i+1].toDouble();
    String value =values[i];
    if(value.equals("+") || value.equals("-")){
      if(value.equals("+"))iC = value1 + value2;
      if(value.equals("-"))iC = value1 - value2;
      values[i-1] = iC;
      for(int i2=i;i2<100;i2++){
        values[i2] = values[i2+1];
      }
      for(int i2=i;i2<100;i2++){
        values[i2] = values[i2+1];
      }
      i-=2;
    }
    iC=0;
  }
  clear(-1,1);
  write(0,1,values[0]);
}

void clearTab(){
  for(int i=0;i<100;i++){
    values[i] = "\0";
  }
}


