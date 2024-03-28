#include "struct_parser.h"

/* Note :
 * Le fichier struct_parser.c contient toutes les fonctions relative a la structure de l'arbre, utiles a la fois pour la grammaire simple et la grammaire complete.
 * Le fichier struct_parser.h est le header concernant uniquement les fonctions utiles a la fois a la grammaire simple et a la grammaire complete.
 * Le fichier http_parser.h est le header concernant uniquement les fontions utiles a la grammaire complete.
*/

Noeud* creer_noeud(TypeNoeud type, char* etiquette, char* pointeur_debut, int longueur){
    Noeud* nd = (Noeud*) malloc(sizeof(Noeud));
    if (nd == NULL){
        perror("Erreur malloc <creer_noeud>\n");
        return EXIT_FAILURE;
    }

    int taille_etiquette = strlen(etiquette);
    nd->etiquette = (char*) malloc(taille_etiquette * sizeof(char));
    if (nd->etiquette == NULL){
        perror("Erreur malloc <creer_noeud>\n");
        return EXIT_FAILURE;
    }

    nd->type = type;
    nd->pointeur_debut = pointeur_debut;
    nd->longueur = longueur;
    nd->pere = NULL;
    nd->fils_aine = NULL;
    nd->frere = NULL;
    strncpy(nd->etiquette, etiquette, taille_etiquette);
    return nd;
}


Noeud* chercher_noeud(Noeud* racine, char* mot, int longueur){
    if (racine == NULL){
        return NULL;
    }

// Recherche parmi la racine et ses freres
    Noeud* courant = racine;
    while (courant != NULL){
        if (courant->longueur == longueur && strncmp(courant->pointeur_debut, mot, longueur) == 0){
            return courant;
        }

        courant = courant->frere;
    }

// Descente recursive : on relance la recherche parmi les descendants
    return chercher_noeud(racine->fils_aine, mot, longueur);
}


void ajouter_fils(Noeud** nd, Noeud* fils){
    if (nd == NULL){
        perror("Segfault <ajouter_fils>\n");
        return;
    }

    if (*nd == NULL){                       // Cas ou l'arbre est vide
        *nd = fils;
    }
    else if ((*nd)->fils_aine == NULL){     // Cas ou le noeud n'a pas de fils
        (*nd)->fils_aine = fils;
        fils->pere = *nd;
    }
    else{                                   // Sinon on parcout la liste chainee des fils pour ajouter le nouveau a la fin
        Noeud* courant = (*nd)->fils_aine;
        while (courant->frere != NULL){
            courant = courant->frere;                       
        }
        courant->frere = fils;
        fils->pere = nd;
    }
}

void ajouter_frere(Noeud** nd, Noeud* frere){
    if (nd == NULL){
        perror("Segfault <ajouter_frere>\n");
        return;
    }

    if (*nd == NULL){           // Das ou le noeud est vide
        *nd = frere;
    } 
    else{                       // Sinon parcout la liste chainee des freres pour inserer le noueau a la fin
        Noeud* courant = *nd;
        while (courant->frere != NULL)
        {
            courant = courant->frere;
        }
        courant->frere = frere;
        frere->pere = courant->pere;
    }
}


void afficher_arbre_generique(Noeud* racine, int niveau){
    if (racine == NULL){
        return;
    }

// Affichage du noeud courant
    Noeud* courant = racine;
    char* contenu;
    strncpy(contenu, courant->pointeur_debut, courant->longueur);
    printf("[%d : %s] = %s", niveau, courant->etiquette, contenu);

// Affichage des freres du noeud courant
    if (racine->frere != NULL){
        printf("\t");
        afficher_arbre(racine->frere, niveau);
    }

// Descente recursive : on relance parmi les descendants
    if (racine->fils_aine != NULL){
        printf("\n");
        afficher_arbre(racine->fils_aine, niveau +1);
    }

    return;
}

