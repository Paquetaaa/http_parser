#include "api.h"
#include "struct_parser.h"
#include "http_parser.h"

Noeud* abr; 

void* getRootTree(){
    return abr;
}

_Token *searchTree(void *start, char *name){

/* Cas de start nul : on commence a la racine */
    if (start == NULL){
        if (getRootTree() == NULL){
            fprintf(stderr, "Erreur getRootTree dans <searchTree>");
            exit(1);
        }
        return searchTree(getRootTree(), name);
    }

    _Token* token = (_Token*) malloc(sizeof(_Token));
    token->node = NULL;
    token->next = NULL;

    _Token* tk_courant = token;
    int flag = -1;

/* On va parcourir dans une boucle for les fils et dans une boucle while imbriquee tous les freres de chaque noeud courant */
    for(Noeud* nd_courant = start; nd_courant != NULL; nd_courant = nd_courant->fils_aine){

        if (strcmp(nd_courant->etiquette, name) == 0){
            if (flag == -1){                // Cas ou la tete de la liste chainee qui sera retournee n'est pas encore creee
                tk_courant->node = nd_courant;
                flag = 1;
            } else{                         // Sinon on cree un nouvel element qui vient s'ajouter en queue de la liste chainee
                _Token* nv = (_Token*) malloc(sizeof(_Token));
                nv->node = nd_courant;
                nv->next = NULL;
                tk_courant->next = nv;
                tk_courant = tk_courant->next;
            }
        }
    /* Parcours de tous les freres en explorant la liste chainee de ces derniers */
        Noeud* frere_courant = nd_courant;

        while(frere_courant->frere != NULL){
            frere_courant = frere_courant->frere;

            if (strcmp(frere_courant->etiquette, name) == 0){
                if (flag == -1){                // Cas ou la tete de la liste chainee qui sera retournee n'est pas encore creee
                    tk_courant->node = frere_courant;
                    flag = 1;
                } else{                         // Sinon on cree un nouvel element qui vient s'ajouter en queue de la liste chainee
                    _Token* nv = (_Token*) malloc(sizeof(_Token));
                    nv->node = frere_courant;
                    nv->next = NULL;
                    tk_courant->next = nv;
                    tk_courant = tk_courant->next;
                }
            }
        }
    /* On sort de la boucle while lorsque tous les freres ont ete checke; on peut donc passer au fils suivant (laisser l'iteration suivante de la boucle for s'executer) */   
    }

    if (token->node == NULL){
        return NULL;
    }

    return token;
}

char* getElementTag(void* node, int* len){
    Noeud* nd = node;

    if (len != NULL){
        *len = nd->longueur;
    }

    return nd->etiquette;
}

char* getElementValue(void* node, int* len){
    Noeud* nd = node;

    if (len != NULL){
        *len = nd->longueur;
    }

    return nd->pointeur_debut;
}

void purgeElement(_Token** r){
    if (r == NULL || *r == NULL){
        return;
    }

    if ((*r)->next != NULL){
        purgeElement(&(*r)->next);
    }

    free((*r)->node);
    free(*r);
}

void purgeTree(void* root){
    if (root == NULL){
        return;
    }

    Noeud* courant = root;

    if (courant->frere != NULL){
        purgeTree(courant->frere);
    }

    if (courant->fils_aine != NULL){
        purgeTree(courant->fils_aine);
    }

    free(courant->etiquette);
    free(courant->pointeur_debut);
    free(courant);
}

int parseur(char* req, int len){

    abr = construire_arbre(req);
}


