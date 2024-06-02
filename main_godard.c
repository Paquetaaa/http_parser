#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"   
#include "api.h"   

#define REPONSEGOOD "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n"
#define REPONSEBAD "HTTP/1.0 400 Bad Request\r\n\r\n"

typedef enum methode {
    GET,
    HEAD,
    POST
} Methode;

void sendBadReponse(char* buf, unsigned int clientId, struct sockaddr_in* clientAddress);
void traiteMethode(_Token* t, Methode* m, message* requete, bool* flag_err);


// ceci decrit le squelette global de l'analyseur semantique

int main(int argc, char *argv[])
{
	message *requete; 

	while ( 1 ) {
	// On attend la reception d'une requete HTTP, requete pointera vers une ressource allouée par librequest. 
		if ((requete=getRequest(8080)) == NULL ) return -1; // Il faudra penser a faire tourner le code sur le port 80 (HTTP)


	// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);  


    // Declaration de quelques variables globales au traitement de chaque header
        bool flag_err = false;
        bool flag_v1_1 = true;                  // On part du principe qu'on encode le serveur sur une version HTTP/1.1...
        bool flag_connection_keepalive = true; // ... d'ou le keepalive par defaut
        struct stat file_stat;                  // Structure contenant les stats du fichier requete (champs "st_mode" & "st_size" interessants)
        int f_size;


    // Verifie que le parser s'est execute correctement (et recupere la strcuture qu'il en sort par la meme occasion)
		if ( parseur(requete->buf,requete->len) == 1 ) { 

			void* root = getRootTree();     // Racine de l'arbre stockant tous les elements syntaxiques de la requete recue
            _Token* token;


        // On regarde d'abord la request-line recue...
        // ... d'abord la methode (GET / HEAD / POST seulement)
            Methode methode;

            if ((token = searchTree(root, "method")) != NULL) {
                
                traiteMethode(token, &methode, requete, &flag_err);

            // Regarde si lors du traitement une erreur n'a pas ete detectee 
            // (si tel est le cas : msg d'erreur deja envoye, on passe a une autre requete)
                if (flag_err){
                    continue;
                }

            } 
        // La methode est obligatoirement presente dans la requete (envoie une erreur en reponse si tel n'est pas le cas)
            else {
                sendBadReponse("HTTP/1.1 400 Bad Request\r\n", requete->clientId, requete->clientAddress);
                flag_err = true;
                continue;
            }
			
        // Toutes les analyses semantiques des autres headers devront se faire selon la methode
            switch (methode) {
                case HEAD:

                    // ... //
                    // Presque une recopie du GET, juste sans faire de message-body. A voir s'il n'est pas plus judicieux de regarder dans quelle methode on est
                    // lors de l'ecriture du message body plutot que de rajouter 2x plus de ligne in fine...
                    // ... //

                    break;

                case POST:  // La methode POST sera implementee plus tard
                    sendBadReponse("HTTP/1.1 501 Not Iimplemented\r\n", requete->clientId, requete->clientAddress);
                    flag_err = true;
                    continue;

                    break;

                default:    // La methode GET etant celle utilisee a 90% du temps, on considerera celle-ci comme celle par defaut (si erreur, detectee au if precedent)
            

        // ... puis la version HTTP de l'emetteur
                    purgeElement(&token);

                    if ((token = searchTree(root, "HTTP-version")) != NULL) {

                        traiteHTTPVersion(token, requete, &flag_v1_1, &flag_err);

                    // Regarde si lors du traitement une erreur n'a pas ete detectee 
                    // (si tel est le cas : msg d'erreur deja envoye, on passe a une autre requete)
                        if (flag_err){
                            continue;
                        }
                    
                    } else {    // La version HTTP est obligatoirement presente (message d'erreur si tel n'est pas le cas)
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\n", requete->clientId, requete->clientAddress);
                        flag_err = true;
                        continue;
                    }

                // ... enfin la request-target (bien se rappeler que les ressources sont stockees dans /var/www/html/*)
                    purgeElement(&token);

                    char prefixe_target[] = "/var/www/html/";
                    char* request_target;
                    int len_request_target;
                    if ((token = searchTree(root, "request-target")) != NULL) {

                        request_target = getElementValue(token->node, &len_request_target);

                        if (len_request_target > 2000) {    // On est cense supporter des request-line d'au moins 8 000 octets... on dira que c'est notre max pour la request-target
                            sendBadReponse("HTTP/1.1 501 Not Iimplemented\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            continue;
                        } 

                    // On effectue le percent-coding : on parse toute la request-target pour s'assurer que tous les caracteres sont ecrits formellement et pas avec %..
                    // (transforme les %.. en caractere formel pour ensuite pouvoir effectuer les operations dessus sans soucis)

                        // PERCENT CODING //


                    // On effectue le dot-removal dans le but de ne pas pouvoir acceder a des repertoires parents par une requete mal intentionnee
                        
                        // DOT REMOVAL //


                        request_target = strcat(prefixe_target, request_target); // devient "path_request_target" en quelques sortes

                    // Traitement de la request-target...
                    // ... d'abord on check si le fichier existe (avec stat(), on obtient des informations complementaires et si le retour est -1, le fichier n'existe pas)
                        if ((stat(request_target, &file_stat)) == -1) {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            continue;
                        }






                        free(request_target);
                    }

                // Ensuite on peut traiter les 13 headers, dans un certain ordre (HOST, ACCEPT, CONTENT, OTHERS)  
                // D'abord Host-header (on part sur Host car Host-header contient la string "Host" au debut, cela evite de re-parser completement la ligne)
                    purgeElement(&token);

                    char* host;
                    int len_host;
                    if ((token = searchTree(root, "Host")) != NULL) {
                        host = getElementValue(token->node, &len_host);

                    /* TRAITEMENT DU HOST */

                    }
                // En version HTTP/1.1, le champs Host-header est le seul a etre obligatoirement present
                    else if (flag_v1_1 && token == NULL) {
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\n", requete->clientId, requete->clientAddress);
                        flag_err = true;
                        continue;
                    }

                // Ensuite les headers Accept-*
                // Commencons par Accept
                    purgeElement(&token);

                    // ... //





                        
















                    
                    break;
            
                }



			
			/*
             * Routine pour ecrire dans le buffer une ligne de la reponse :
             * writeDirectClient(requete->clientId, "msg", strlen("msg")); 
             *
             * Rq : preferer print("msg") pour le moment, facilite la comprehension et le debug et la verification de bon fonctionnement
             */


		} 
		else {
			writeDirectClient(requete->clientId,REPONSEBAD,strlen(REPONSEBAD)); 
		}

    // Verifie s'il y a au moins une erreur lors du traitement de la requete (si oui, une BAD RESPONSE aura ete envoye, pas la peine d'envoyer le buffer contenant la GOOD RESPONSE)
        if (!flag_err) {
            endWriteDirectClient(requete->clientId); 
        }

    // Garde la connection ouverte selon la version HTTP (1.1 ici donc keepalive par defaut) et du champs Connection-header s'il est present (keepalive | close)
        if (!flag_connection_keepalive) {
            requestShutdownSocket(requete->clientId); 	
        }
		
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requete);
	}

	return (1);
}



