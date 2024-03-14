#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


int is_sp(char sp)
{
    if (sp == " "){
        return 1;
    }
    else{
        return 0;
    }
}

int is_digit(int nombre)
{
    if (nombre >= '0' && nombre <= '9'){
        return 1;
    }
    else{
        return 0;
    }
}

int is_alpha(char lettre)
{
    if ((lettre >= 'A' && lettre <= 'Z') || (lettre >= 'a' && lettre <= 'z')){
        return 1;
    }
    else{
        return 0;
    }

}

int htab(char h)
{
    if (h == "\t"){
        return 1;
    }
    else {
        return 0;
    }
}

int lf(char h)
{
    if (h == "\n"){
        return 1;
    }
    else {
        return 0;
    }
}

int is_ponctuation(char lettre)
{
    if(lettre == ',' || lettre == '!' || lettre == "?" || lettre == ':'){
        return 1;
    }
    else{
        return 0;
    }
}

int is_separateur(char sep)
{
    if(is_sp(sep) || is_alpha(sep) || sep == "-" || sep == "_"){
        return 1;
    }
    else{
        return 0;
    }
}

int is_debut(char* phrase)
{
    if(strncmp(phrase,"start",4) == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int is_fin(char* phrase)
{
    if(strncmp(phrase,"fin",3) == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int lire_dans_fichier(char* FILE_IN)
{
    FILE* fichier_in = fopen(FILE_IN,"r");

    fseek(fichier_in, 0, SEEK_END);
    int taille = ftell(fichier_in);
    fseek(fichier_in, 0, SEEK_SET);
    
    char* ligne = malloc(taille);
    
    if(fgets(ligne,taille,fichier_in) != NULL){
        printf("rentré \n");
        printf("ligne = %s \n",ligne);
        if(is_debut(ligne)){
            printf("on a le début \n");
        }


        return 1;
    }


    free(ligne);
    fclose(fichier_in);
    return 0;
}

int is_mot(char* word){


    return 0;
}

typedef struct _token {
void *node; 
struct _token *next; 
} _Token;


void run_test(void)
{
    char nom_fichier_debut[] = "test0.txt";
    for(int i = 0; i < 10; i++){
        printf(" test sur %s \n",nom_fichier_debut);
        
        // blabla on fait les fonctions de test
        nom_fichier_debut[4] = '0' + (i + 1) % 10;
    }
    char nom_fichier[] = "test10.txt";
    for(int i = 10; i < 100; i++){
        printf(" test sur %s \n",nom_fichier);
        
        // blabla on fait les fonctions de test
        nom_fichier[5] = '0' + (i + 1) % 10;
        nom_fichier[4] = '0' + (i + 1) / 10;

    }    
}
int main(void){

    printf("Ca marche toujours pas tkt \n");
    lire_dans_fichier("test0.txt");
    //run_test();
    return 0;
}
