#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"   
#include "api.h"

bool flag_unsecured_protocol;

int accept_header_verif(_Token token){
    char* accept = getElementValue(node->value,NULL);
    if(strcmp(accept,"*/*"==0)){
        return 0;
    }

    char premier = strtok(accept,"/");
    char suivant = strtok(NULL,"/");
    char suivant2 = strtok(suivant,",");

    if(strcmp(suivant2,"*")==0){
        return 0;
    }

    if(strcmp(premier,"text")==0){
        if(strcmp(suivant2,"html")==-1 && strcmp(suivant2,"css")==-1){
            flag_err=true;
            return -1;
        }
    }
    else if(strcmp(premier,"image")==0){
        if(strcmp(suivant2,"jpeg")==-1 && strcmp(suivant2,"png")==-1 && strcmp(suivant2,"gif")==-1 && strcmp(suivant2,"tiff")==-1 strcmp(suivant2,"svg+xml")==-1){
            flag_err=true;
            return -1;
        }
    }
    else if((strcmp(premier,"application")==0)){
        if(strcmp(suivant2,"ison")==-1 && strcmp(suivant2,"pdf")==-1){
            flag_err=true;
            return -1;
        }
    }
    printf("accept est bon");
    return 0;
}