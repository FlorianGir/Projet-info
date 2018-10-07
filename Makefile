PROGRAMME = lex
PROGTRACE = lex_trace

all: $(PROGRAMME) $(PROGTRACE)

$(PROGRAMME) :  lecture.c Fonction_liste.c
	gcc lecture.c Fonction_liste.c -o $(PROGRAMME)
	
$(PROGTRACE) : lecture_trace.c Fonction_liste.c
	gcc lecture_trace.c Fonction_liste.c -o $(PROGTRACE)

clean:
	rm -rf *.o

reconstruct: clean
	rm -rf $(PROGRAMME)
	rm -rf $(PROGTRACE)
