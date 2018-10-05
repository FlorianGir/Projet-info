#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fonction_liste.h"




// Gestion erreur caractere
void erreur_carac() {
	printf("erreur de caractere \n");	
	return;
}


L_LEX lect() {					// Renvoie une liste de lexemes

	FILE* fichier;
	int c;					// c stocke les differents caracteres du programmes MIPS
	int d, b;
	LEXEME A = DEBUT;			// Etat de l'automate
	L_LEX lex = NULL;			// Liste des lexemes( pointeur sur liste de lexeme)
	MOT mot = NULL;				// Liste de sauvegarde du mot ( pointeur sur liste de caracteres)
	
	printf("2 \n");		//TEST

	// Ouverture du fichier :
	fichier = fopen("TEST.txt", "r");
	if (fichier == NULL) perror("Erreur ouverture fichier");	// Si erreur ouverture
	


	while ( (c = fgetc(fichier)) != EOF) {			// Tant que pas a la fin du fichier
				
		
		switch (A) {			
		
		case DEBUT:
			
			printf("DEBUT %c \n", c);				//TEST
			mot = ajout_queue_mot(mot, c);			
	
			if (isdigit(c)) {				// Si nombre dans [0;9]
				if (c == '0') A = DECIMAL_ZERO;		// Si c == 0, 0 ou hexadecimal
				else A = DECIMAL;			// Sinon nombre decimal
			}
			else if (isspace(c)) {
				if (c =='\n') {
					lex = ajout_queue_lex(lex, NL, mot);	// Si c'est un retour à la ligne, on ajoute NL à la liste de lexeme
				
				}
				mot = NULL;				// Reinitialisation du mot
			}
			else if (c == '#') A = COMMENT;			// Si #, c'est un commentaire
			else if (c == '.') A = DIRECTIVE;		// Si ., c'est une directive
			else if (c == ',') {
				lex = ajout_queue_lex(lex, VIRGULE, mot);
				mot = NULL;
				A = DEBUT;
			}
			else if (c == '$') A = REGISTRE;		// Si $, c'est un registre
			else if (isalpha(c)) A = SYMBOLE; 		// isalpha() --> Test lettre 
			else {
				printf("erreur de caractere \n");	// Gestion erreur
				return ; 
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
				mot = NULL;					// Reinitialisation mot
				if (c =='\n') {					// Si fin de ligne
					mot = ajout_queue_mot(mot, c);
					lex = ajout_queue_lex(lex , NL, mot);	// On rentre la NL dans la liste lex
					mot = NULL; 
					A = DEBUT;				// On retourne au cas d'initialisation
				}
				else A = DEBUT;
			}
			else if (c == ',') {					// Si virgule
				lex = ajout_queue_lex(lex, DECIMAL_ZERO, mot);  // On ajoute le nombre decimal a lex
				mot = NULL;
				mot = ajout_queue_mot(mot, c);			// Puis on ajoute la virgule dans lex
				lex = ajout_queue_lex(lex, VIRGULE, mot);	
				mot = NULL;
				A = DEBUT;
			}
			else erreur_carac();			// Sinon, c'est une erreur de caractere
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
				if (c =='\n') {
					mot = ajout_queue_mot(mot, c);
					lex = ajout_queue_lex(lex , NL, mot);
					mot = NULL; 
					A = DEBUT;
				}
				else A = DEBUT;
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, HEXA, mot);
				mot = NULL;
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, VIRGULE, mot);
				mot = NULL;
				A=DEBUT;
			}
			else erreur_carac();	
			break;


		case DECIMAL:						// Si nbre decimal
			if (isdigit(c)) {
				A = DECIMAL;
				mot = ajout_queue_mot(mot, c);
			}
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, DECIMAL, mot);
				mot = NULL;
				if (c =='\n') {
					mot = ajout_queue_mot(mot, c);
					lex = ajout_queue_lex(lex, NL, mot);
					mot = NULL;
					A = DEBUT;
				}
				else A = DEBUT;
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, DECIMAL, mot);
				mot = NULL;
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, VIRGULE, mot);
				mot = NULL;
				A=DEBUT;
			}
			else erreur_carac();	
			break;
			
		case COMMENT:
			printf("COMMENT %c \n", c);			// TEST
			if ( c != '\n' ) {
				A = COMMENT;		
				mot = ajout_queue_mot(mot, c);
			}
				
			else {
				lex = ajout_queue_lex(lex, COMMENT, mot);
				mot = NULL;
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, NL, mot);
				mot = NULL;
				A = DEBUT;
			}
			break; 

		case SYMBOLE:
			printf("SYMBOLE %c \n", c);			// TEST
			if (isalpha(c)) {
				A = SYMBOLE; 	
				mot = ajout_queue_mot(mot, c);
			}
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, SYMBOLE, mot);
				mot = NULL;
				if (c =='\n') {
					mot = ajout_queue_mot(mot, c);
					lex = ajout_queue_lex(lex, NL, mot);
					mot = NULL;
					A = DEBUT;
				}
				else A = DEBUT;
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, SYMBOLE, mot);
				mot = NULL;
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, VIRGULE, mot);
				mot = NULL;
				A=DEBUT;
			}
			else if (c == ':'){
				lex = ajout_queue_lex(lex, SYMBOLE, mot);
				mot = NULL;
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, DEUX_PTS, mot);
				mot = NULL;
				A = DEBUT;
				printf("DEUX_PTS %c \n", c);			// TEST
			}
			else erreur_carac();				
		   	break; 


		case DIRECTIVE:
			printf("DIRECTIVE %c \n", c);			// TEST
			if (isalpha(c)){
				mot = ajout_queue_mot(mot, c);
			}
			else if (isspace(c)) {
				lex = ajout_queue_lex(lex, DIRECTIVE, mot);
				mot = NULL;
				if (c =='\n') {
					mot = ajout_queue_mot(mot, c);
					lex = ajout_queue_lex(lex, NL, mot);
					mot = NULL;
					A = DEBUT;
				}
				else A = DEBUT;
			}
			else if (c == ',') {
				lex = ajout_queue_lex(lex, DIRECTIVE, mot);
				mot = NULL;
				mot = ajout_queue_mot(mot, c);
				lex = ajout_queue_lex(lex, VIRGULE, mot);
				mot = NULL;
				A=DEBUT;
			}
			else erreur_carac();		
			break; 
			

		case REGISTRE:
			printf("REGISTRE %c \n", c);			// TEST
			d = fgetc(fichier);
			b = test_registre( c, d);
			if ( b == 1){
				if (!isdigit(d)){
					if (d == ',') {
						mot = ajout_queue_mot(mot, c);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						mot = NULL;
						mot = ajout_queue_mot(mot, d);
						lex = ajout_queue_lex(lex, VIRGULE, mot);
						mot = NULL;
						A = DEBUT;
					}
					else if (isspace(d)) {
						mot = ajout_queue_mot(mot, c);
						lex = ajout_queue_lex(lex, REGISTRE, mot);
						mot = NULL;
						if (d =='\n') {
							mot = ajout_queue_mot(mot, d);
							lex = ajout_queue_lex(lex, NL, mot);
							mot = NULL;
							A = DEBUT;
						}
						else A = DEBUT;
					}
					else erreur_carac();
				}
				else {
					mot = ajout_queue_mot(mot, c);
					mot = ajout_queue_mot(mot, d);
					lex = ajout_queue_lex(lex, REGISTRE, mot);
					mot = NULL;
					A = DEBUT;
				}
			}
			else {
				erreur_carac();
			}
			break;
		}
	}

	return lex;
		
}	



