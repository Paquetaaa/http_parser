#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"   


// Attention j'ai pas fait de fonction, faut juste rajouter tout ca dans le main  : //

/* le nom des flags va changer biensur*/
bool flag_v; 
int transfer_encoding;
bool flag_err;

// Partie Content Length
  if ((token = searchTree(root, "Content_length")) != NULL) {
    if(transfer_encoding){
        printf("problème : content length et transfer_encoding \n");
        flag_err = true;
        continue;
        //return 400;
    }
    if(token->next != NULL){ // Ya plusieurs content-length : faut qu'ils soient tous égayx
        while(token)
            {
                int value = atoi(token);
                if(premiere_valeur == -100000000){
                    premiere_valeur = value;
                }
                else if(premiere_valeur != value){
                    flag_err = true;
                    printf("erreur dans les champs \n");
                    //return 400;
                    continue;
                }
                token = strtok(NULL,",");
            }
    }
    // Si un seul, le parser a déjà vérifié que c'était correct donc est bon, le champ content length est bon
    printf("Content Length vérifié, on est bien \n"); 
    flag_err = false;
  }
                    

// Partie Host : a partir de purge token 
token = searchTree(root,"Host");
if(token == NULL && flag_v){
    printf("Pas de host alors qu'on est en 1.1 : erreur \n");
    flag_err = true;
    continue;
    //return 400;
}
if(token != NULL && token->next != NULL){ // Plusieurs hosts
    printf("plusieurs hosts \n");
    flag_err = true;
    continue;
    //return 400;
}
// Le parser s'occupe de vérifier si l'ip est valide ou si le nom est valide donc est bon pour l'host
// Pareil si ya un port

// Partie Connection header
purgeElement(&token);

int len_connection;
token = searchTree(root,"Connection");
char *valeur = getElementValue(token->node, &len_connection);
    if(strcmp(valeur,"close") == 0){
        flag_connection_keepalive = false;
    }
    else if(flag_v){
        flag_connection_keepalive = true;
    }
    else if (!flagv && (!strcmp(valeur,"keep-alive") || !strcmp(valeur,"Keep-Alive"))){
        flag_connection_keepalive = true;
    }
    else {
        flag_connection_keepalive = false;
    }

purgeElement(&token);
