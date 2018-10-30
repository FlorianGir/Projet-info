#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "fonction_rep.h"

#include "Fonction_liste.h"




void repartition(L_LEX l_lex, L_INST l_inst, L_DATA l_data, L_BSS l_bss, L_SYMB l_symb){

	

	L_LEX p, attente_op, l_etiquette;

	L_INST q;

	ETAT A = INIT;

	ETAT A_memory;


	char tab[100];

	int existe, verif_cond;
	

	//! Decalages 

	int dec_bss = 0;			

	int dec_text = 0;

	int dec_data = 0;



	

	attente_op = NULL;

	p = l_lex;
	l_etiquette = memory_etiquette(l_lex);

	while (p != NULL) {

		

		switch(A) {

			

		case INIT:

			printf("INIT\n");
			

			recup_mot(p->mot, tab);				//! recuperation du mot associé au premier lexeme

			

			if (strcmp(tab, ".text") == 0) A = TEXT;

			else if (strcmp(tab, ".data") == 0) A = DATA;

			else if (strcmp(tab, ".bss") == 0) A = BSS;

			p = p->suiv;

			break;

			

			

		case TEXT:				//! quand on est dans .text 

			printf("TEXT\n");

			recup_mot(p->mot, tab);

			

			if (strcmp(tab, ".data") == 0) A = DATA, p = p->suiv;

			else if (strcmp(tab, ".bss") == 0) A = BSS, p = p->suiv;

			else if ((p->suiv)->lex == DEUX_PTS) A = TEXT_COLLE_SYMB;   

			else if (p->lex == SYMBOLE) A = TEXT_SYMB;

			else A = TEXT, p = p->suiv;

			break;

			

			

		case DATA:				//! quand on est dans .data

			printf("DATA\n");

			recup_mot (p->mot, tab);

			

			if (strcmp(tab, ".text") == 0) A = TEXT, p = p->suiv;

			else if (strcmp(tab, ".bss") == 0) A = BSS, p = p->suiv;

			else if (p->lex == DIRECTIVE) A = DATA_DIRECTIVE;

			else if ((p->suiv)->lex == DEUX_PTS) A = DATA_COLLE_SYMB;

			break;

		

		

		case BSS:				//! quand on est dans .bss

			printf("BSS\n");

			recup_mot (p->mot, tab);

			

			if (strcmp(tab, ".text") == 0) A = TEXT;

			else if (strcmp(tab, ".data") == 0) A = DATA;

			else if (strcmp(tab,".space") == 0) A = BSS_SPACE;
			else if ((p->suiv)->lex == DEUX_PTS) A = DATA_COLLE_BSS;

			p = p->suiv;

			break;

		

		

		case TEXT_SYMB:				//! on est dans .text et on a un symbole

			printf("TEXT_SYMB\n");

			existe = symb_existe(p->mot);
			existe -= 48;	

			if (existe == -1) {

				printf("Ligne %d: erreur d'instruction dans la partie text \n", p->ligne);

				return;

			}

			else {
				

				A = TEST_OP;

				l_inst = ajout_queue_inst(l_inst, p->mot, SYMBOLE, existe, p->ligne, dec_text, NULL); 
				dec_text += 4;

			}

			p = p->suiv;

			break;





		case TEXT_COLLE_SYMB:

			printf("TEXT_COLLE_SYMB\n");

			l_symb = ajout_queue_symb(l_symb, p->mot, p->ligne, 3, dec_text); 

			A = TEXT;
			p = p->suiv;

			break;

		

			

		case TEST_OP: 			//! le symbole existe, test des operandes

			printf("TEST_OP, %d \n", existe);			

			if (existe > 1) {
				if (p->lex == REGISTRE || p->lex == DECIMAL || p->lex == DECIMAL_ZERO || p->lex == HEXA ||   {		//! lorsqu il y a encore plus de une operande
					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					A = SEPA_OP;

					existe-- ;
				}

			}

			else if (existe == 1) {
				if (p->lex == REGISTRE || p->lex == DECIMAL || p->lex == DECIMAL_ZERO || p->lex == HEXA) {		//! lorsqu il reste que une operande

					
					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					q = l_inst;

					while(q->suiv != NULL){

						q = q->suiv;

					}

					q->operande = attente_op;			//! revoir vu que le maillon est en queue

					attente_op = NULL;

					A = TEXT;
				}

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

			printf("SEPA_OP\n");
			affiche_lex(p);
			printf("\n");

			if (p->lex == VIRGULE) A = TEST_OP;
			else if (p->lex == NL) A = TEXT;

			else {

				printf("Ligne %d: erreur de separateur dans une instruction de la partie text \n", p->ligne);

				return;

			}

			p = p->suiv;

			break;

			



		case DATA_DIRECTIVE:					//! on est dans .data et on a une directive

		
			recup_mot (p->mot, tab);
			

			if (strcmp(tab, ".byte") == 0) A = DATA_BYTE;

			else if (strcmp(tab, ".word") == 0) A = DATA_WORD;

			else if (strcmp(tab, ".asciiz") == 0) A = DATA_ASCIIZ;

			else if (strcmp(tab, ".space") == 0) A = DATA_SPACE;

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

					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					l_data = ajout_queue_data(l_data, ".byte", DIRECTIVE, 1, p->ligne, dec_data, attente_op); 

					attente_op = NULL;
					dec_data += 1;			//! Decalage de 1 pour le .byte

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

					

					attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

					l_data = ajout_queue_data(l_data,".word", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

					attente_op = NULL;
					dec_data += 4;			//! Decalage de 4 pour le .word

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

				

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_data = ajout_queue_data( l_data, ".asciiz", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

				attente_op = NULL;
				dec_data += nbre_cara(p->mot) - 1; 

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

				

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_data = ajout_queue_data( l_data, ".space", DIRECTIVE, 1, p->ligne, dec_data, attente_op);

				attente_op = NULL;
				dec_data += valeur_deci(p->mot, p->lex);			//! Decalage de c pour le .space

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

		

			

			l_symb = ajout_queue_symb(l_symb, p->mot, p->ligne, 2, dec_data); 

			A = DATA;

			p = p->suiv;

			break;

		

		

		case BSS_SPACE:				//! on est dans .bss et on a un .space

			

			if ( p->lex == (DECIMAL||DECIMAL_ZERO||HEXA)) {

				attente_op = ajout_queue_lex(attente_op, p->lex, p->mot, p->ligne);

				l_bss = ajout_queue_bss( l_bss, ".space", DIRECTIVE, 1, p->ligne, dec_bss, attente_op);

				attente_op = NULL;
				dec_bss += valeur_deci(p->mot, p->lex); 

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
				
				
		case DATA_COLLE_BSS: 

		

			

			l_symb = ajout_queue_symb(l_symb, p->mot, p->ligne, 1, dec_bss); 

			A = BSS;

			p = p->suiv;

			break;

		

		}

	}

	return;



		

			

}

			

			

			

			

			



void main(int nargs, char* argv[]) {

	printf("START \n\n");
	L_LEX lex;
	L_LEX p;
	L_INST l_inst = NULL;
	L_DATA l_data = NULL;
	L_BSS l_bss = NULL;
	L_SYMB l_symb = NULL;
	FILE* fichier;
	//! Ouverture du fichier :
	fichier = fopen(argv[1], "r");
	if (fichier == NULL) perror("Erreur ouverture fichier");	//! Si erreur ouverture

	printf("Debut automate 1  \n");
	lex = lect(fichier);
	printf("Fin automate 1 \n\n");	

	if (lex == NULL) printf("Erreur dans le fichier MIPS \n");
	
	
	printf("Debut automate 2  \n");
	repartition(lex, l_inst, l_data, l_bss, l_symb);
	printf("Fin automate 2 \n\n");
	
	fclose(fichier);

	printf("\nEND \n");
	return;
}		

			

			

			

			

			

			

			

			

			

			
