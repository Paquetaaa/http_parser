#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"   




/* le nom des flags va changer biensur*/
int v11; 
int transfer_encoding;

int flag_err;
int content_length_verif(_Token* node)
{
    char* cl = getElementValue(node,"Content_length");
    if(transfer_encoding){
        return 0;
    }
    // ya qu'un truc à vérifier à ce stade la : si yen a plusieurs faut que ce soit les mêmes et qu'elles soient toutes valides une par une
    char* cl_copy;
    strcpy(cl_copy,cl);
    char* token = strtok(contentLengthCopy, ","); // les différentes valeurs sont séparées par une ,
    int premiere_valeur = -100000000;
    while(token)
    {
        for(int i = 0; i < strlen(token); i++){
            if(!isdigit(token[i])){
                return 0;
            }
        }
        int value = atoi(token);
        if(premiere_valeur == -100000000){
            premiere_valeur = value;
        }
        else if(premiere_valeur != value){
            return 0;
        }
        token = strtok(NULL,",");
    }
    // si on a passé tout ça c'est que le content-length est valide
    return 1;
}


// fonction a appeler après :
/*
if ((_Token* h1 = searchTree(root,HOST)) == NULL) && v11) : faire une erreur , faut qu'il y en ai un
else {
    host_verif()
}

*/
int host_verif(_Token* node)
{
    const char* host = getElementValue(node, "Host");
        if (strlen(host) == 0) {
        return 0;
    }
    // on sépare le port de l'uri host
    char* host_cpy;
    strcpy(host_cpy,host);
    char* debut_port = strchr(host_cpy,':');
    // faut vérifier que le port c'est que des chiffres
        
}

int connection_header_verif(_Token* arbre)
{
    return 0;
}