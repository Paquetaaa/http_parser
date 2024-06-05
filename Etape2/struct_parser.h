#ifndef __STRUCT__
#define __STRUCT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include "http_parser.h"
#include "api.h"

/* Routine permettant de creer un noeud et de le retourner */
Noeud* creer_noeud(TypeNoeud type, char* etiquette, char* pointeur_debut, int longueur);

/* Routine permettant de rechercher un noeud selon son contenu (le retourne si le trouve, NULL sinon) */
Noeud* chercher_noeud(Noeud* racine, char* mot, int longueur);

/* Routine permettant d'ajouter le Noeud frere en tant que frere (dans la liste chainee) du Noeud nd */
void ajouter_frere(Noeud** nd, Noeud* frere);

/* Routine permettant d'ajouter le Noeud fils en tant que fils (possiblement a la fin de la liste chainee) du Noeud nd */
void ajouter_fils(Noeud** nd, Noeud* fils);

/* Routine permettant d'afficher le contenu de la racine et de tous ses descendants */
void afficher_arbre_generique(Noeud* racine, int niveau);

#endif
