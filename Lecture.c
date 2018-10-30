#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

#include "Fonction_liste.h"





L_LEX lect(FILE* fichier) {					//! Renvoie une liste de lexemes



	int c;					//! c stocke les differents caracteres du programmes MIPS

	int d, b;

	int* ligne; 

	int a = 1;

	ligne = &a;

	LEXEME A = DEBUT;			//! Etat de l'automate

	L_LEX lex = NULL;			//! Liste des lexemes( pointeur sur liste de lexeme)

	MOT mot;				//! Liste de sauvegarde du mot ( pointeur sur liste de caracteres)
	

	



	

	while ( (c = fgetc(fichier)) != EOF) {			//! Tant que pas a la fin du fichier

						

		switch (A) {			



		

		case DEBUT:

			mot = NULL;					//! Reinitialisation du mot

			//!printf("DEBUT %c \n", c);			//!TEST

			mot = ajout_queue_mot(mot, c);			

	

			if ((isdigit(c)) || (c == '-')) {		//! Si nombre dans [0;9]

				if (c == '0') A = DECIMAL_ZERO;		//! Si c == 0, 0 ou hexadecimal

				else A = DECIMAL;			//! Sinon nombre decimal

			}

			else if (isspace(c)) {

				if (c =='\n') {

					lex = ajout_queue_lex(lex, NL, mot, *ligne);	//! Si retour a la ligne, on ajoute NL a la liste de lexeme

					*ligne = *ligne +1;

				}

			}

			else if (c == '#') A = COMMENT;			//! Si #, c'est un commentaire

			else if (c == '.') A = DIRECTIVE;		//! Si ., c'est une directive

			else if (c == ',') {				//! Si virgule

				lex = ajout_queue_lex(lex, VIRGULE, mot, *ligne);

			}

			else if (c == '$') A = REGISTRE;		//! Si $, c'est un registre

			else if (c == '"') A = GUIL;			//! Si c'est un guillemet

			else if (c == '(') Par_o(lex, &mot, *ligne);		//! Si parenthese ouvrante

			else if (c == ')') Par_f(lex, &mot, *ligne);		//! Si parenthese fermante

			else if (isalpha(c) || (c == '_')) A = SYMBOLE; 		//! isalpha() --> Test lettre 

			else {

				return erreur_carac();		//! Gestion erreur (sort du programme)

			}

			break; 



			

		case DECIMAL_ZERO:

			if (c == 'x') {

				A = HEXA;				//! Si 0x, nombre hexa

				mot = ajout_queue_mot(mot, c);		//! On ajoute x au mot qui est un nombre hexa

			}

			else if (isdigit(c)) {

				A = DECIMAL;				//! Si chiffre, c'est un 0 decimal

				mot = ajout_queue_mot(mot, c);		//! On ajoute le chiffre au mot

			}

			else if (isspace(c)) {				//! Si fin du nombre

				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot, *ligne);	//! C'est un nombre decimal qu'on ajoute a la liste lex	

				A = DEBUT;					//! Retour au cas DEBUT, le nbre est termine

				if (c =='\n') New_Line(lex, &mot, ligne); 		//! Si retour Ã  la ligne

			}

			else if (c == ',') {					//! Si virgule

				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot, *ligne);  //! On ajoute le nombre decimal a lex

				Virgule(lex, &mot, *ligne);				//! On ajoute la virgule a lex

				A = DEBUT;					//! REtour cas DEBUT

			}

			else if (c == '(') {

				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot, *ligne);

				Par_o(lex, &mot, *ligne);

				A = DEBUT;

			}

			else if (c == ')') {

				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot, *ligne);

				Par_f(lex, &mot, *ligne);

				A = DEBUT;

			}

			else return erreur_carac();			//! Sinon, c'est une erreur de caractere

			break;

		

		

		case HEXA:						//! Si nombre hexadecimal

			if (isxdigit(c)) {

				A = HEXA;

				mot = ajout_queue_mot(mot, c);		//! Tant que le nbre n'est pas termine, on l'ajoute dans mot

			}

			else if (isspace(c)) {				//! Si espace ou retour ligne

				lex = ajout_queue_lex(lex, HEXA, mot, *ligne);	//! On ajoute le mot actuel a la liste lex

				A = DEBUT;				//! On retourne au cas DEBUT

				if (c =='\n') New_Line(lex, &mot, ligne); 	//! Si retour ligne, on l'ajoute a la liste lex

			}

			else if (c == ',') {

				lex = ajout_queue_lex(lex, HEXA, mot, *ligne);

				Virgule(lex, &mot, *ligne);

				A = DEBUT;

			}

			else if (c == '(') {

				lex = ajout_queue_lex(lex, HEXA, mot, *ligne);

				Par_o(lex, &mot, *ligne);

				A = DEBUT;

			}

			else if (c == ')') {

				lex = ajout_queue_lex(lex, HEXA, mot, *ligne);

				Par_f(lex, &mot, *ligne);

				A = DEBUT;

			}

			else return erreur_carac();	

			break;





		case DECIMAL:						//! Si nbre decimal

			if (isdigit(c)) mot = ajout_queue_mot(mot, c);	//! Tant que c est un nombre, on ajoute c a mot

			else if (isspace(c)) {

				lex = ajout_queue_lex(lex, DECIMAL, mot, *ligne);

				A = DEBUT;

				if (c =='\n') New_Line(lex, &mot, ligne);

			}

			else if (c == ',') {

				lex = ajout_queue_lex(lex, DECIMAL, mot, *ligne);

				Virgule(lex, &mot, *ligne);

				A = DEBUT;

			}

			else if (c == '(') {

				lex = ajout_queue_lex(lex, DECIMAL, mot, *ligne);

				Par_o(lex, &mot, *ligne);

				A = DEBUT;

			}

			else if (c == ')') {

				lex = ajout_queue_lex(lex, DECIMAL, mot, *ligne);

				Par_f(lex, &mot, *ligne);

				A = DEBUT;

			}

			else return erreur_carac();	

			break;



					

		case COMMENT:						//! Si commentaire			

			if ( c != '\n' ) mot = ajout_queue_mot(mot, c);	//! ConcatÃ©nation

			else {						//! Sinon fin COMMENT

				lex = ajout_queue_lex(lex, COMMENT, mot, *ligne);

				mot = NULL;

				New_Line(lex, &mot, ligne);

				A = DEBUT;

			}

			break; 



			

		case SYMBOLE:						//! Si symbole

			if (isalpha(c)||( c == '_')||isdigit(c)) mot = ajout_queue_mot(mot, c); //! Si le caractere est toujours un symbole

			else if (isspace(c)) {							//! Sinon fin symbole

				lex = ajout_queue_lex(lex, SYMBOLE, mot, *ligne);

				mot = NULL;

				A = DEBUT;

				if (c =='\n') New_Line(lex, &mot, ligne);	

			}

			else if (c == ',') {

				lex = ajout_queue_lex(lex, SYMBOLE, mot, *ligne);

				Virgule(lex, &mot, *ligne);

				A=DEBUT;

			}

			else if (c == ':'){

				lex = ajout_queue_lex(lex, ETIQUETTE, mot, *ligne);

				Deux_pts(lex, &mot, *ligne);

				A = DEBUT;

			}

			else return erreur_carac();				

		   	break; 





		case DIRECTIVE:						//! Si directive

			if (isalpha(c)) mot = ajout_queue_mot(mot, c);

			else if (isspace(c)) {

				lex = ajout_queue_lex(lex, DIRECTIVE, mot, *ligne);

				mot = NULL;

				A = DEBUT;

				if (c =='\n') New_Line(lex, &mot, ligne);

			}

			else if (c == ',') {

				lex = ajout_queue_lex(lex, DIRECTIVE, mot, *ligne);

				Virgule(lex, &mot, *ligne);

				A = DEBUT;

			}

			else return erreur_carac();		

			break; 



				

		case GUIL:					//! Si guillemet

			if((c != '"') && (c != '\\')){		//! Si le caractere n'est pas un guillemet ou caractere d'echappement

				mot = ajout_queue_mot(mot, c);	//! On concatene

			}

			else if (c == '\\'){			//! Si caractere echappement

				c = fgetc(fichier);		//! On prend le caractere suivant

				mot = ajout_queue_mot(mot, c);	//! Et on le concatene

			}

			else if (c == '"'){			//! Fin GUIL

				mot = ajout_queue_mot(mot, c);

				lex = ajout_queue_lex(lex, GUIL, mot, *ligne);

				mot = NULL;

				A = DEBUT;

			}

			break;	



			

		case REGISTRE:					//! Si registre

			d = fgetc(fichier);			//! On prend le caactere suivant

			b = test_registre( c, d);

			if ( b == 1){				//! Si c'est un registre

				

				if ((isdigit(c))&&(isdigit(d))){

					mot = ajout_queue_mot(mot, c);

					mot = ajout_queue_mot(mot, d);

					lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

					mot = NULL;

					A = DEBUT;

				}

				else if ((isdigit(c)) && (!isdigit(d))){

					if (d == ',') {

						mot = ajout_queue_mot(mot, c);

						lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

						Virgule(lex, &mot, *ligne);

						A = DEBUT;

					}

					else if (isspace(d)) {

						mot = ajout_queue_mot(mot, c);

						lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

						mot = NULL;

						if (d =='\n') New_Line(lex, &mot, ligne);

						A = DEBUT;

					}

					else if (d == '(') {

						mot = ajout_queue_mot(mot, c);

						lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

						Par_o(lex, &mot, *ligne);

						A = DEBUT;

					}

					else if (d == ')') {

						mot = ajout_queue_mot(mot, c);

						lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

						Par_f(lex, &mot, *ligne);

						A = DEBUT;

					}			

				}

				else if ((!isdigit(c))&&(isdigit(d))){

					mot = ajout_queue_mot(mot, c);

					mot = ajout_queue_mot(mot, d);

					lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

					mot = NULL;

					A = DEBUT;

				}

				else if ((!isdigit(c))&&(!isdigit(d))){

					if ((c == 'z') && (d == 'e')){

						mot = ajout_queue_mot(mot, c);

						mot = ajout_queue_mot(mot, d);

						if ((c = fgetc(fichier) == 'r') && (d = fgetc(fichier) == 'o')) { 

							mot = ajout_queue_mot(mot, 'r');

							mot = ajout_queue_mot(mot, 'o');

							lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

							mot = NULL;

						}

						else {

							return erreur_carac();

						}

					}

					else {

						mot = ajout_queue_mot(mot, c);

						mot = ajout_queue_mot(mot, d);

						lex = ajout_queue_lex(lex, REGISTRE, mot, *ligne);

						mot = NULL;

					}

					A = DEBUT;

				}

			}

			else { 				//! Sinon ce n'est pas un registre valable

				return erreur_carac();

				

			}

			break;

		}

	}

	return lex;	

}	











/*!void main(int nargs, char* argv[]) {



	printf("START \n\n");

	L_LEX lex;

	L_LEX p;

	

	FILE* fichier;

	//! Ouverture du fichier :

	fichier = fopen(argv[1], "r");

	if (fichier == NULL) perror("Erreur ouverture fichier");	//! Si erreur ouverture



	printf("Debut automate \n");

	lex = lect(fichier);

	printf("Fin automate \n\n");	



	if (lex == NULL) printf("Erreur dans le fichier MIPS \n");

	else {

		printf("Affichage des lexemes et des mots correspondant : \n\n");

		p = lex;

	

		while (p != NULL) {



			affiche_lex(p);

			affiche_mot(p);

			printf( " ---> ligne %d \n", p->ligne);

			p = p->suiv;

		}

		

	}

	fclose(fichier);



	printf("\nEND \n");

	return;

}*/
