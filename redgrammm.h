#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

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
    TypeNoeud type;         // Type du noeud
    char* etiquette;        // Etiquette du noeud
    char* pointeur_debut;   // Debut de la string que represente le noeud 
    int longueur;            // Taille de la string representee par le noeud
    struct node* pere;      // 
    struct node* fils_aine; // 
    struct node* frere;     // 

} Noeud;

Noeud* creer_noeud(TypeNoeud type, char* etiquette, char* pointeur_debut, int longueur);
Noeud* chercher_noeud(Noeud* racine, char* mot, int longueur);
Noeud* construire_arbre(char* requete);
void ajouter_frere(Noeud** nd, Noeud* frere);
void ajouter_fils(Noeud** nd, Noeud* fils);
void afficher_arbre(Noeud* racine, int niveau);
