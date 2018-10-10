#ifndef _LISTE_LEX
#define _LISTE_LEX
#include <stdio.h>



//! Different types de lexemes obtenables
typedef enum LEXEME {DEBUT, COMMENT, NL, SYMBOLE, DEUX_PTS, DIRECTIVE, VIRGULE, DECIMAL, DECIMAL_ZERO, HEXA, AUTRE, REGISTRE, GUIL, PAR_O, PAR_F} LEXEME; 


//! Liste chainee qui forme un mot propre a un lexeme 
struct LETTRE {
	int c;				//! Un des caracteres du mot (ex : A de ADD)
   	struct LETTRE* suiv; };		//! Chainage

typedef struct LETTRE* MOT;


//!Liste chainee qui comporte un mot et son lexeme associe pour chaque bloc (ex : SYMBOLE --> mot possible associÃ© : ADD)
struct ELEMENT {
   	LEXEME lex;
    	MOT mot;
   	struct ELEMENT* suiv; };

typedef struct ELEMENT* L_LEX;


//! Prototypes des fonctions utilisees 
L_LEX creer_liste_lex(void);
MOT creer_liste_mot(void);
L_LEX ajout_tete_lex(L_LEX l, LEXEME lex, MOT l_mot);
L_LEX ajout_queue_lex(L_LEX l, LEXEME lex, MOT l_mot);
MOT ajout_tete_mot(MOT l_mot, int c);
MOT ajout_queue_mot(MOT l_mot, int c);
L_LEX erreur_carac();
void affiche_lex(L_LEX lex);
void affiche_mot(L_LEX p_lex);
void Virgule(L_LEX lex, MOT *mot);
void New_Line(L_LEX lex, MOT *mot);
void Par_o(L_LEX lex, MOT *mot);
void Par_f(L_LEX lex, MOT *mot);
void Deux_pts(L_LEX lex, MOT *mot);
void Fin_nbre(int c, L_LEX* lex, MOT* mot, LEXEME type_lexeme, LEXEME* A);
int test_registre( int p, int u);


#endif
