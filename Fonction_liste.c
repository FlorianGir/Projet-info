#include <stdio.h>
#include <stdlib.h>
#include "Fonction_liste.h"


L_LEX creer_liste_lex(void) {
	return NULL;
}

MOT creer_liste_mot(void) {
	return NULL;
}

L_LEX ajout_tete_lex(L_LEX l_lex, LEXEME lex, MOT l_mot) {
	L_LEX p = NULL;				//! CrÃ©ation pointeur sur liste lexeme
	p = calloc(1, sizeof(*p));		//! Allocation de mÃ©moire
	p->lex = lex;				//! On complÃ¨te le bloc cree
	p->mot = l_mot;
	p->suiv = l_lex;			//! Liaison du bloc avec les suivants
 	return p;				//! On renvoie p pour ajouter le bloc en tete
}

L_LEX ajout_queue_lex(L_LEX l_lex, LEXEME lex, MOT l_mot) {
	L_LEX p = NULL;
	p = calloc(1, sizeof(*p));
	p->lex = lex;
	p->mot = l_mot;
	if (l_lex == NULL) return p;	//! Si liste NULL, on a que ce bloc en memoire
	else {				//! Sinon, on parcour la liste pour arriver jusqu'au pointeur de fin 
		L_LEX q = l_lex;
		while (q->suiv != NULL) {
			q = q->suiv;
		}
		q->suiv = p;		//! On rattache le bloc cree en fin de liste
		return l_lex;
	}
}



MOT ajout_tete_mot(MOT l_mot, int c) {
	MOT p = NULL;				
	p = calloc(1, sizeof(*p));		
	p->c = c;				
	p->suiv = l_mot;			
 	return p;				
}


MOT ajout_queue_mot(MOT l_mot, int c) {
	MOT p = NULL;				
	p = calloc(1, sizeof(*p));		
	p->c = c;
	if (l_mot == NULL) return p;	
	else {				
		MOT q = l_mot;
		while (q->suiv != NULL) {
			q = q->suiv;
		}
		q->suiv = p;		
		return l_mot;
	}	
}

//! Affichage des lexemes
void affiche_lex(L_LEX p_lex) {

	if (p_lex->lex == DEBUT) printf("DEBUT --> ");
	else if (p_lex->lex == COMMENT) printf("COMMENT --> ");
	else if (p_lex->lex == NL) printf("NL --> ");
	else if (p_lex->lex == SYMBOLE) printf("SYMBOLE --> ");
	else if (p_lex->lex == DEUX_PTS) printf("DEUX_PTS --> ");
	else if (p_lex->lex == DIRECTIVE) printf("DIRECTIVE --> ");
	else if (p_lex->lex == VIRGULE) printf("VIRGULE --> ");
	else if (p_lex->lex == DECIMAL) printf("DECIMAL --> ");
	else if (p_lex->lex == DECIMAL_ZERO) printf("DECIMAL_ZERO --> ");
	else if (p_lex->lex == HEXA) printf("HEXA --> ");
	else if (p_lex->lex == AUTRE) printf("AUTRE --> ");
	else if (p_lex->lex == REGISTRE) printf("REGISTRE --> ");
	else if (p_lex->lex == GUIL) printf("GUIL --> ");
	else if (p_lex->lex == PAR_O) printf("PAR_O --> ");
	else if (p_lex->lex == PAR_F) printf("PAR_F --> ");
	return;
}

//! Affiche le mot associe a un lexeme
void affiche_mot(L_LEX p_lex) {
	MOT q = p_lex->mot;
	while (q != NULL) {
		if (q->c == '\n') printf("\\n");
		else printf("%c", q->c);
		q = q->suiv;
		
	}
	printf("\n");
	return;
}


//! Gestion erreur caractere : impose fin de fonction lect
L_LEX erreur_carac() {
	printf("Erreur de caractere \n");	
	return NULL;
}


//! Gestion virgule 
void Virgule(L_LEX lex, MOT* mot) {		//! Passage de mot par adresse car modifie dans la fonction (pareil pour les 4 fonctions suivantes)
	
	*mot = NULL;
	*mot = ajout_queue_mot(*mot, ',');
	lex = ajout_queue_lex(lex, VIRGULE, *mot);
	*mot = NULL;
}

