##############################################################################################################
#													     #
#					GUYARD Léon 	GIRAUDON Florian				     #
#					      README LIVRABLE 1						     #
#													     #
##############################################################################################################


L'archive est composée de 9 fichiers :
- TEST_OK.s qui est le fichier en langage MIPS a traiter par le programme sans erreur
- TEST_ERROR.s qui est identique à TEST_OK.s mais qui contient quant à lui une erreur (registre trop élevé)
- lecture.c, lecture_trace.c, lecture_fichier_erreur.c, Fonction_liste.c, Fonction_liste.h qui contiennent les programmes développés
	--> lecture.c et lecture_trace.c constituent l'automate créé mais lecture_trace affiche en plus le caractère actuel et le cas dans lequel 		    il se situe pedant son exécution
	--> lecture_fichier_erreur.c est identique à lecture.c mais contient un fichier avec une erreur de registre
	--> Fonction_liste.c contient toutes les fonctions annexes utiles, notamment pour gérer les listes
	--> Fonction_liste.h contient toutes les déclarations de fonctions ainsi que les structures utile pour gérer les listes de lexèmes

- README.txt (ce fichier)
- Makefile


Pour compiler le programme de test, effectuer la commande :
$ make 

Pour ensuite éxécuter le programme, taper :
$ ./lex
Cela permet d'afficher une liste de lexèmes avec les mots associés à chaque lexème depuis le fichier miam_sujet.s.

Pour afficher le parcours de l'algorithme en plus de la liste générée en fin, taper :
$ ./lex_trace

Pour visualiser une erreur syntaxique, taper :
$ ./lex_error

Pour visualiser d'autres erreurs, ouvrir le fichier TEST_ERROR.s et modifier la syntaxe. Refaire les commandes de compilation et d'éxécution des programmes.
