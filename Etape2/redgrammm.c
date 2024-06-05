#include "redgrammm.h"


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


Noeud* construire_arbre(char* requete){

// Construction racine de l'arbre
    char* start = strstr(requete, "start");                              // strstr va chercher la sous-chaine "start" dans la chaine requete et retourner le pointeur d'ou elle commence
    Noeud* racine = creer_noeud(DEBUT, "Start", start, strlen("start")); 
   
    Noeud* courant = racine;
    char* token = strtok(start + strlen("start"), " \t\n-_");
   
    while (token != NULL)
    {
        Noeud *n = NULL;
        //printf("token ce round = %c \n",token[0]);
        if (strcmp(token, "start") == 0){
            token = strtok(NULL, " \t\n-_");
            continue;
        }
        else if (strchr(",.!?:", token[0]) != NULL){ // strchr() trouve la première occurrence du caractère dans la chaîne, puis renvoie un pointeur vers le caractère dans la chaîne.
            n = creer_noeud(PONCTUATION, "Ponctuation", token, 1);
        }
        else if (strchr(" \t\n-_", token[0]) != NULL){
            n = creer_noeud(SEPARATEUR, "Separateur", token, strlen(token));
        }
        else if (strcmp(token, "fin") == 0) // Ne fonctionne pas si le mot se termine par "[mot]fin"
        {
            n = creer_noeud(FIN, "Fin", token, strlen(token));
            ajouter_frere(&courant, n);
            break;
        }

        else if (isnumber(token[0])) // peut etre Pas capable de reconnaitre des nombres
        {
            n = creer_noeud(NOMBRE, "Nombre", token, strlen(token));
        }
        else
        {
            n = creer_noeud(MOT, "Mot", token, strlen(token));
        }
       
        ajouter_fils(&courant, n); // Creation du fils
        courant = n;               // On descend d'un niveau

        //printf("token[0] = %d \n",token[0]);

        token = strtok(NULL, " \t\n-_"); // On passe au token suivant
    }

    return racine;
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

void afficher_arbre(Noeud* racine, int niveau){
    if (racine == NULL){
        return;
    }

    for (int i = 0; i < niveau; i++)
    {
        printf(" ");
    }

// Affichage du noeud courant
    char* contenu;
    strncpy(contenu, racine->pointeur_debut, racine->longueur);
    switch (racine->type){
        case DEBUT:
            printf("Debut\n");
            break;
        case FIN:
            printf("Fin\n");
            break;
        case MOT:
            printf("Mot: %s\n", contenu);
            break;
        case NOMBRE:
            printf("Nombre: %s\n", contenu);
            break;
        case PONCTUATION:
            printf("Ponctuation: %s\n", contenu);
            break;
        case SEPARATEUR:
            printf("Separateur: %s\n", contenu);
            break;
    }

// Affichage des freres du noeud courant
    if (racine->frere != NULL){
        afficher_arbre(racine->frere, niveau);
    }

// Descente recursive : on relance parmi les descendants
    if (racine->fils_aine != NULL){
        afficher_arbre(racine->fils_aine, niveau +1);
    }

    return;
}

bool isnumber(int n)
{
    return (n >= '0' && n <= '9');
    printf("c'est un nombre \n");
}
