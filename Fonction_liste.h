#ifndef _LISTE_LEX
#define _LISTE_LEX
#include <stdio.h>



// Different types de lexemes obtenables
typedef enum LEXEME {DEBUT, COMMENT, NL, SYMBOLE, DEUX_PTS, DIRECTIVE, VIRGULE, DECIMAL, DECIMAL_ZERO, HEXA, AUTRE, REGISTRE} LEXEME; 


// Liste chainee qui forme un mot propre a un lexeme 
struct LETTRE {
	int c;				// Un des caracteres du mot (ex : A de ADD)
   	struct LETTRE* suiv; };		// Chainage

typedef struct LETTRE* MOT;


//Liste chainee qui comporte un mot et son lexeme associe pour chaque bloc (ex : SYMBOLE --> mot possible associé : ADD)
struct ELEMENT {
   	LEXEME lex;
    	MOT mot;
   	struct ELEMENT* suiv; };

typedef struct ELEMENT* L_LEX;


// Prototypes des fonctions utilisees 
L_LEX creer_liste_lex(void);
MOT creer_liste_mot(void);
L_LEX ajout_tete_lex(L_LEX l, LEXEME lex, MOT l_mot);
L_LEX ajout_queue_lex(L_LEX l, LEXEME lex, MOT l_mot);
MOT ajout_tete_mot(MOT l_mot, int c);
MOT ajout_queue_mot(MOT l_mot, int c);
void affiche_lex(L_LEX lex);
void affiche_mot(L_LEX p_lex);


#endif

