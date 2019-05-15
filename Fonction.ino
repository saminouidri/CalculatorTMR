/*  Fonction.ino
 *  Fonctions du projet
 *  Jean-Pierre Kolly
 *  Janvier 2017
 */
#include "Fonction.h"
void write(int x, int y, String aff){//Fonction pour afficher un String sur le lcd
  if(x>15){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Limite  atteinte");
  }
  else{
    lcd.setCursor(x,y);
    lcd.print(aff);
  }
}
void write(int x, int y, byte aff){
    if(x>15){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Limite  atteinte");
  }
  else{
    lcd.setCursor(x,y);
    lcd.write(aff);
  }

}

void clear(int x, int y){
  if(x == -1){
    for(int i=0;i<16;i++){
      lcd.setCursor(i,y);
      lcd.print(" ");
    }
  }
  if(y == -1){
    for(int i=0;i<16;i++){
      lcd.setCursor(x,i);
      lcd.print(" ");
    }
  }
  lcd.setCursor(x,y);
  lcd.print(" ");
}

