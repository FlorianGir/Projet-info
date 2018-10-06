#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fonction_liste.h"

L_LEX lect() {					// Renvoie une liste de lexemes

	FILE* fichier;
	int c;					// c stocke les differents caracteres du programmes MIPS
	int d, b;
	LEXEME A = DEBUT;			// Etat de l'automate
	L_LEX lex = NULL;			// Liste des lexemes( pointeur sur liste de lexeme)
	MOT mot;				// Liste de sauvegarde du mot ( pointeur sur liste de caracteres)

	// Ouverture du fichier :
	fichier = fopen("TEST.txt", "r");
	if (fichier == NULL) perror("Erreur ouverture fichier");	// Si erreur ouverture
	

	while ( (c = fgetc(fichier)) != EOF) {			// Tant que pas a la fin du fichier
						
		switch (A) {			

		
		case DEBUT:
			mot = NULL;
			printf("DEBUT %c \n", c);				//TEST
			mot = ajout_queue_mot(mot, c);			
	
			if ((isdigit(c)) || (c == '-')) {				// Si nombre dans [0;9]
				if (c == '0') A = DECIMAL_ZERO;		// Si c == 0, 0 ou hexadecimal
				else A = DECIMAL;			// Sinon nombre decimal
			}
			else if (isspace(c)) {
				if (c =='\n') {
					lex = ajout_queue_lex(lex, NL, mot);	// Si c'est un retour Ã  la ligne, on ajoute NL Ã  la liste de lexeme
				}
				mot = NULL;				// Reinitialisation du mot
			}
			else if (c == '#') A = COMMENT;			// Si #, c'est un commentaire
			else if (c == '.') A = DIRECTIVE;		// Si ., c'est une directive
			else if (c == ',') {
				lex = ajout_queue_lex(lex, VIRGULE, mot);
				mot = NULL;
			}
			else if (c == '$') A = REGISTRE;		// Si $, c'est un registre
			else if (c == '"') A = GUIL;			// Si c'est un guillemet
			else if (c == '(') Par_o(lex, &mot);		
			else if (c == ')') Par_f(lex, &mot);
			else if (isalpha(c) || (c == '_')) A = SYMBOLE; 		// isalpha() --> Test lettre 
			else {
				return erreur_carac();	// Gestion erreur
			}
			break; 

			
		case DECIMAL_ZERO:
			printf("DECIMAL_ZERO %c \n", c);			// TEST
			if (c == 'x') {
				A = HEXA;				// Si 0x, nombre hexa
				mot = ajout_queue_mot(mot, c);		// On ajoute x au mot qui est un nombre hexa
			}
			else if (isdigit(c)) {
				A = DECIMAL;				// Sinon c'est un 0 decimal
				mot = ajout_queue_mot(mot, c);		// On ajoute le chiffre au mot
			}
			else if (isspace(c)) {				// Sinon, fin du nombre
				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot);	// C'est un nombre decimal qu'on ajoute a la liste lex
				mot = NULL;	
				A = DEBUT;
				if (c =='\n') New_Line(lex, &mot); 
			}
			else if (c == ',') {					// Si virgule
				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot);  // On ajoute le nombre decimal a lex
				Virgule(lex, &mot);
				A = DEBUT;
			}
			else if (c == '(') {
				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot);
				Par_o(lex, &mot);
				A = DEBUT;
			}
			else if (c == ')') {
				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot);
				Par_f(lex, &mot);
				A = DEBUT;
			}
			else return erreur_carac();			// Sinon, c'est une erreur de caractere
			break;
		
		
		case HEXA:						// Si nombre hexadecimal
			printf("HEXA %c \n", c);			// TEST
			if (isxdigit(c)) {
				A = HEXA;
				mot = ajout_queue_mot(mot, c);		// Tant que le nbre n'est pas termine, on l'ajoute dans mot
			}
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, HEXA, mot);
				mot = NULL;
				A = DEBUT;
				if (c =='\n') New_Line(lex, &mot); 
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, HEXA, mot);
				Virgule(lex, &mot);
				A = DEBUT;
			}
			else if (c == '(') {
				lex = ajout_queue_lex(lex, HEXA, mot);
				Par_o(lex, &mot);
				A = DEBUT;
			}
			else if (c == ')') {
				lex = ajout_queue_lex(lex, HEXA, mot);
				Par_f(lex, &mot);
				A = DEBUT;
			}
			else return erreur_carac();	
			break;


		case DECIMAL:						// Si nbre decimal
			printf("DECIMAL %c \n", c);			// TEST
			if (isdigit(c)) mot = ajout_queue_mot(mot, c);
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, DECIMAL, mot);
				mot = NULL;
				A = DEBUT;
				if (c =='\n') New_Line(lex, &mot);
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, DECIMAL, mot);
				Virgule(lex, &mot);
				A = DEBUT;
			}
			else if (c == '(') {
				lex = ajout_queue_lex(lex, DECIMAL, mot);
				Par_o(lex, &mot);
				A = DEBUT;
			}
			else if (c == ')') {
				lex = ajout_queue_lex(lex, DECIMAL, mot);
				Par_f(lex, &mot);
				A = DEBUT;
			}
			else return erreur_carac();	
			break;

			
		case COMMENT:
			printf("COMMENT %c \n", c);			// TEST
			if ( c != '\n' ) mot = ajout_queue_mot(mot, c);
			else {
				lex = ajout_queue_lex(lex, COMMENT, mot);
				mot = NULL;
				New_Line(lex, &mot);
				A = DEBUT;
			}
			break; 

			
		case SYMBOLE:
			printf("SYMBOLE %c \n", c);			// TEST
			if (isalpha(c)||( c == '_')||isdigit(c)) mot = ajout_queue_mot(mot, c);
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, SYMBOLE, mot);
				mot = NULL;
				A = DEBUT;
				if (c =='\n') New_Line(lex, &mot);	
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, SYMBOLE, mot);
				Virgule(lex, &mot);
				A=DEBUT;
			}
			else if (c == ':'){
				printf("DEUX_PTS %c \n", c);			// TEST
				lex = ajout_queue_lex(lex, SYMBOLE, mot);
				Deux_pts(lex, &mot);
				A = DEBUT;
			}
			else return erreur_carac();				
		   	break; 


		case DIRECTIVE:
			printf("DIRECTIVE %c \n", c);			// TEST
			if (isalpha(c)) mot = ajout_queue_mot(mot, c);
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, DIRECTIVE, mot);
				mot = NULL;
				A = DEBUT;
				if (c =='\n') New_Line(lex, &mot);
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, DIRECTIVE, mot);
				Virgule(lex, &mot);
				A=DEBUT;
			}
			else return erreur_carac();		
			break; 

			
		case GUIL:
			printf("GUIL %c \n", c);			// TEST
			if((c != '"') && (c != '\\')){
				mot = ajout_queue_mot(mot, c);
			}
			else if (c == '\\'){
				c = fgetc(fichier);
				mot = ajout_queue_mot(mot, c);
			}
			else if (c == '"'){
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, GUIL, mot);
				mot = NULL;
				A = DEBUT;
			}
			break;	

			
		case REGISTRE:
			printf("REGISTRE %c \n", c);			// TEST
			d = fgetc(fichier);
			b = test_registre( c, d);
			if ( b == 1){
				
				if ((isdigit(c))&&(isdigit(d))){
					mot = ajout_queue_mot(mot, c);
					mot = ajout_queue_mot(mot, d);
					lex = ajout_queue_lex(lex, REGISTRE, mot);
					mot = NULL;
					A = DEBUT;
				}
				else if ((isdigit(c)) && (!isdigit(d))){
					if (d == ',') {
						mot = ajout_queue_mot(mot, c);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						Virgule(lex, &mot);
						A = DEBUT;
					}
					else if (isspace(d)) {
						mot = ajout_queue_mot(mot, c);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						mot = NULL;
						if (d =='\n') New_Line(lex, &mot);
						A = DEBUT;
					}
					else if (d == '(') {
						mot = ajout_queue_mot(mot, c);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						Par_o(lex, &mot);
						A = DEBUT;
					}
					else if (d == ')') {
						mot = ajout_queue_mot(mot, c);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						Par_f(lex, &mot);
						A = DEBUT;
					}			
				}
				else if ((!isdigit(c))&&(isdigit(d))){
					mot = ajout_queue_mot(mot, c);
					mot = ajout_queue_mot(mot, d);
					lex = ajout_queue_lex(lex, REGISTRE, mot);
					mot = NULL;
					A = DEBUT;
				}
				else if ((!isdigit(c))&&(!isdigit(d))){
					if ((c == 'z') && (d == 'e')){
						mot = ajout_queue_mot(mot, c);
						mot = ajout_queue_mot(mot, d);
						if ((c = fgetc(fichier) == 'r') && (d = fgetc(fichier) == 'o')) { 
							mot = ajout_queue_mot(mot, fgetc(fichier));
							mot = ajout_queue_mot(mot, fgetc(fichier));
							lex = ajout_queue_lex(lex, REGISTRE, mot);
							mot = NULL;
						}
						else {
							return erreur_carac();
						}
					}
					else {
						mot = ajout_queue_mot(mot, c);
						mot = ajout_queue_mot(mot, d);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						mot = NULL;
					}
					A = DEBUT;
				}
			}
			else { 
				return erreur_carac();
				A = DEBUT;
				mot == NULL;
			}
			break;
		}
	}
	return lex;	
}	





void main() {

	L_LEX lex;
	L_LEX p;

	printf("Debut automate \n");
	lex = lect();
	printf("Fin automate \n");	
	
	p = lex;
	
	while (p != NULL) {

		affiche_lex(p);
		affiche_mot(p);
		p = p->suiv;
	}
	printf("End \n");
	return;
}

