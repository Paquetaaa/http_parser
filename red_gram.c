/* Notes :
Ce fichier sert a la fois a implementer les fonctionnalites essentielles et a les tester sur la grammaires reduites donnees a titre d'exemple.
Lorsqu'il faudra passer sur la grammaire complete, 3 fichiers (minimum) sont a prevoir :
-> api (.c ne devant qu'implementer les fonctions contenus dans son header et .h);
-> parseur (.c ne devant qu'implementer les fonctionnalites essentielles et avancees et .h);
-> main.c qui servira a lancer les tests en regroupant tous les fichiers necessaires a la construction du parseur + l'api 
    (/!\ seul fichier qui contiendra une routine main() /!\)

On pourra imaginer plus tard utiliser un fichier externe (en tant que second argument de la ligne de commande) plutot qu'une string rentree de maniere interne.
*/

#include "red_gram.h"

int main(int argc, char* argv[]){

    if(argc != 2){
        perror("usage : %s <fichier_requetes>");
    }







    return 0;
}