// Routines secondaires 

/** 
 * Routine pour envoyer un message d'erreur a l'emetteur si la requete contient des erreurs
*/
void sendBadReponse(char* buf, unsigned int clientId, struct sockaddr_in* clientAddress){
    message* reponse = (message*) malloc(sizeof(message));

    reponse->buf = buf;
    reponse->len = strlen(buf);
    reponse->clientId = clientId;
    reponse->clientAddress = clientAddress;

    sendReponse(reponse);
    free(reponse);
}

/**
 * Routine qui se charge de traiter le champs METHODE de la requete
*/
void traiteMethode(_Token* t, Methode* m, message* requete, bool* flag_err){
    if (strcmp("GET", getElementValue(t->node, NULL)) == 0) {
        *m = GET;
    } else if (strcmp("HEAD", getElementValue(t->node, NULL)) == 0) {
        *m = HEAD;
    } else if (strcmp("POST", getElementValue(t->node, NULL)) == 0) {    
        *m = POST;
    } else {
        sendBadReponse("HTTP/1.1 400 Bad Request\r\n", requete->clientId, requete->clientAddress);
        *flag_err = true;
    }
}

/**
 * Routine qui se charge de traiter le champs HTTP-Version de la requete
*/
void traiteHTTPVersion(_Token* t, message* requete, bool* flag_v, bool* flag_err) {

    char* version = getElementValue(t->node, NULL);

// Verifie que la majeure est bien 1 (si 0 ou 2 ou 3, version non supportee par le serveur)
    if (version[5] != '1') {
        sendBadReponse("HTTP/1.1 505 HTTP Version Not Supported\r\n", requete->clientId, requete->clientAddress);     
        *flag_err = true;
    } 
// Verifie la mineure de l'emetteur : si en HTTP/1.0, on doit renvoyer du 1.0 et non du 1.1 !
    else if (version[7] == '0') {     
        *flag_v = false;  
    }                                   
// Sinon, on est assure d'etre en HTTP/1.n ou n >= 1, donc on traite comme si c'etait du 1.1 =)

    free(version); 
}



