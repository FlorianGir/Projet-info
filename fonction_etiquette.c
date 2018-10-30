int check_etiquette(L_LEX l_etiquette, MOT mot){
	L_LEX q = l_etiquette;
	
	char tab_etiquette[100];
	char tab_mot[100];
	
	recup_mot(mot, tab_mot);
	while( q != NULL ){
		recup_mot(q->mot, tab_etiquette);
		if (strcmp(tab_mot, tab_etiquette == 0){
			return 1;
		}
		q = q->suiv;
	}
	return 0;
}
