#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "fastcgi.h"
#include "api.h"
#include "request.h"

/*
Pour params, la bonne moitié faut juste lire les champs "header-fields" dans l'arbre et les balancer
pour le reste, on les genère en regardant la ou il faut

La fonction pour créer les params et les écrire dans la socket : static char* createRequeteParams()


*/

// J'essaie de prendre tous les headers http et les foutre quelque part
// fd c'est le file descriptor
char* ecrire_http_header()
{
    void *root = getRootTree(); 
    _Token* token = searchTree(root, "header_field");
    token = token->next;
    char *header_http = NULL;
    // token -> node c'est le contenu, -> next c'est l'élement d'après
    while(token != NULL)
    {
        char *reponse = "HTTP_";
        char* value = getElementValue(token->node,NULL);
        char* tag = getElementTag(token->node,NULL);
        strcat(reponse,tag);
        reponse[strlen(reponse)] = ' ';
        strcat(reponse,"= ");
        strcat(reponse,value);
        strcat(header_http,reponse);
        strcat(header_http,"\r\n");
        token = token->next;
    }
    return header_http;
}

char* ecrire_fcgi_header()
{
    char* reponse;
    char* retour = "\r\n";

    char* server_name = "127.0.0.1";
    char* server_addr = "127.0.0.1";
    char* server_port = 80;
    char* document_root = "/var/www/html";
    

    void *root = getRootTree(); 
    _Token* token = searchTree(root, "request_target");
    char* requestURI = getElementValue(token->node,NULL);

    char* current;
    strcpy(current,requestURI);
    char* next = strtok(current,'/');
    while(next != NULL)
    {
        current = next;
        next = strtok(NULL,'/');
    }
    char* scriptname;
    strcpy(scriptname,current);

    char* script_filename = "proxy:fcgi://";
    char* port = "9000/";

    strcat(script_filename,port);
    strcat(script_filename,document_root);
    strcat(script_filename,scriptname);
    strcat(script_filename,retour);
    strcat(reponse,script_filename);

    strcat(server_addr,retour);
    strcat(reponse,server_addr);
   
    strcat(server_port,retour);
    strcat(reponse,server_port);
   
    strcat(document_root,retour);
    strcat(reponse,document_root);
    
    strcat(requestURI,retour);
    strcat(reponse,requestURI);
   
    strcat(scriptname,retour);
    strcat(reponse,scriptname);
   
    strcat(port,retour);
    strcat(reponse,port);
   
    return reponse;
}


static FCGI_Header* createRequeteParams(int fd)
{
    // type
    FCGI_Header* header = malloc(sizeof(FCGI_Header));
    header->type = '4';

    // version
    void *root = getRootTree(); 
    _Token* token = searchTree(root, "HTTP_version");
    char *version = getElementValue(token->node, NULL);
    header->version = version;
    header->requestId = 1;
    
    char* http_headers = ecrire_http_header();
    char* fcgi_headers = ecrire_fcgi_header();

    char* params = malloc((strlen(http_headers)+4+strlen(fcgi_headers))*sizeof(char));
    strcat(params,http_headers);
    strcat(params,"\r\n");
    strcat(params,fastcgi_headers);

    header->contentData = params;
    
    //write(fd,header,sizeof(header)); vu qu'on le retourne pas besoin de l'écrire

    return header;
}
