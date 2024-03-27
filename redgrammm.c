#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "redgrammm.h"

Noeud *creer_noeud(TypeNoeud type, char *word, char *pointeur_debut, int length)
{
    Noeud *n = (Noeud *)malloc(sizeof(Noeud));
    if (n == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    n->type = type;
    n->word = (char *)malloc((length + 1) * sizeof(char));
    if (n->word == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    n->pointeur_debut = pointeur_debut;
    n->length = length;
    n->fils_aine = NULL;
    n->frere = NULL;
    strncpy(n->word, word, length);
    n->word[length] = '\0';
    return n;
}

void ajouter_noeud(Noeud *racine, Noeud *n)
{
    if (racine->fils_aine == NULL) // Si le noeud n'a pas de fils.
    {
        racine->fils_aine = n;
    }
    else
    {
        Noeud *p = racine->fils_aine;
        while (p->frere != NULL) // Tant qu'on parcourt les freres du noeud,
        {
            p = p->frere; // on avance au frere suivant
        }
        p->frere = n; // Puis on ajoute n en tant que frere du dernier frere.
    }
}

Noeud *chercher_noeud(Noeud *racine, char *word, int length)
{
    Noeud *p = racine->fils_aine;
    while (p != NULL)
    {
        if (p->length == length && strncmp(p->word, word, length) == 0)
        {
            return p;
        }
        p = p->frere;
    }
    return NULL;
}

void ajouter_frere(Noeud *n, Noeud *frere)
{
    Noeud *p = n;
    while (p->frere != NULL)
    {
        p = p->frere;
    }
    p->frere = frere;
}

void ajouter_fils(Noeud *parent, Noeud *fils)
{
    if (parent->fils_aine == NULL)
    {
        parent->fils_aine = fils;
    }
    else
    {
        Noeud *p = parent->fils_aine;
        while (p->frere != NULL)
        {
            p = p->frere;
        }
        p->frere = fils;
    }
}

void afficher_arbre(Noeud *racine, int niveau)
{
    for (int i = 0; i < niveau; i++)
    {
        printf(" ");
    }
    switch (racine->type)
    {
    case DEBUT:
        printf("Debut\n");
        break;
    case FIN:
        printf("Fin\n");
        break;
    case MOT:
        printf("Mot: %s\n", racine->word);
        break;
    case NOMBRE:
        printf("Nombre: %s\n", racine->word);
        break;
    case PONCTUATION:
        printf("Ponctuation: %s\n", racine->word);
        break;
    case SEPARATEUR:
        printf("Separateur: %s\n", racine->word);
        break;
    }
    Noeud *fils = racine->fils_aine;
    while (fils != NULL)
    {
        afficher_arbre(fils, niveau + 1);
        fils = fils->frere;
    }
}

Noeud *construire_arbre(char *requete)
{
    char *start = strstr(requete, "start");                              // strstr va chercher la sous-chaine "start" dans la chaine requete
    Noeud *racine = creer_noeud(DEBUT, "start", start, strlen("start")); // Creation du noeud racine, contient "start"
    Noeud *courant = racine;                                             // Noeud courant, départ de l'arbre

    char *token = strtok(start + strlen("start"), " \t\n-_");

    while (token != NULL)
    {
        Noeud *n;

        if (strcmp(token, "start") == 0)
        {
            token = strtok(NULL, " \t\n-_");
            continue;
        }
        else if (strchr(",.!?:", token[0]) != NULL) // strchr() trouve la première occurrence du caractère dans la chaîne, puis renvoie un pointeur vers le caractère dans la chaîne.
        {
            n = creer_noeud(PONCTUATION, token, token, strlen(token));
        }
        else if (strchr(" \t\n-_", token[0]) != NULL)
        {
            n = creer_noeud(SEPARATEUR, token, token, strlen(token));
        }
        else if (strcmp(token, "fin") == 0) // Ne fonctionne pas si le mot se termine par "[mot]fin"
        {
            n = creer_noeud(FIN, "fin", token, strlen(token));
            ajouter_frere(courant, n);
            break;
        }
       /*
       isnumber existe pas encore
        else if (isnumber(token[0])) // Pas capable de reconnaitre des nombres
        {
            n = creer_noeud(NOMBRE, token, token, strlen(token));
        }
        else
        {
            n = creer_noeud(MOT, token, token, strlen(token));
        }
       */

        ajouter_fils(courant, n); // Creation du fils
        courant = n;              // On descend d'un niveau

        token = strtok(NULL, " \t\n-_"); // On passe au token suivant
    }

    return racine;
}