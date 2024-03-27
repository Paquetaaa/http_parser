#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ABNF TEST
/*SEPARATEUR*/
/*" \t\n-_"*/

/*PONCTUATION*/
/*"," / "." / "!" / "?" / ":"*/

typedef enum
{
    DEBUT,
    FIN,
    MOT,
    NOMBRE,
    PONCTUATION,
    SEPARATEUR
} TypeNoeud;

typedef struct node
{
    TypeNoeud type; // Type du noeud
    char *word;     // Permet de stocker la valeur du noeud
    char *pointeur_debut;
    int length;
    struct node *fils_aine;
    struct node *frere;

} Noeud;

Noeud *creer_noeud(TypeNoeud type, char *word, char *pointeur_debut, int length);
void ajouter_noeud(Noeud *racine, Noeud *n);
Noeud *chercher_noeud(Noeud *racine, char *word, int length);
void ajouter_frere(Noeud *n, Noeud *frere);
void ajouter_fils(Noeud *parent, Noeud *fils);
void afficher_arbre(Noeud *racine, int niveau);
Noeud *construire_arbre(char *requete);
