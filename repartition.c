#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "fonction_rep.h"

#include "Fonction_liste.h"





void repartition(L_LEX l_lex, L_INST l_inst, L_DATA l_data, L_BSS l_bss){

	

	L_LEX p, attente_op;

	L_INST q;

	ETAT A = INIT;
	ETAT A_memory;

	char* c;

	int existe, verif_cond;

	
	int dec_bss = 0;			//! Decalages 
	int dec_text = 0;
	int dec_data = 0;



	attente_op = NULL;

	p = l_lex;

	while (p != NULL) {

		

		switch(A) {

			

		case INIT:

		

			c = recup_mot(p->mot);				//! recuperation du mot associé au premier lexeme

			

			if (strcmp(c, ".text") == 0) A = TEXT;

			else if (strcmp(c, ".data") == 0) A = DATA;

			else if (strcmp(c, ".bss") == 0) A = BSS;

			p = p->suiv;

			break;

			

			

		case TEXT:				//! quand on est dans .text 

			

			c = recup_mot (p->mot);

			

			if (strcmp(c, ".data") == 0) A = DATA, p = p->suiv;

			else if (strcmp(c, ".bss") == 0) A = BSS, p = p->suiv;

			else if (p->lex == SYMBOLE) A = TEXT_SYMB;

			else A = TEXT, p = p->suiv;

			break;

			

			

		case DATA:				//! quand on est dans .data

		

			c = recup_mot (p->mot);

			

			if (strcmp(c, ".text") == 0) A = TEXT, p = p->suiv;

			else if (strcmp(c, ".bss") == 0) A = BSS, p = p->suiv;

			else if (p->lex == DIRECTIVE) A = DATA_DIRECTIVE;

			break;

		

		

		case BSS:				//! quand on est dans .bss

		

			c = recup_mot (p->mot);

			

			if (strcmp(c, ".text") == 0) A = TEXT;

			else if (strcmp(c, ".data") == 0) A = DATA;

			else if (strcmp(c,".space") == 0) A = BSS_SPACE;

			p = p->suiv;

			break;

		

		

		case TEXT_SYMB:				//! on est dans .text et on a un symbole

			

			existe = symb_existe(p->mot);		//! Faire la fonction qui regarde si le symbole fait partie du dico, si oui renvoie le nombre d'operande, sinon -1

			

			if (existe == -1) {

				printf(" erreur d'instruction dans la partie text \n");

				return;

			}

			else {
				A = TEST_OP ;
				l_inst = ajout_queue_inst( l_inst, p->mot, existe, p->ligne, DECALAGE, NULL);
			}

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

				printf(" Erreur dans les operandes dans une instruction de la partie text\n");

				return;

			}

			p = p->suiv;

			break;

		

		

		case SEPA_OP:						//! test du separateur d'operande

			

			if ( p->lex == (VIRGULE)) A = TEST_OP;

			else {

				printf(" Erreur de separateur dans une instruction de la partie text \n");

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

				printf(" erreur de directive inexistante dans la partie data \n");

				return;

			}
			A_memory = A;

			p = p->suiv;

			break;

		

		

		case DATA_BYTE:					//! cas ou la directive est .byte

		

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				verif_cond = verification_byte( p->mot );				//! creer fonction qui verifie les conditions operande de byte, renvoie 0 si c'est bon et 1 si non

				if (verif_cond == 0){
					dec_data += 1;			//! Decalage de 1 pour le .byte

					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					l_data = ajout_queue_data( l_data, ".byte", DIRECTIVE, dec_data , p->ligne, , attente_op);

					attente_op = NULL;

					A = DATA_CHECK_SEPA;

				}

				else {

					printf(" Erreur d'operande dans le .byte de la partie data \n");

					return;

				}

			}

			else {

				printf(" Erreur d'operande dans la partie data \n");

				return;

			}

			p = p->suiv;

			break;

			

		

		case DATA_WORD:				//! cas ou la directive est .word

		

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				verif_cond = verification_word( p->mot );				//! creer fonction qui verifie les conditions operande de word, renvoie 0 si c'est bon et 1 si non

				if (verif_cond == 0){
					dec_data += 4;			//! Decalage de 4 pour le .word

					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					l_data = ajout_queue_data( l_data, ".word", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

					attente_op = NULL;

					A = DATA_CHECK_SEPA;

				}

				else {

					printf(" Erreur d'operande dans le .word de la partie data \n");

					return;

				}

			}

			else {

				printf(" Erreur d'operande dans la partie data \n");

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

				printf(" Erreur d'operande dans la partie data \n");

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

				printf(" Erreur d'operande dans la partie data \n");

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

				printf(" Erreur de separateur dans la partie data \n");

				return;

			}

			p = p->suiv;

			break;

		

		

		case BSS_SPACE:				//! on est dans .bss et on a un .space

			

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {
				dec_bss += valeur_deci(p->mot, p->lex);   //! A FAIIIIIIIIIIIIIIIIIIIIIRE

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_bss = ajout_queue_bss( l_bss, ".space", DIRECTIVE, 1, p->ligne, DECALAGE, attente_op);

				attente_op = NULL;

				A = BSS_CHECK_SEPA;

			}

			else {

				printf(" Erreur d'operande dans la partie bss \n");

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

				printf(" Erreur de separateur dans la partie bss \n");

				return;

			}

			p = p->suiv;

			break;

		

		}

	}

	return;

}
