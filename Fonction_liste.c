#include <stdio.h>
#include "Fonction_liste.h"


LEX creer_liste_lex(void) {
	return NULL;
}

MOT creer_liste_lex(void) {
	return NULL;
}

LEX ajout_tete_lex(L_LEX l_lex, LEXEME lex, MOT l_mot) {
	L_LEX p = NULL;				// Création pointeur sur liste lexeme
	p = calloc(1, sizeof(*p));		// Allocation de mémoire
	p->lex = lex;				// On complète le bloc cree
	p->mot = l_mot;
	p->suiv = l_lex				// Liaison du bloc avec les suivants
 	return p;				// On renvoie p pour ajouter le bloc en tete
}


LEX ajout_queue_lex(L_LEX l_lex, LEXEME lex, MOT l_mot) {
	L_LEX p = NULL;
	p = calloc(1, sizeof(*p));
	p->lex = lex;
	p->mot = l_mot;
	if (l == NULL) return p;	// Si liste NULL, on a que ce bloc en memoire
	else {				// Sinon, on parcour la liste pour arriver jusqu'au pointeur de fin 
		L_LEX q = l_lex
		while (q->suiv != NULL) {
			q = q->suiv;
		}
		q->suiv = p;		// On rattache le bloc cree en fin de liste
		return l_lex;
	}
}



MOT ajout_tete_mot(MOT l_mot, int c) {
	MOT p = NULL;				
	p = calloc(1, sizeof(*p));		
	p->c = c;				
	p->suiv = l				
 	return p;				
}


MOT ajout_queue_mot(MOT l_mot, int c);
	MOT p = NULL;				
	p = calloc(1, sizeof(*p));		
	p->c = c;
	if (l_mot == NULL) return p;	
	else {				
		MOT q = l_mot
		while (q->suiv != NULL) {
			q = q->suiv;
		}
		q->suiv = p;		
		return l_mot;
	}
}
