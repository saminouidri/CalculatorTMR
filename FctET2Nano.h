/*  FctET2Nano.h pour utilisation du circuit ET2Nano
 *  Les fonctions propres au projets sont a mettre dans les fichiers "Fonction.h" et .ino 
 *  Fred Sculati & Jean-Pierre Kolly
 *  mai 2016
 */
#ifndef _FctET2Nano
#define _FctET2Nano

  // definition des valeurs passees et reçu par la fontion "Antirebond"
  #define INACTIF         0 // 0: L'entree est non actionnee
  #define FLANC_INACTIF   1 // 1: L'entree passe en position de repos
  #define ACTIF           2 // 2: L'entree est active
  #define FLANC_ACTIF     3 // 3: L'entree passe a l'etat actif
  #define JUSTE_ACTIF     6 // 6: L'entree est active avant antirebond (ne pas utiliser)
  #define JUSTE_INACTIF   9 // 9: L'entree est inactive avant antirebond (ne pas utiliser)
  #define ACTIF_A_1 0b10000000 // A utiliser en faisant un "|" avec le byte en paramètre
                               // permet d'avoir une entree active a 1 (pulldown)
							   
  // definition des broches utilisees par l'affichage
  #define STCP 10 // Latch
  #define DS   13 // Data
  #define SHCP  9 // Clock

  void TstET2Nano(void);  // Fonction permettant de tester les entrees/sorties et l'affichage
                          // Elle copie les etats des interrupteurs sur les led et affiche de 0 a F
                          
  // Fonction gerant les flancs et les etats d'interrupteurs avec antirebond  
  byte Antirebond(int,byte);  // Lecture des entrees. Le paramètre "int" est l'entree a traiter
                              // Voir les defines pour "byte"
                              // La valeur reçu correspond a l'etat avant l'appel de la fonction
							  
  // Fonction gerant l'affichage 7 segments commande par un 74HC595
  void Aff(char); // Prototype de la fonction Aff
  void InitAff(); // Fonction d'init des sorties pour Aff et effacement de l'affichage

#endif

