PROGRAMME = lex
PROGTRACE = lex_trace
PROGERROR = lex_error

all: $(PROGRAMME) $(PROGTRACE) $(PROGERROR)

$(PROGRAMME) :  lecture.c Fonction_liste.c
	gcc lecture.c Fonction_liste.c -o $(PROGRAMME)
	
$(PROGTRACE) : lecture_trace.c Fonction_liste.c
	gcc lecture_trace.c Fonction_liste.c -o $(PROGTRACE)

$(PROGERROR) : lecture_fichier_erreur.c Fonction_liste.c
	gcc lecture_fichier_erreur.c Fonction_liste.c -o $(PROGERROR)

clean:
	rm -rf *.o

reconstruct: clean
	rm -rf $(PROGRAMME)
	rm -rf $(PROGTRACE)
	rm -rf $(PROGERROR)
