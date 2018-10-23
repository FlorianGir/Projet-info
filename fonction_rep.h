#ifndef _LISTE_AUTRE
#define _LISTE_AUTRE
#include <stdio.h>
#include "Fonction_liste.h"

typedef enum ETAT {INIT, TEXT, DATA, BSS, TEXT_SYMB, TEST_OP, SEPA_OP, DATA_DIRECTIVE, DATA_BYTE, DATA_WORD, DATA_ASCIIZ, DATA_SPACE, BSS_SPACE, DATA_CHECK_SEPA, BSS_CHECK_SEPA} ETAT; 

struct INST {
        MOT mot;
	LEXEME lex;
	int nb_op;
	int ligne;
	int decalage;
	L_LEX operande;
   	struct INST* suiv; };

typedef struct INST* L_INST;


struct DATA {
        char mot[20];				//! mettre un char par ce que voila
	LEXEME lex;
	int nb_op;
	int ligne;
	int decalage;
	L_LEX operande;
   	struct DATA* suiv; };

typedef struct DATA* L_DATA;


struct BSS {
        char mot[20];				//! mettre un char par ce que voila
	LEXEME lex;
	int nb_op;
	int ligne;
	int decalage;
	L_LEX operande;
   	struct BSS* suiv; };

typedef struct BSS* L_BSS;





//! Prototypes des fonctions utilisees 

L_INST ajout_queue_inst(L_INST l_inst, MOT mot, int nb_op, int ligne, int decalage, L_LEX operande);
L_DATA ajout_queue_data(L_DATA l_data, char mot[20], int nb_op, int ligne, int decalage, L_LEX operande);
L_BSS ajout_queue_bss(L_BSS l_bss, char mot[20], int nb_op, int ligne, int decalage, L_LEX operande);
void recup_mot(MOT mot, char tab[]);
int nbre_cara(MOT mot);
int valeur_deci(MOT mot, LEXEME lex);


#endif
