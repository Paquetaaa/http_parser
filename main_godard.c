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
void traiteHTTPVersion(_Token* t, message* requete, bool* flag_v, bool* flag_err);
char* effectuePercentCoding(char* msg, int *taille_msg, bool* flag_err);
void effectueDotRemoval(char* chaine, int taille_chaine, bool* flag_err);
void effectueDatationSystem(char** date);
void effectueDatation(char** date);


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
        char* path_request_target;              // Chemin absolu vers la ressource demandee
        struct stat file_stat;                  // Structure contenant les stats du fichier requete (champs "st_mode" & "st_size" interessants)
        int f_size;                             // Taille de la ressource envoyee en octet (= file_stat.st_size)


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
                    freeRequest(requete);
                    continue;
                }

            } 
        // La methode est obligatoirement presente dans la requete (envoie une erreur en reponse si tel n'est pas le cas)
            else {
                sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                flag_err = true;
                freeRequest(requete);
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
                    sendBadReponse("HTTP/1.1 501 Not Iimplemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
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
                            freeRequest(requete);
                            continue;
                        }
                    
                    } else {    // La version HTTP est obligatoirement presente (message d'erreur si tel n'est pas le cas)
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                // ... enfin la request-target (bien se rappeler que les ressources sont stockees dans /var/www/html/*)
                    purgeElement(&token);

                    char prefixe_target[] = "/var/www/html/";
                    char* request_target = "/";
                    int len_request_target = 1;
                    if ((token = searchTree(root, "request-target")) != NULL) {

                        request_target = getElementValue(token->node, &len_request_target);

                        if (len_request_target > 2000) {    // On est cense supporter des request-line d'au moins 8 000 octets... on dira que c'est notre max pour la request-target
                            sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            continue;
                        } 

                    // On effectue le percent-coding : on parse toute la request-target pour s'assurer que tous les caracteres sont ecrits formellement et pas avec %..
                    // (transforme les %.. en caractere formel pour ensuite pouvoir effectuer les operations dessus sans soucis)
                        request_target = effectuePercentCoding(request_target, &len_request_target, &flag_err);


                    // On effectue le dot-removal dans le but de ne pas pouvoir acceder a des repertoires parents par une requete mal intentionnee
                        effectueDotRemoval(request_target, len_request_target, &flag_err);

                        if (flag_err) {     // S'assure que si une erreur a ete remontee, qu'elle soit correctement traitee
                            sendBadReponse("HTTP/1.1 400 Bad Resquest\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            freeRequest(requete);
                            continue;
                        }

                    // Premiere concatenation pour former le chemin vers la target (maintenant que la request target est clean)
                        path_request_target = strcat(prefixe_target, request_target); 


                    // On traite d'abord Host avant la fin du traitement de la request-target pour s'assurer que les dernieres erreurs dans la requete seront reperees 
                    // (et car du Host besoin pour finaliser le chemin absolu)  
                    // (on part sur Host car Host-header contient la string "Host" au debut, cela evite de re-parser completement la ligne)
                        purgeElement(&token);

                        char* host = "";
                        int len_host = 0;
                        if ((token = searchTree(root, "Host")) != NULL) {
                            host = getElementValue(token->node, &len_host);

                        /* TRAITEMENT DU HOST */

                        }
                    // En version HTTP/1.1, le champs Host-header est le seul a etre obligatoirement present
                        else if (flag_v1_1 && token == NULL) {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            freeRequest(requete);
                            continue;
                        }




                    // Seconde concatenation pour former le chemin vers la target (maintenant que le host a ete traite)
                        path_request_target = strcat(host, path_request_target);

                    // Traitement de la request-target...
                    // ... d'abord on check si le fichier existe (avec stat(), on obtient des informations complementaires et si le retour est -1, le fichier n'existe pas)
                    // ATTENTION : soit le fichier n'existe pas du tout, soit l'extension n'a juste pas ete precisee !
                        if ((stat(path_request_target, &file_stat)) == -1) {

                            // Utiliser la libmagic (importation de magic.h, compilation avec -lmagic, puis se referer a "$man 3 libmagic" - notamment magic_open() puis magic_file())

                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            freeRequest(requete);
                            continue;
                        }

                    // ... s'assure que le fichier, dont l'extention est forcement precisee, est soit un repertoire, soit un fichier regulier (donc il faudra regarder le type...)
                    // PEUT-ETRE REMPLACER CELA PAR L'UTILISATION DE LA LIBMAGIC !? //
                        if (!S_ISREG(file_stat.st_mode) && !S_ISDIR(file_stat.st_mode)) {
                            sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            freeRequest(requete);
                            continue;
                        }


                        free(request_target);
                    } 
                // La request-target est obligatoirement presente dans la start-line, elle fait au minimum "/"
                    else {
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                // ATTENTION :
                // METTRE LE TRAITEMENT DES HEADERS VERIFIANT DE POSSIBLES ERREURS AVANT CEUX ECRIVANT DANS LA REPONSE !
                // LES 2 LIGNES CI-APRES DOIVENT ETRE LE DEBUT DE LA REPONSE

                // Ensuite on peut traiter les 12 headers restant, dans l'ordre ACCEPT-CONTENT-OTHERS
                    flag_v1_1 ? writeDirectClient(requete->clientId, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n")) 
                                : writeDirectClient(requete->clientId, "HTTP/1.0 200 OK\r\n", strlen("HTTP/1.0 200 OK\r\n"));  

                // Commencons par Accept
                    purgeElement(&token);

                    // ... //


                // Ensuite Accept-Encoding
                    purgeElement(&token);

                    // ... //

                
                // Regardons a present Content-Length
                    purgeElement(&token);

                    // ... //



                // Puis Content-Type
                    purgeElement(&token);

                    // ... //


                // A present regardons Transfer-Encoding (a ignorer pour une version anterieure au 1.1)
                    purgeElement(&token);
                    if ((token = searchTree(root, "Transfer-Encoding")) != NULL && flag_v1_1) {
                        _Token* transfer_codings = searchTree(token, "transfert-coding");

                    // Regarde tous les transfer-codings qui ont ete utilise (si le header est present, il doit y en avoir au moins 1)
                        for (_Token* i = transfer_codings->node; i != NULL; i = i->next) {
                            char* coding = getElementValue(i, NULL);

                        // S'assure que chaque transfer-coding fait partie de ceux connus (seulement les classiques)
                            if ((strcmp(coding, "chunked") != 0) && (strcmp(coding, "compress") != 0) && (strcmp(coding, "deflate") != 0) && (strcmp(coding, "gzip") != 0)) {
                                sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                                flag_err = true;
                                freeRequest(requete);
                                continue;
                            }

                        // S'assure que l'emetteur ne "chunked" sa requete qu'en dernier transfer-coding applique
                            if ((strcmp(coding, "chunked") == 0) && i->next != NULL) {
                                sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                                flag_err = true;
                                freeRequest(requete);
                                continue;
                            }
                        }

                    }

                    // ... //


                // Puis Cookie
                    purgeElement(&token);

                    // ... //

                // Puis Referer
                    purgeElement(&token);

                    // ... //

                
                // Puis User-Agent
                    purgeElement(&token);

                    // ... //



                // Puis Expect
                    purgeElement(&token);

                    // ... //

                // Enfin Connection
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
void sendBadReponse(char* buf, unsigned int clientId, struct sockaddr_in* clientAddress) {
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
void traiteMethode(_Token* t, Methode* m, message* requete, bool* flag_err) {
    if (strcmp("GET", getElementValue(t->node, NULL)) == 0) {
        *m = GET;
    } else if (strcmp("HEAD", getElementValue(t->node, NULL)) == 0) {
        *m = HEAD;
    } else if (strcmp("POST", getElementValue(t->node, NULL)) == 0) {    
        *m = POST;
    } else {
        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
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
        sendBadReponse("HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);     
        *flag_err = true;
    } 
// Verifie la mineure de l'emetteur : si en HTTP/1.0, on doit renvoyer du 1.0 et non du 1.1 !
    else if (version[7] == '0') {     
        *flag_v = false;  
    }                                   
// Sinon, on est assure d'etre en HTTP/1.n ou n >= 1, donc on traite comme si c'etait du 1.1 =)

    free(version); 
}

/**
 * Routine se chargeant de traiter le Percent Coding de la request-target pour n'avoir in fine que des caracteres ASCII
 * @warning Re-ecrit la request-target et modifie sa taille sur place !
*/
char* effectuePercentCoding(char* msg, int *taille_msg, bool* flag_err) {
    char* msg_final = (char*) malloc(*taille_msg * sizeof(char));

    for (int i = 0; i < *taille_msg; i++) {
        msg_final[i] = msg[i];
    }
    int taille_finale = *taille_msg;

// Parse l'integralite de la request-target a la recherche de caraceteres encodes avec des pourcents
    for (int index = 0; index < taille_finale; index++) {

        if (msg_final[index] == '%') {
            char nbr_hex[4];
            nbr_hex[0] = '0';
            nbr_hex[1] = 'x';

            if (index +2 < taille_finale) {     // S'assure que les 2 prochains caracteres sont disponibles (ils doivent etre compris entre 0 et 9 ou A et F)
                nbr_hex[2] = msg_final[index +2];
                nbr_hex[3] = msg_final[index +3];

                if ((nbr_hex[2] < 'A' || nbr_hex[2] > 'F') && (nbr_hex[2] < '0' || nbr_hex[2] > '9')) {
                    *flag_err = true;
                    break;
                }

                if ((nbr_hex[3] < 'A' || nbr_hex[3] > 'F') && (nbr_hex[3] < '0' || nbr_hex[3] > '9')) {
                    *flag_err = true;
                    break;
                }

                int nbr_int = (int) strtol(nbr_hex, NULL, 16);

                msg_final[index] = (char) nbr_int;

                for(int j = 0; j < taille_finale; j++){
                    msg_final[j-2] = msg_final[j];
                }

                taille_finale = taille_finale -2;
            } else {    // Sinon c'est que la request-target ne veut rien dire !
                *flag_err = true;
            }
        }
    }
// S'il y a une erreur de levee, autant ne pas s'embeter plus et renvoyer le message erronne, enverra une 400 Bad Request ulterieurement
    if (*flag_err) {
        return msg;
    }

    msg_final = realloc(msg_final, taille_finale * sizeof(char));
    msg_final[taille_finale] = '\0';
    *taille_msg = taille_finale;

    return msg_final;
}


/**
 * Routine se chargeant de s'assurer qu'il n'y a aucune tentative de remonter a un repertoire parent dans la request-target
 * @warning Le Dot-Removal se charge ici de directement lever une erreur lorsqu'une tentative est reperee, ne re-ecrit pas la request-target SANS les ".."
*/
void effectueDotRemoval(char* chaine, int taille_chaine, bool* flag_err) {
// Parse la request-target dans son integralite
    for (int i = 0; i < taille_chaine; i++) {

    // Si un point est trouve, s'assure qu'il n'est pas suivi par un second point
        if (chaine[i] == '.') {
            if (i+1 < chaine && chaine[i +1] == '.') {  // Sinon fait remonter l'erreur pour provoquer un 400 Bad Request
                *flag_err = true;
            }
        }
    }

}



/**
 * Routine qui se charge d'obtenir la date via un appel syste
 * @warning DOIT ETRE ROOT POUR UTILISER CETTE ROUTINE
*/
void effectueDatationSystem(char** date) {
    char path[] = "/var/www/date.txt";
    FILE* f_date;
    size_t len;
    *date = NULL;

    system("date > /var/www/date.txt");

    f_date = fopen(path, "r");

    getline(date, &len, f_date);

    fclose(f_date);

    system("rm /var/www/date.txt");
}


/**
 * Routine qui se charge d'obtenir la date du jour 
*/
void effectueDatation(char** date) {
    date = __DATE__;
}


