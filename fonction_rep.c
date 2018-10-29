#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "fonction_rep.h"

#include "Fonction_liste.h"





void repartition(L_LEX l_lex, L_INST l_inst, L_DATA l_data, L_BSS l_bss, L_SYMB l_symb){

	

	L_LEX p, attente_op;

	L_INST q;

	ETAT A = INIT;

	ETAT A_memory;

	char* c;
	char tab[100];

	int existe, verif_cond;

	

	int dec_bss = 0;			//! Decalages 

	int dec_text = 0;

	int dec_data = 0;





	attente_op = NULL;

	p = l_lex;

	while (p != NULL) {

		

		switch(A) {

			

		case INIT:

		

			recup_mot(p->mot, tab);				//! recuperation du mot associé au premier lexeme

			

			if (strcmp(c, ".text") == 0) A = TEXT;

			else if (strcmp(c, ".data") == 0) A = DATA;

			else if (strcmp(c, ".bss") == 0) A = BSS;

			p = p->suiv;

			break;

			

			

		case TEXT:				//! quand on est dans .text 

			

			recup_mot(p->mot, tab);

			

			if (strcmp(c, ".data") == 0) A = DATA, p = p->suiv;

			else if (strcmp(c, ".bss") == 0) A = BSS, p = p->suiv;

			else if (p->suiv->mot->c == ':') A = TEXT_COLLE_SYMB;   //! 	LES IF S EFFECTUENT PAR ORDRE D ECRITURE ?

			else if (p->lex == SYMBOLE) A = TEXT_SYMB;

			else A = TEXT, p = p->suiv;

			break;

			

			

		case DATA:				//! quand on est dans .data

		

			recup_mot (p->mot, tab);

			

			if (strcmp(c, ".text") == 0) A = TEXT, p = p->suiv;

			else if (strcmp(c, ".bss") == 0) A = BSS, p = p->suiv;

			else if (p->lex == DIRECTIVE) A = DATA_DIRECTIVE;

			else if (p->suiv->mot->c == ':') A = DATA_COLLE_SYMB;

			break;

		

		

		case BSS:				//! quand on est dans .bss

		

			recup_mot (p->mot, tab);

			

			if (strcmp(c, ".text") == 0) A = TEXT;

			else if (strcmp(c, ".data") == 0) A = DATA;

			else if (strcmp(c,".space") == 0) A = BSS_SPACE;

			p = p->suiv;

			break;

		

		

		case TEXT_SYMB:				//! on est dans .text et on a un symbole

			

			existe = symb_existe(p->mot);	



			if (existe == -1) {

				printf("Ligne %d: erreur d'instruction dans la partie text \n", p->ligne);

				return;

			}

			else {

				A = TEST_OP ;

				l_inst = ajout_queue_inst(l_inst, p->mot, SYMBOLE, existe, p->ligne, DECALAGE, NULL);  //! SYMBOLE ? + gérer decalage

			}

			p = p->suiv;

			break;





		case TEXT_COLLE_SYMB:

	

			l_symb = ajout_queue_symbole(l_symb, p->mot, p->ligne, ".text", decalage); //! TRAITER DECALAGE RELATIF

			A = TEXT;

			p = p->suiv;

			break;

		

			

		case TEST_OP: 			//! le symbole existe, test des operandes

			

			if ((existe > 1)&&(p->lex == (REGISTRE||DECIMAL||DECIMAL_ZERO||HEXA))){			//! lorsqu il y a encore plus de une operande

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				A = SEPA_OP;

				existe-- ;

			}

			else if ((existe == 1)&&(p->lex == (REGISTRE||DECIMAL||DECIMAL_ZERO||HEXA))){		//! lorsqu il reste que une operande

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				q = l_inst;

				while(q->suiv != NULL){

					q = q->suiv;

				}

				q->operande = attente_op;			//! revoir vu que le maillon est en queue

				attente_op = NULL;

				A = TEXT;

			}

			else if (existe == 0){				        //! lorsqu il n y a pas d operande

				A = TEXT;

			}

			else {

				printf("Ligne %d: erreur dans les operandes dans une instruction de la partie text\n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

		

		

		case SEPA_OP:						//! test du separateur d'operande

			

			if ( p->lex == (VIRGULE)) A = TEST_OP;

			else {

				printf("Ligne %d: erreur de separateur dans une instruction de la partie text \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

			



		case DATA_DIRECTIVE:					//! on est dans .data et on a une directive

		

			if (strcmp(c, ".byte") == 0) A = DATA_BYTE;

			else if (strcmp(c, ".word") == 0) A = DATA_WORD;

			else if (strcmp(c, ".asciiz") == 0) A = DATA_ASCIIZ;

			else if (strcmp(c, ".space") == 0) A = DATA_SPACE;

			else {

				printf("Ligne %d: erreur de directive inexistante dans la partie data \n", p->ligne);

				return;

			}

			A_memory = A;

			p = p->suiv;

			break;

		

		

		case DATA_BYTE:					//! cas ou la directive est .byte

		

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				verif_cond = verification_word_byte(p->mot, p->lex, A);		//! fonction qui verifie les conditions operande de byte, renvoie 0 si c'est bon et 1 si non

				if (verif_cond == 0){

					dec_data += 1;			//! Decalage de 1 pour le .byte

					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					l_data = ajout_queue_data(l_data, ".byte", DIRECTIVE, dec_data, p->ligne, attente_op); //! NOMBRE OPERANDE ?

					attente_op = NULL;

					A = DATA_CHECK_SEPA;

				}

				else {

					printf("Ligne %d: erreur d'operande dans le .byte de la partie data \n", p->ligne);

					return;

				}

			}

			else {

				printf("Ligne %d: erreur d'operande dans la partie data \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

			

		

		case DATA_WORD:				//! cas ou la directive est .word

		

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				verif_cond = verification_word_byte(p->mot, p->lex, A);				//! creer fonction qui verifie les conditions operande de word, renvoie 0 si c'est bon et 1 si non

				if (verif_cond == 0){

					dec_data += 4;			//! Decalage de 4 pour le .word

					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					l_data = ajout_queue_data(l_data,".word", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

					attente_op = NULL;

					A = DATA_CHECK_SEPA;

				}

				else {

					printf("Ligne %d: erreur d'operande dans le .word de la partie data \n", p->ligne);

					return;

				}

			}

			else {

				printf("Ligne %d: erreur d'operande dans la partie data \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

			

			

		case DATA_ASCIIZ:			//! cas ou la directive est .asciiz

		

			if ( p->lex == GUIL) {

				dec_data += nbre_cara(p->mot) - 1; 

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_data = ajout_queue_data( l_data, ".asciiz", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

				attente_op = NULL;

				A = DATA_CHECK_SEPA;

			}

			else {

				printf("Ligne %d: erreur d'operande dans la partie data \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

			

		

		case DATA_SPACE:			//! cas ou la directive est .space

		

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				dec_data += valeur_deci(p->mot, p->lex);			//! Decalage de c pour le .space

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_data = ajout_queue_data( l_data, ".space", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

				attente_op = NULL;

				A = DATA_CHECK_SEPA;

			}

			else {

				printf("Ligne %d: erreur d'operande dans la partie data \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

		

		

		case DATA_CHECK_SEPA:

		

			if ( p->lex == (VIRGULE)) {

				A = A_memory;

			}

			else if ( p->lex == (NL||COMMENT)){

				A = DATA;

			}

			else {

				printf("Ligne %d: erreur de separateur dans la partie data \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

		

		case DATA_COLLE_SYMB: 

		

			

			l_symb = ajout_queue_symbole(l_symb, p->mot, p->ligne, ".data", decalage); //! TRAITER DECALAGE RELATIF

			A = DATA;

			p = p->suiv;

			break;

		

		

		case BSS_SPACE:				//! on est dans .bss et on a un .space

			

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				dec_bss += valeur_deci(p->mot, p->lex);   

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_bss = ajout_queue_bss( l_bss, ".space", DIRECTIVE, 1, p->ligne, DECALAGE, attente_op);

				attente_op = NULL;

				A = BSS_CHECK_SEPA;

			}

			else {

				printf("Ligne %d: erreur d'operande dans la partie bss \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

			

		

		case BSS_CHECK_SEPA:

			

			if ( p->lex == (VIRGULE)) {

				A = BSS_SPACE;

			}

			else if ( p->lex == (NL||COMMENT)){

				A = BSS;

			}

			else {

				printf("Ligne %d: erreur de separateur dans la partie bss \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

		

		}

	}

	return;



		

			

}
