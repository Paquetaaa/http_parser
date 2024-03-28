#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
//#include 'redgrammm.h"
#include "http_parser.h"


Noeud* creer_noeud(TypeNoeud type, char* etiquette, char* pointeur_debut, int longueur);
Noeud* chercher_noeud(Noeud* racine, char* mot, int longueur);
void ajouter_frere(Noeud** nd, Noeud* frere);
void ajouter_fils(Noeud** nd, Noeud* fils);
void afficher_arbre_generique(Noeud* racine, int niveau);