int test_registre( int p, int u){
	if ((isdigit(u))&&(isdigit(p))){						// Si le deuxieme carac est un nombre
		if ((p == '0')||(p == '1')||(p == '2')) return 1;			// Registre existant entre 00 et 29
		else if (p == '3'){
			if ((u == '0')||(u == '1')) return 1;					// Registre 30 et 31
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
	else if (isdigit(p)) return 1;									// Registre 0 a 9
	else {
		return 0;
		printf("erreur de caractere pour les registre \n");
	}
}
		







// Gestion virgule 
void Virgule(L_LEX lex, MOT mot) {		
	
	mot = NULL;
	mot = ajout_queue_mot(mot, ',');
	lex = ajout_queue_lex(lex, VIRGULE, mot);
	mot = NULL;

}


// Gestion nouvelle ligne
void New_Line(L_LEX lex, MOT mot) {
	
	mot = NULL;
	mot = ajout_queue_mot(mot, '\n');
	lex = ajout_queue_lex(lex, NL, mot);
	mot = NULL;

}


void main() {

	L_LEX lex;
	L_LEX p;
	printf("1 \n");		//TEST
	
	lex = lect();

	
	printf("fin lect() \n");	//TEST
	p = lex;
	printf("3 \n");
	while (p != NULL) {

		affiche_lex(p);
		affiche_mot(p);
		p = p->suiv;
	}
	printf("end \n");
	return;
}






