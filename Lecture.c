#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lecture.h"


LEXEME* lect() {

	FILE* fichier;
	int c = fgetc(fichier);			// Stocke les differents caractÃ¨res
	int d, b;
	LEXEME A = DEBUT;			// Etat de l'automate
	LISTE LEX;				//Liste des lexemes

	fichier = fopen("nom_fichier.txt", "r");
	if (fichier == NULL) perror("Erreur ouverture fichier"; return 1;);	// Si erreur ouverture
	
	while ( c != EOF) {			// Tant que pas a la fin du fichier
		c = fgetc(fichier);		// Lecture du caractÃ¨re
		
		switch (A) {			
		
		case DEBUT:
			if (isdigit(c)) {				// Si nombre dans [0;9]
				if (c == '0') A = DECIMAL_ZERO;		// Si c == 0, 0 ou hexadecimal
				else A = DECIMAL;			// Sinon nombre decimal
			}

			else if (isspace(c)) {
				if (c =='\n') ajout_queue( LEX, NL, chaine);	//Faire les fonctions !!!!!!!!!
				else A = DEBUT;
			}
			else if (c == ':') A = DEUX_POINTS;
			else if (c == '#') A = COMMENT;
			else if (c == '.') A = DIRECTIVE;
			else if (c == ',') {
				ajout_queue(LEX, VIRGULE, c);
				A=DEBUT;
			}
			else if (c == '$') A = REGISTRE;
			else if (isalpha(c)) A = SYMBOLE; 		// Test lettre en code ASCII
			else {
				printf("erreur de caractere" \n);	// Gestion erreur
				return 0; 
			}
			break; 

			
		case DECIMAL_ZERO:
			if (c == 'x') A = HEXA;				// Si 0x, nbre hexa, sinon 0 decimal
			else if (isdigit(c)) A = DECIMAL;
			else if (isspace(c)) {
				ajout_queue(LEX, DECIMAL_ZERO, c);
				if (c =='\n') {
					ajout_queue( LEX, NL, c);	//Faire les fonctions !!!!!!!!!
					A = DEBUT;
				}
				else A = DEBUT;
			else if (c == ',') {
				ajout_queue(LEX, DECIMAL_ZERO, c);
				ajout_queue(LEX, VIRGULE, c);
				A=DEBUT;
			}
			else erreur_carac();		
			break;
		
		
		case HEXA:						// Si nbre hexadecimal
			if (isxdigit(c)) A = HEXA;
			else if (isspace(c)) {
				ajout_queue(LEX, HEXA, chaine);
				if (c =='\n') {
					ajout_queue( LEX, NL, c);	//Faire les fonctions !!!!!!!!!
					A = DEBUT;
				}
				else A = DEBUT;
			else if (c == ',') {
				ajout_queue(LEX, HEXA, chaine);
				ajout_queue(LEX, VIRGULE, c);
				A=DEBUT;
			}
			else erreur_carac();	
			break;


		case DECIMAL:						// Si nbre decimal
			if (isdigit(c)) A = DECIMAL;
			else if (isspace(c)) {
				ajout_queue(LEX, DECIMAL, chaine);
				if (c =='\n') {
					ajout_queue( LEX, NL, c);	//Faire les fonctions !!!!!!!!!
					A = DEBUT;
				}
				else A = DEBUT;
			else if (c == ',') {
				ajout_queue(LEX, DECIMAL, chaine);
				ajout_queue(LEX, VIRGULE, c);
				A=DEBUT;
			}
			else erreur_carac();	
			break;
			
		case COMMENT:
			if ( c != '\n' ) A = COMMENT;
			else if {
				ajout_queue(LEX, COMMENT, chaine);
				ajout_queue(LEX, NL, c);			//Faire les fonctions !!!!!!!!!
				A = DEBUT;
			break; 

		case SYMBOLE:
			if (isalpha(c)) A = SYMBOLE;
			else if (isspace(c)) {
				ajout_queue(LEX, SYMBOLE, chaine);
				if (c =='\n') {
					ajout_queue( LEX, NL, c);	//Faire les fonctions !!!!!!!!!
					A = DEBUT;
				}
				else A = DEBUT;
			else if (c == ',') {
				ajout_queue(LEX, SYMBOLE, chaine);
				ajout_queue(LEX, VIRGULE, c);
				A=DEBUT;
			}
			else if (c == ':'){
				ajout_queue(LEX, SYMBOLE, chaine);
				ajout_queue(LEX, DEUX_PTS, c);
				A = DEBUT;
			}
			else erreur_carac();				
		   	break; 


		case DIRECTIVE:
			if (isalpha(c));
			else if (isspace(c)) {
				ajout_queue(LEX, DIRECTIVE, chaine);
				if (c =='\n') {
					ajout_queue( LEX, NL, c);	//Faire les fonctions !!!!!!!!!
					A = DEBUT; 
				}
				else A = DEBUT;
			else if (c == ',') {
				ajout_queue(LEX, DIRECTIVE, chaine);
				ajout_queue(LEX, VIRGULE, c);
				A=DEBUT;
			}
			else erreur_carac();		
			break; 
			

		case REGISTRE:
			d = fgetc(fichier);
			b = test_registre( c, d);
			if ( b == 1){
				if (!isdigit(d)){
					else if (d == ',') {
						ajout_queue(LEX, REGISTRE, c);
						ajout_queue(LEX, VIRGULE, d);
						A=DEBUT;
					}
					else if (isspace(d)) {
						ajout_queue(LEX, REGISTRE, c);
						if (d =='\n') {
							ajout_queue( LEX, NL, d);	//Faire les fonctions !!!!!!!!!
							A = DEBUT;
						}
					}
						else A = DEBUT;
					else erreur_carac();
					}
				else A = DEBUT;
			break;

	} 			
	
	

int test_registre( int p, int u){
	if ((isdigit(u))&&(isdigit(p))){						// si le deuxieme carac est un nombre
		if ((p == '0')||(p == '1')||(p == '2')) return 1;			//registre existant entre 00 et 29
		if else (p == '3'){
			if ((u == '0')||(u == '1')) return 1;					//registre 30 et 31
			else {
				return 0;
				printf("erreur de caractere pour les registre \n");
			}
		}
		else {
			return 0;
			printf("erreur de caractere pour les registre \n");
		}
	}
	else if (isdigit(p)) return 1;									//registre 0 a 9
	else {
		return 0;
		printf("erreur de caractere pour les registre \n");
	}
}
		



// Gestion erreur caractÃ¨re
int erreur_carac() {
	printf("erreur de caractere \n");	// ameliorer avec ligne erreur
	return 0;
}