//! Gestion nouvelle ligne
void New_Line(L_LEX lex, MOT* mot) {
	
	*mot = NULL;
	*mot = ajout_queue_mot(*mot, '\n');
	lex = ajout_queue_lex(lex, NL, *mot);
	*mot = NULL;
}

//!Gestion PAR_O
void Par_o(L_LEX lex, MOT *mot) {
	
	*mot = NULL;
	*mot = ajout_queue_mot(*mot, '(');
	lex = ajout_queue_lex(lex, PAR_O, *mot);
	*mot = NULL;
}

//!Gestion PAR_F
void Par_f(L_LEX lex, MOT* mot) {
	
	*mot = NULL;
	*mot = ajout_queue_mot(*mot, ')');
	lex = ajout_queue_lex(lex, PAR_F, *mot);
	*mot = NULL;
}

//!Gestion deux points
void Deux_pts(L_LEX lex, MOT *mot){
	
	*mot = NULL;
	*mot = ajout_queue_mot(*mot, ':');
	lex = ajout_queue_lex(lex, DEUX_PTS, *mot);
	*mot = NULL;
}

/*!Gestion fin de nombre (virgule, espace, parenthÃ¨se...)
void Fin_nbre(int c, L_LEX* lex, MOT* mot, LEXEME type_lexeme, LEXEME* A) {
	
	if (isspace(c)) {
		*lex = ajout_queue_lex(*lex, type_lexeme, *mot);
		*A = DEBUT;
		if (c =='\n') New_Line(lex, &mot);
	}
	else if (c == ',') {
		*lex = ajout_queue_lex(*lex, type_lexeme, *mot);
		Virgule(*lex, mot);
		*A = DEBUT;
	}
	else if (c == '(') {
		*lex = ajout_queue_lex(*lex, type_lexeme, *mot);
		Par_o(*lex, mot);
		*A = DEBUT;
	}
	else if (c == ')') {
		*lex = ajout_queue_lex(*lex, type_lexeme, *mot);
		Par_f(*lex, mot);
		*A = DEBUT;
	}
	return;
}
*/

//!Test des registres 
int test_registre( int p, int u){
	if ((isdigit(u))&&(isdigit(p))){						//! Si le deuxieme carac est un nombre
		if ((p == '0')||(p == '1')||(p == '2')) return 1;			//! Registre existant entre 00 et 29
		else if (p == '3'){
			if ((u == '0')||(u == '1')) return 1;					//! Registre 30 et 31
			else {
				return 0;
				printf("erreur de caractere pour les registres \n");
			}
		}
		else {
			return 0;
			printf("erreur de caractere pour les registres \n");
		}
	}
	else if (isdigit(p)) return 1;									//! Registre 0 a 9
	else if ((p == 'a')&&(u == 't')) return 1;						//! Mnemonique
	else if ((p == 'v')&&((u == '0')||(u == '1'))) return 1;
	else if ((p == 'a')&&((u == '0')||(u == '1')||(u == '2')||(u == '3'))) return 1;
	else if ((p == 't')&&((u == '0')||(u == '1')||(u == '2')||(u == '3')||(u == '4')||(u == '5')||(u == '6')||(u == '7'))) return 1;
	else if ((p == 's')&&((u == '0')||(u == '1')||(u == '2')||(u == '3')||(u == '4')||(u == '5')||(u == '6')||(u == '7'))) return 1;
	else if ((p == 't')&&((u == '8')||(u == '9'))) return 1;
	else if ((p == 'k')&&((u == '0')||(u == '1'))) return 1;
	else if ((p == 'g')&&(u == 'p')) return 1;
	else if ((p == 's')&&(u == 'p')) return 1;
	else if ((p == 'f')&&(u == 'p')) return 1;
	else if ((p == 'r')&&(u == 'a')) return 1;
	else if ((p == 'z')&&(u == 'e')) return 1;
	else return 0;
}
		


/*! TEST :

void main() {
	
	FILE* fichier;
	MOT mot = NULL, p;
	int c;
	
	fichier = fopen("miam_sujet.s", "r");
	if (fichier == NULL) perror("Erreur ouverture fichier");    
	printf("1 \n");
	c = fgetc(fichier);
	while ( (c = fgetc(fichier)) != EOF) {
		printf("%c \n", c);
		
		mot = ajout_queue_mot(mot, c);
		
	}
	printf("2 \n");
	p = mot;
	while (p != NULL) {
		printf("%c \n", p->c);
		p = p->suiv;
	}
	return;
}
*/
