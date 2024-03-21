#include <stdio.h>
#include <string.h>
#include "redgrammm.h"

void lire_et_analyser_requete(FILE *Fplecture)
{
    size_t len = 0;
    ssize_t read;
    char *ligne = NULL;
    while ((read = getline(&ligne, &len, Fplecture)) != -1)
    {
        Noeud *racine = construire_arbre(ligne);
        afficher_arbre(racine, 0);
    }
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("usage: ./http_parser <test_file> \n");
        return 1;
    }

    FILE *Fplecture = fopen(argv[1], "r");

    lire_et_analyser_requete(Fplecture);

    fclose(Fplecture);

    return 0;
}
