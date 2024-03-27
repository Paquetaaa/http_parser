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
void run_test(void)
{

    char nom_fichier_debut[] = "test0.txt";
    for (int i = 0; i < 10; i++)
    {
        printf(" test sur %s \n", nom_fichier_debut);

        FILE *Fplecture = fopen(nom_fichier_debut, "r");

        lire_et_analyser_requete(Fplecture);

        fclose(Fplecture);

        printf("\n");
        printf("\n");

        // blabla on fait les fonctions de test
        nom_fichier_debut[4] = '0' + (i + 1) % 10;
    }
    char nom_fichier[] = "test10.txt";
    for (int i = 10; i < 100; i++)
    {
        printf(" test sur %s \n", nom_fichier);

        FILE *Fplecture = fopen(nom_fichier, "r");

        lire_et_analyser_requete(Fplecture);

        fclose(Fplecture);

        // blabla on fait les fonctions de test
        nom_fichier[5] = '0' + (i + 1) % 10;
        nom_fichier[4] = '0' + (i + 1) / 10;
    }
}

int main(int argc, char *argv[])
{

    run_test();

    return 0;
}
