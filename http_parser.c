#include "struct_parser.h"

/* Note :
 * Le fichier http_parser.c contient toutes les fonctions utiles, et uniquement utiles, pour la grammaire complete.
 * Le fichier http_parser.c est a vocation de ne plus contenir de main (au profit du fichier main.c).
 * Il contiendra une routine main tant que la periode de tests est en cours. 
*/

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

int main(int argc, char *argv[]){

    int len = strlen(argv[1]);
    const char delim = ' ';
    char* word = (char*) malloc(len * sizeof(char));
    char *word_a_jour = (char*) malloc(len * sizeof(char)); 
    strncpy(word, argv[1], len);
    strncpy(word_a_jour, word, len);

    char* tok = strtok(word, &delim);

    while(tok != NULL){
        printf("tok : %s\n", tok);
        printf("taille tok : %d\n", strlen(tok));
        printf("ce qu'il reste du parametre : %s\n", word_a_jour);
        sleep(1);
        /*
        if (strchr(".\n ", tok[0]) != NULL){
            printf("%s", tok);
        } else if (strcmp("Opex", tok) == 0){
            printf("Tok !\n");
        } else{
            printf("C'est un mot !\n");
        }
        */
        word_a_jour = word_a_jour + strlen(tok);
        tok = strtok(NULL, &delim);
        
    }

    return 0;
}
