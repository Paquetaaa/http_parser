#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <_ctype.h>

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
    if (racine->fils_aine == NULL)
    {
        racine->fils_aine = n;
    }
    else
    {
        Noeud *p = racine->fils_aine;
        while (p->frere != NULL)
        {
            p = p->frere;
        }
        p->frere = n;
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
    char *token = strtok(requete, " \t\n");                            // Sépare la requête en tokens en utilisant les espaces, les tabulations et les sauts de ligne comme délimiteurs
    Noeud *racine = creer_noeud(DEBUT, "start", token, strlen(token)); // Crée un nœud pour le début de la requête
    Noeud *courant = racine;                                           // Initialise le nœud courant à la racine

    while (token != NULL)
    {
        Noeud *n;
        if (strcmp(token, "start") == 0)
        {
            // Ignore le token "start" car nous l'avons déjà ajouté à la racine
        }
        else if (strcmp(token, "fin") == 0)
        {
            n = creer_noeud(FIN, "fin", token, strlen(token));
            ajouter_frere(courant, n);
            break; // Sort de la boucle car nous avons atteint la fin de la requête
        }
        else if (isdigit(token[0]))
        {
            // Token est un nombre
            n = creer_noeud(NOMBRE, token, token, strlen(token));
            ajouter_fils(courant, n);
        }
        else
        {
            // Token est un mot
            n = creer_noeud(MOT, token, token, strlen(token));
            ajouter_fils(courant, n);
        }

        token = strtok(NULL, " \t\n"); // Passe au prochain token

        // Si le token suivant est un séparateur, l'ajouter comme frère du nœud courant
        if (token != NULL && strchr(",.!?:", token[0]) != NULL)
        {
            Noeud *separateur = creer_noeud(SEPARATEUR, token, token, strlen(token));
            ajouter_frere(n, separateur);
            token = strtok(NULL, " \t\n"); // Passe au prochain token
        }

        courant = n; // Met à jour le nœud courant
    }

    return racine;
}
