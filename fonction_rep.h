#ifndef _LISTE_AUTRE

#define _LISTE_AUTRE

#include <stdio.h>

#include "Fonction_liste.h"




//! Differents etats definis dans l'automate

typedef enum ETAT {INIT, TEXT, DATA, BSS, TEXT_SYMB, TEST_OP, SEPA_OP, TEXT_COLLE_SYMB, DATA_DIRECTIVE, DATA_BYTE, DATA_WORD, DATA_ASCIIZ, DATA_SPACE, DATA_COLLE_SYMB, BSS_SPACE, DATA_CHECK_SEPA, BSS_CHECK_SEPA, DATA_COLLE_BSS} ETAT; 





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

        char mot[20];				

	LEXEME lex;

	int nb_op;

	int ligne;

	int decalage;

	L_LEX operande;

   	struct DATA* suiv; };



typedef struct DATA* L_DATA;





struct BSS {

        char mot[20];				

	LEXEME lex;

	int nb_op;

	int ligne;

	int decalage;

	L_LEX operande;

   	struct BSS* suiv; };



typedef struct BSS* L_BSS;





struct SYMB {

	MOT mot;

	int ligne;

	int section;  		//! 1 --> bss, 2 --> data, 3 --> text

	int decalage;		//! decalage relatif dans la section

	struct SYMB* suiv; };



typedef struct SYMB* L_SYMB;




//! Prototypes des fonctions utilisees 

L_INST ajout_queue_inst(L_INST l_inst, MOT mot, LEXEME lex, int nb_op, int ligne, int decalage, L_LEX operande);

L_DATA ajout_queue_data(L_DATA l_data, char mot[20], LEXEME lex, int nb_op, int ligne, int decalage, L_LEX operande);

L_BSS ajout_queue_bss(L_BSS l_bss, char mot[20], LEXEME lex, int nb_op, int ligne, int decalage, L_LEX operande);

L_SYMB ajout_queue_symb(L_SYMB l_symb, MOT mot, int ligne, int section, int decalage);

void recup_mot(MOT mot, char tab[]);

int symb_existe(MOT mot);

int verification_word_byte(MOT mot, LEXEME lex, ETAT A);

int nbre_cara(MOT mot);

int valeur_deci(MOT mot, LEXEME lex);



#endif
