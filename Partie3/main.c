/**
 * @author Mazabrard G. - Greneche L. - N'Koumo-Mobio S. - Godard E.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <magic.h>
#include <ctype.h>
#include <time.h>
#include "request.h"
#include "api.h"

#define REPONSEGOOD "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n"
#define REPONSEBAD "HTTP/1.0 400 Bad Request\r\n\r\n"
#define MAX_PATH_SIZE 300

typedef enum methode
{
    GET,
    HEAD,
    POST
} Methode;

typedef struct
{
    char *name;
    char *value;
    char *domain;
    char *path;
    bool secure;
    bool http_only_flag;
    bool host_only_flag;
    char *max_age;
    time_t last_access_time;
} Cookie;

// Cookie//
Cookie *get_cookies_from_store();
void update_last_access_time(Cookie *cookie);
void cookie_header_verif(_Token *node, _Token *root, bool* flag_err, char* path_target);
void update_last_access_time(Cookie *cookie);
bool path_matches(const char *request_path, const char *cookie_path);
bool domain_matches(const char *request_host, const char *cookie_domain);

const char default_path[] = "/var/www/html/";

// Magasin de Cookie, j'ai pas capté comment on allait le remplir.
static Cookie cookie_store[] = {
    {"name1", "value1", "domain1", "path1", true, true, true, "Max_age1", 0},
    {"name2", "value2", "domain2", "path2", false, true, false, "Max_age2", 0},
    {NULL, NULL, NULL, NULL, 0, 0} // Sentinelle.
};


// Expect//
void expect_header_verif(_Token *node, bool flag_v1_1, char *message_body);


void sendBadReponse(char *buf, unsigned int clientId, struct sockaddr_in *clientAddress);
void traiteMethode(_Token *t, Methode *m, message *requete, bool *flag_err);
void traiteHTTPVersion(_Token *t, message *requete, bool *flag_v, bool *flag_err);
char *effectuePercentCoding(char *msg, int *taille_msg, bool *flag_err);
void effectueDotRemoval(char *chaine, int taille_chaine, bool *flag_err);
void effectueDatationSystem(char **date);
void effectueDatation(char **date);

// ceci decrit le squelette global de l'analyseur semantique

int main(int argc, char *argv[])
{
    message *requete;

    while (1)
    {
        // On attend la reception d'une requete HTTP, requete pointera vers une ressource allouée par librequest.
        if ((requete = getRequest(8080)) == NULL)
            return -1; // Il faudra penser a faire tourner le code sur le port 80 (HTTP)

        // Affichage de debug
        printf("#########################################\nDemande recue depuis le client %d\n", requete->clientId);
        printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
        printf("Contenu de la demande %.*s\n\n", requete->len, requete->buf);

        // Declaration de quelques variables globales au traitement de chaque header
        bool flag_err = false;
        bool flag_v1_1 = true;                                                                // On part du principe qu'on encode le serveur sur une version HTTP/1.1...
        bool flag_connection_keepalive = true;                                                // ... d'ou le keepalive par defaut
        char *path_request_target = (char*) malloc(MAX_PATH_SIZE * sizeof(char));             // Chemin absolu vers la ressource demandee
        struct stat file_stat;                                                                // Structure contenant les stats du fichier requete (champs "st_mode" & "st_size" interessants)
        int f_size;                                                                           // Taille de la ressource envoyee en octet (= file_stat.st_size)
        char *f_type;                                                                         // String contenant le(s) type(s) du/des fichier(s) renvoye(s) en reponse
        char *host_filter;                                                                    // String contenant le host sans prefixe (e.g. www.) et sans suffixe (e.g. .fr)
        int f_descripteur;
        magic_t file_magic = magic_open(MAGIC_MIME);                                          // Ouvre le cookie qui servira a determiner le type du fichier requete
        magic_load(file_magic, NULL);                                                         // Charge le cookie, maintenant pret a etre utilise !

        // Verifie que le parser s'est execute correctement (et recupere la strcuture qu'il en sort par la meme occasion)
        if (parseur(requete->buf, requete->len) == 1)
        {

            void *root = getRootTree(); // Racine de l'arbre stockant tous les elements syntaxiques de la requete recue
            _Token *token;

            // On regarde d'abord la request-line recue...
            // ... d'abord la methode (GET / HEAD / POST seulement)
            Methode methode;

            if ((token = searchTree(root, "method")) != NULL)
            {

                traiteMethode(token, &methode, requete, &flag_err);

                // Regarde si lors du traitement une erreur n'a pas ete detectee
                // (si tel est le cas : msg d'erreur deja envoye, on passe a une autre requete)
                if (flag_err)
                {
                    freeRequest(requete);
                    continue;
                }
            }
            // La methode est obligatoirement presente dans la requete (envoie une erreur en reponse si tel n'est pas le cas)
            else
            {
                sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                flag_err = true;
                freeRequest(requete);
                continue;
            }

            // Toutes les analyses semantiques des autres headers devront se faire selon la methode
            switch (methode)
            {
            case POST: // La methode POST sera implementee plus tard
                sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                flag_err = true;
                continue;

                break;

            default: // La methode GET et HEAD etant celles utilisees a 90% du temps, on considerera celle-ci comme celle par defaut (si erreur, detectee au if precedent)

                // ... puis la version HTTP de l'emetteur
                purgeElement(&token);

                if ((token = searchTree(root, "HTTP_version")) != NULL)
                {

                    traiteHTTPVersion(token, requete, &flag_v1_1, &flag_err);

                // Changement du comportement par defaut de l'usage de la connexion (1.0 => close)
                    if (!flag_v1_1) {
                        flag_connection_keepalive = false;
                    }

                // Regarde si lors du traitement une erreur n'a pas ete detectee
                // (si tel est le cas : msg d'erreur deja envoye, on passe a une autre requete)
                    if (flag_err)
                    {
                        freeRequest(requete);
                        continue;
                    }
                }
                else
                { // La version HTTP est obligatoirement presente (message d'erreur si tel n'est pas le cas)
                    if (!flag_connection_keepalive)
                    {
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                        requestShutdownSocket(requete->clientId);
                    } else {
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                    }
                                                
                    
                    flag_err = true;
                    freeRequest(requete);
                    continue;
                }

                // ... enfin la request-target (bien se rappeler que les ressources sont stockees dans /var/www/html/*)
                purgeElement(&token);

                char *prefixe_target = "/var/www/html";
                char *request_target;
                int len_request_target;
                if ((token = searchTree(root, "request_target")) != NULL)
                {

                    request_target = getElementValue(token->node, &len_request_target);

                    if (len_request_target > 2000)
                    { // On est cense supporter des request-line d'au moins 8 000 octets... on dira que c'est notre max pour la request-target
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                    // On effectue le percent-coding : on parse toute la request-target pour s'assurer que tous les caracteres sont ecrits formellement et pas avec %..
                    // (transforme les %.. en caractere formel pour ensuite pouvoir effectuer les operations dessus sans soucis)
                    request_target = effectuePercentCoding(request_target, &len_request_target, &flag_err);

                    printf("request_target : %s\n", request_target);

                    if (flag_err) {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        freeRequest(requete);
                        continue;
                    }

                    // On effectue le dot-removal dans le but de ne pas pouvoir acceder a des repertoires parents par une requete mal intentionnee
                    effectueDotRemoval(request_target, len_request_target, &flag_err);

                    if (flag_err)
                    { // S'assure que si une erreur a ete remontee, qu'elle soit correctement traitee
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        freeRequest(requete);
                        continue;
                    }

                    // Premiere concatenation pour former le chemin vers la target (maintenant que la request target est clean)
                    strcpy(path_request_target, prefixe_target);

                    // On traite d'abord Host avant la fin du traitement de la request-target pour s'assurer que les dernieres erreurs dans la requete seront reperees
                    // (et car du Host besoin pour finaliser le chemin absolu)
                    // (on part sur Host car Host-header contient la string "Host" au debut, cela evite de re-parser completement la ligne)
                    purgeElement(&token);

                    char *host;
                    int len_host;
                    if ((token = searchTree(root, "Host")) != NULL)
                    {

                        if (token->next != NULL)
                        { // Plusieurs hosts, erreur !
                            if (!flag_connection_keepalive)
                            {
                                sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                                requestShutdownSocket(requete->clientId);
                            } else {
                                sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            }
                            flag_err = true;
                            freeRequest(requete);
                            continue;
                        }
                        // Le parser s'occupe de vérifier si l'ip est valide ou si le nom est valide donc c'est bon pour l'host
                        // Pareil si y'a un port

                        /* TRAITEMENT DU HOST */
                        host = getElementValue(token->node, &len_host);

                    // Uniquement pour les tests internes : a la place de faire du nc -C 127.0.0.1 8080 :)
                        if (strstr(host, "127.0.0.1") != NULL) {
                            host_filter = ".";
                        } else {
                            host_filter = strtok(host, ".");
                            host_filter = strtok(NULL, "."); // Obtient tout ce qui est apres le prefixe (e.g. "www.")
                        }

                    }
                    // En version HTTP/1.1, le champs Host-header est le seul a etre obligatoirement present
                    else if (flag_v1_1 && token == NULL)
                    {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                    // Regarde (via une sorte de pattern maching) si le host rentre est correcte pour nous
                    // (pas obligatoire car le fichier ne sera pas ouvrable si de toutes manieres l'host ne veut rien dire pour nous)

                        // ... //

                    // Seconde concatenation pour former le chemin vers la target (maintenant que le host a ete traite)
                    // (s'assure tout de meme que le host ait ete traite !)
                    if (token != NULL) {
                        strcat(path_request_target, "/");
                        strcat(path_request_target, host_filter);
                        strcat(path_request_target, "/");
                    }

                    // /!\ ATTENTION : LA VARIABLE $LD_LIBRARY_PATH S'EST RAJOUTEE AU DEBUT DE path_request_target !!!
                    strcat(path_request_target, request_target);

                    // Traitement de la request-target...
                    //... d'abord on s'assure qu'on ne demande pas la racine (on n'accepte pas les repertoires, uniquements les fichiers reguliers)
                    if (strcmp(path_request_target, "/") == 0) {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                    // ... ensuite on check si le fichier existe (avec stat(), on obtient des informations complementaires et si le retour est -1, le fichier n'existe pas)
                    // ATTENTION : soit le fichier n'existe pas du tout, soit l'extension n'a juste pas ete precisee !
                    if ((stat(path_request_target, &file_stat)) == -1)
                    {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                    // ... on s'assure que le fichier designe / ouvrable est bien un fichier regulier (on pourrait etendre renvoyer la liste des fichiers dispo dans un repertoire...)
                    // (donc il faudra regarder le type...)
                    if (!S_ISREG(file_stat.st_mode))
                    {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                    // Determine le type du fichier ouvert via la libmagic
                    f_type = magic_file(file_magic, path_request_target);

                    
                }
                // La request-target est obligatoirement presente dans la start-line, elle fait au minimum "/"
                else
                {
                    if (!flag_connection_keepalive)
                    {
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                        requestShutdownSocket(requete->clientId);
                    } else {
                        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                    }
                    flag_err = true;
                    freeRequest(requete);
                    continue;
                }

                // Ensuite on peut traiter les 12 headers restant, dans l'ordre ACCEPT-CONTENT-OTHERS
                // Commencons par Accept
                // purgeElement(&token);

                // ... //

                // Ensuite Accept-Encoding
                // purgeElement(&token);

                // ... //

                // Regardons a present Content-Length
                purgeElement(&token);
                if ((token = searchTree(root, "Content_Length")) != NULL)
                {

                    // Il ne doit pas y avoir les header-fields Content-Length et Transfer-Encoding en meme temps
                    if (searchTree(root, "Transfer_Encoding") != NULL)
                    {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }

                    if (token->next != NULL)
                    { // Y'a plusieurs content-length : faut qu'ils soient tous égaux
                        while (token != NULL)
                        {
                            int value = atoi(getElementValue(token->node, NULL));
                            int premiere_valeur = value;

                            if (premiere_valeur == -100000000)
                            {
                                premiere_valeur = value;
                            }

                            else if (premiere_valeur != value)
                            {
                                if (!flag_connection_keepalive)
                                {
                                    sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                                    requestShutdownSocket(requete->clientId);
                                } else {
                                    sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                                }
                                flag_err = true;
                                freeRequest(requete);
                                continue;
                            }
                            token = token->next;
                        }
                    }
                    // Si un seul de present, le parser a déjà vérifié que c'était correct donc c'est bon, le champ content-length est bon
                }

                // Puis Content-Type
                // purgeElement(&token);

                    // ... //

                // A present regardons Transfer-Encoding (a ignorer pour une version anterieure au 1.1)
                purgeElement(&token);
                if ((token = searchTree(root, "Transfer_Encoding")) != NULL && flag_v1_1)
                {
                    _Token *transfer_codings = searchTree(token, "transfert_coding");

                    // Regarde tous les transfer-codings qui ont ete utilise (si le header est present, il doit y en avoir au moins 1)
                    for (_Token *i = transfer_codings->node; i != NULL; i = i->next)
                    {
                        char *coding = getElementValue(i, NULL);

                        // S'assure que chaque transfer-coding fait partie de ceux connus (seulement les classiques)
                        if ((strcmp(coding, "chunked") != 0) && (strcmp(coding, "compress") != 0) && (strcmp(coding, "deflate") != 0) && (strcmp(coding, "gzip") != 0))
                        {
                            sendBadReponse("HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            freeRequest(requete);
                            continue;
                        }

                        // S'assure que l'emetteur ne "chunked" sa requete qu'en dernier transfer-coding applique
                        if ((strcmp(coding, "chunked") == 0) && i->next != NULL)
                        {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                            flag_err = true;
                            freeRequest(requete);
                            continue;
                        }
                    }
                }

                // Puis Cookie
                purgeElement(&token);
                if ((token = searchTree(root, "Cookie")) != NULL)
                {
                    cookie_header_verif(token, root, &flag_err, path_request_target);

                    if (flag_err) {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        freeRequest(requete);
                        continue;
                    }
                }

                // Puis Expect
                purgeElement(&token);
                if ((token = searchTree(root, "Expect")) != NULL)
                {
                    expect_header_verif(token, flag_v1_1, requete->buf);
                }


                // Enfin Connection
                purgeElement(&token);
                if ((token = searchTree(root, "connection_option")) != NULL)
                {
                    char *valeur_co = getElementValue(token, NULL);

                    // La valeur de ce header prevaut sur la qualite par defaut de la connexion
                    if (strcmp(valeur_co, "close") == 0)
                    {
                        flag_connection_keepalive = false;
                    }
                    else if (strcmp(valeur_co, "keep-alive") == 0)
                    {
                        flag_connection_keepalive = true;
                    }
                    else
                    {
                        if (!flag_connection_keepalive)
                        {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", requete->clientId, requete->clientAddress);
                            requestShutdownSocket(requete->clientId);
                        } else {
                            sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
                        }
                        flag_err = true;
                        freeRequest(requete);
                        continue;
                    }
                }

                // A partir de la on peut ecrire la reponse (plus d'erreur a detecter et toutes les var. globales utiles remplies)
                flag_v1_1 ? writeDirectClient(requete->clientId, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"))
                        : writeDirectClient(requete->clientId, "HTTP/1.0 200 OK\r\n", strlen("HTTP/1.0 200 OK\r\n"));

                // Check des caracteristiques en lecture du fichier voulu
                f_size = file_stat.st_size;
                char *lecture = (char *) malloc(f_size * sizeof(char));
                f_descripteur = open(path_request_target, O_RDONLY);
                ssize_t nb_lu = read(f_descripteur, lecture, f_size);

                if (nb_lu < f_size)
                { // f_size exprime la quantite en octets, nb_lu exprime la quantite en nombre d'objets lu
                    lecture = realloc(lecture, nb_lu * sizeof(char));
                    f_size = nb_lu;
                }

                close(f_descripteur);

                char *date;
                effectueDatation(&date);                

                char *date_header = (char*) malloc((strlen("Date: ") + strlen(date) +2) * sizeof(char));
                strcpy(date_header, "Date: ");
                strcat(date_header, date);
                strcat(date_header, "\r\n");

                writeDirectClient(requete->clientId, date_header, strlen(date_header));
                free(date_header);


                char *content_length = (char*) malloc(100 * sizeof(char));
                sprintf(content_length, "%d", f_size);

                char* content_length_header = (char*) malloc((strlen("Content-Length: ") + strlen(content_length) +2) * sizeof(char));
                strcpy(content_length_header, "Content-Length: ");
                strcat(content_length_header, content_length); 
                strcat(content_length_header, "\r\n");

                writeDirectClient(requete->clientId, content_length_header, strlen(content_length_header));
                free(content_length);
                free(content_length_header);


                char *content_type_header = (char*) malloc((strlen("Content-Type: ") + strlen(f_type) +2) * sizeof(char));
                strcpy(content_type_header, "Content-Type: ");
                strcat(content_type_header, f_type);
                strcat(content_type_header, "\r\n");

                writeDirectClient(requete->clientId, content_type_header, strlen(content_type_header));
                free(content_type_header);


                char *connection = (flag_connection_keepalive ? "Connection: keep-alive\r\n" : "Connection: close\r\n");
                writeDirectClient(requete->clientId, connection, strlen(connection));


                writeDirectClient(requete->clientId, "\r\n", strlen("\r\n"));

                if (methode == GET) {
                // Ecriture / Envoie du body-message
                    writeDirectClient(requete->clientId, lecture, nb_lu);
                }

                free(lecture);

                break;
            }
        }
        else
        {
            writeDirectClient(requete->clientId, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n", strlen("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n"));
            requestShutdownSocket(requete->clientId);
        }

        // Verifie s'il y a au moins une erreur lors du traitement de la requete (si oui, une BAD RESPONSE aura ete envoye, pas la peine d'envoyer le buffer contenant la GOOD RESPONSE)
        if (!flag_err)
        {
            endWriteDirectClient(requete->clientId);
        }

        // Garde la connection ouverte selon la version HTTP (1.1 ici donc keepalive par defaut) et du champs Connection-header s'il est present (keepalive | close)
        if (!flag_connection_keepalive)
        {
            requestShutdownSocket(requete->clientId);
        }

        // on ne se sert plus de la requete courante a partir de maintenant, on peut donc liberer toutes les donnees correspondantes...
        freeRequest(requete);
        free(path_request_target);
        magic_close(file_magic);
    }

    return (1);
}

// Routines secondaires

/**
 * Routine pour envoyer un message d'erreur a l'emetteur si la requete contient des erreurs
 */
void sendBadReponse(char *buf, unsigned int clientId, struct sockaddr_in *clientAddress)
{
    message *reponse = (message *)malloc(sizeof(message));

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
void traiteMethode(_Token *t, Methode *m, message *requete, bool *flag_err)
{
    char* methode = getElementValue(t->node, NULL);
    methode = strtok(methode, " ");

    if (strcmp("GET", methode) == 0)
    {
        *m = GET;
    }
    else if (strcmp("HEAD", methode) == 0)
    {
        *m = HEAD;
    }
    else if (strcmp("POST", methode) == 0)
    {
        *m = POST;
    }
    else
    {
        sendBadReponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
        *flag_err = true;
    }
}

/**
 * Routine qui se charge de traiter le champs HTTP-Version de la requete
 */
void traiteHTTPVersion(_Token *t, message *requete, bool *flag_v, bool *flag_err)
{

    char *version = getElementValue(t->node, NULL);

    // Verifie que la majeure est bien 1 (si 0 ou 2 ou 3, version non supportee par le serveur)
    if (version[5] != '1')
    {
        sendBadReponse("HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Length: 0\r\nConnection: Keepalive\r\n\r\n", requete->clientId, requete->clientAddress);
        *flag_err = true;
        return;
    }
    // Verifie la mineure de l'emetteur : si en HTTP/1.0, on doit renvoyer du 1.0 et non du 1.1 !
    else if (version[7] == '0')
    {
        *flag_v = false;
    }
    // Sinon, on est assure d'etre en HTTP/1.n ou n >= 1, donc on traite comme si c'etait du 1.1 =)
}

/**
 * Routine se chargeant de traiter le Percent Coding de la request-target pour n'avoir in fine que des caracteres ASCII
 * @warning Modifie la taille de la request-target en place !
 */
char *effectuePercentCoding(char *msg, int *taille_msg, bool *flag_err)
{
    char *msg_final = (char *)malloc(*taille_msg * sizeof(char));

    for (int i = 0; i < *taille_msg; i++)
    {
        msg_final[i] = msg[i];
    }
    int taille_finale = *taille_msg;

// Parse l'integralite de la request-target a la recherche de caraceteres encodes avec des pourcents
    for (int index = 0; index < taille_finale; index++)
    {

        if (msg_final[index] == '%')
        {
            char nbr_hex[4];
            nbr_hex[0] = '0';
            nbr_hex[1] = 'x';

            if (index + 2 < taille_finale)
            { // S'assure que les 2 prochains caracteres sont disponibles (ils doivent etre compris entre 0 et 9 ou A et F)
                nbr_hex[2] = msg_final[index + 1];
                nbr_hex[3] = msg_final[index + 2];

                if ((nbr_hex[2] < 'A' || nbr_hex[2] > 'F') && (nbr_hex[2] < '0' || nbr_hex[2] > '9'))
                {
                    *flag_err = true;
                    break;
                }

                if ((nbr_hex[3] < 'A' || nbr_hex[3] > 'F') && (nbr_hex[3] < '0' || nbr_hex[3] > '9'))
                {
                    *flag_err = true;
                    break;
                }

                int nbr_int = (int)strtol(nbr_hex, NULL, 16);

                msg_final[index] = (char)nbr_int;   // Remplace le '%' par le caractere represente en hexa

            // Recule tous les elements qui suivent le code hexa de 2 places (car les 2 chiffres formant l'hexa disparaissent)
                for (int j = index +3; j < taille_finale; j++)
                {
                    msg_final[j - 2] = msg_final[j];
                }

                taille_finale = taille_finale - 2;
            }
            else
            { // Sinon c'est que la request-target ne veut rien dire !
                *flag_err = true;
            }
        }
    }
// S'il y a une erreur de levee, autant ne pas s'embeter plus et renvoyer le message erronne, enverra une 400 Bad Request ulterieurement
    if (*flag_err)
    {
        return msg;
    }
// Reforme le message final pour qu'il ne soit pas trop grand au vu des possibles diminutions faites dans le messages
    msg_final = (char*) realloc(msg_final, (taille_finale +1) * sizeof(char));
    msg_final[taille_finale] = '\0';    
    *taille_msg = taille_finale;

    return msg_final;
}

/**
 * Routine se chargeant de s'assurer qu'il n'y a aucune tentative de remonter a un repertoire parent dans la request-target
 * @warning Le Dot-Removal se charge ici de directement lever une erreur lorsqu'une tentative est reperee, ne re-ecrit pas la request-target SANS les ".."
 */
void effectueDotRemoval(char *chaine, int taille_chaine, bool *flag_err)
{
    // Parse la request-target dans son integralite
    for (int i = 0; i < taille_chaine; i++)
    {

        // Si un point est trouve, s'assure qu'il n'est pas suivi par un second point
        if (chaine[i] == '.')
        {
            if (i + 1 < taille_chaine && chaine[i + 1] == '.')
            { // Sinon fait remonter l'erreur pour provoquer un 400 Bad Request
                *flag_err = true;
            }
        }
    }
}

/**
 * Routine qui se charge d'obtenir la date via un appel syste
 * @warning DOIT ETRE ROOT POUR UTILISER CETTE ROUTINE
 */
void effectueDatationSystem(char **date)
{
    char path[] = "/var/www/date.txt";
    FILE *f_date;
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
void effectueDatation(char **date)
{
    *date = __DATE__;
}

void cookie_header_verif(_Token *node, _Token *root, bool* flag_err, char* path_target)
{
    // On récupère la valeur du cookie grace à la fonction getElementValue de l'api.
    char *cookie_string = getElementValue(node->node, NULL);
    if (cookie_string == NULL)
    {
        return;
    }

    // Check si le Cookie header apparait plus d'une fois. (MUSTNOT)
    int cookie_header_count = 0;
    _Token *current_node = node;
    while (current_node != NULL)
    {
        if (strcmp(getElementValue(current_node->node, NULL), "Cookie") == 0)
        {
            cookie_header_count++;
        }
        current_node = current_node->next;
    }

    if (cookie_header_count > 1)
    {
        *flag_err = true;
        return;
    }

    // On récupère les cookies stockés dans le magasin
    Cookie *cookies = get_cookies_from_store();
    char header_value[4096] = "";

    // On va vérifier si le cookie est valide.
    char *cookie_start = cookie_string;
    while (cookie_start)
    {
        char *cookie_end = strchr(cookie_start, ';');
        if (cookie_end)
        {
            *cookie_end = '\0';
        }

        Cookie *cookie = NULL;

        // On check tous les cookies du magasin.
        for (int i = 0; cookies[i].name != NULL; i++)
        {
            char *request_host = getElementValue(searchTree(root, "Host"), NULL);
            char *request_path = path_target;
            int is_secure = (strncmp(cookie_string, "https", 5) == 0);

            // Ajoute le nom du cookie, le caractère '=' et la valeur du cookie à la chaîne de cookies
            strcat(header_value, cookies[i].name);
            strcat(header_value, "=");
            strcat(header_value, cookies[i].value);

            // Vérifie la taille du cookie
            if (strlen(header_value) > 4096)
            {
                *flag_err = true;
                return;
            }

            // MUST
            if (cookie->secure)
            {
                // Ajoute l'attribut "Secure" à la liste des attributs du cookie
                strcat(header_value, "; Secure");
            }
            if (cookie->http_only_flag)
            {
                // Ajoute l'attribut "HttpOnly" à la liste des attributs du cookie
                strcat(header_value, "; HttpOnly");
            }
            // MUST PAth
            if (cookie->path)
            {
                // Si la valeur de l'attribut est vide ou si le premier caractère n'est pas '/',
                // utilise le chemin par défaut.
                if (cookie->path[0] == '\0' || cookie->path[0] != '/')
                {
                    cookie->path = default_path;
                }

                // Ajoute l'attribut "Path" à la liste des attributs du cookie
                strcat(header_value, "; Path=");
                strcat(header_value, cookie->path);
            }
            // MUST Domain
            if (cookie->domain)
            {
                // Si la valeur de l'attribut est vide, ignore le cookie
                if (cookie->domain[0] == '\0')
                {
                    continue;
                }

                // Si le premier caractère est '.', supprime le .
                if (cookie->domain[0] == '.')
                {
                    memmove(cookie->domain, cookie->domain + 1, strlen(cookie->domain));
                }

                // Converti le domaine en minuscules
                for (char *p = cookie->domain; *p; ++p)
                {
                    *p = tolower(*p);
                }

                // Ajoute l'attribut "Domain" à la liste des attributs du cookie
                strcat(header_value, "; Domain=");
                strcat(header_value, cookie->domain);
            }
            if (cookie->max_age)
            {
                // Si le premier caractère n'est pas un chiffre ou un '-', ignore le cookie
                if (!isdigit(cookie->max_age[0]) && cookie->max_age[0] != '-')
                {
                    continue;
                }

                // Si le reste de la valeur de l'attribut contient un caractère non numérique, ignore le cookie
                for (char *p = cookie->max_age + 1; *p; ++p)
                {
                    if (!isdigit(*p))
                    {
                        continue;
                    }
                }

                // Convertis la valeur de l'attribut en un entier
                int delta_seconds = atoi(cookie->max_age);

                // Si delta_seconds est inférieur ou égal à zéro, définis expiry_time sur la date et l'heure les plus anciennes possibles
                // Sinon, définis expiry_time sur la date et l'heure actuelles plus delta_seconds secondes
                time_t expiry_time = (delta_seconds <= 0 ? 0 : time(NULL) + delta_seconds);

                // Ajoute l'attribut "Max-Age" à la liste des attributs du cookie
                char max_age_str[20];
                sprintf(max_age_str, "; Max-Age=%ld", (long)expiry_time);
                strcat(header_value, max_age_str);
            }

            if ((cookie->host_only_flag && strcmp(request_host, cookie->domain) != 0) 
                || (!cookie->host_only_flag && !domain_matches(request_host, cookie->domain)) 
                || !path_matches(request_path, cookie->path) 
                || (cookie->secure && !is_secure) 
                || (cookie->http_only_flag && strncmp(cookie_string, "HTTP", 4) != 0))
            {
                continue;
            }

            // Vérifie si l'API est non-HTTP
            if (cookie->http_only_flag && strncmp(cookie_string, "HTTP", 4) != 0)
            {
                *flag_err = true;
                return;
            }

            cookie = &cookies[i];
            break;
        }

        if (!cookie)    // Ignore
        {
            return;
        }

        update_last_access_time(cookie);

        cookie_start = cookie_end ? cookie_end + 1 : NULL;
    }

    // printf("Cookie-Header correct\n");
}

Cookie *get_cookies_from_store()
{
    return cookie_store;
}

void update_last_access_time(Cookie *cookie)
{
    cookie->last_access_time = time(NULL);
}

bool domain_matches(const char *request_host, const char *cookie_domain)
{
    // On vérifie si le domaine de la requête est égal au domaine du cookie.
    return (strcmp(request_host, cookie_domain) == 0);
}

bool path_matches(const char *request_path, const char *cookie_path)
{
    // On vérifie si le path de la requête est égal au path du cookie.
    return strcmp(request_path, cookie_path) == 0;
}

void expect_header_verif(_Token *node, bool flag_v1_1, char *message_body)

// Si falg_v1_1 = True c'est du 1.1 sinon c'est 1.0.

{
    //char *expect_string = getElementValue(node->node, NULL);

    //if (strcmp(expect_string, "Expect: 100-continue") != 0)
    //{
        // printf("Expected: Expect: 100-continue\n");
        // printf("Received: %s\n", expect_string);
        //exit(1);
    //}

    // Si le corps du message a déjà été reçu en partie ou en totalité, omet d'envoyer une réponse 100 (Continue)
    //if (message_body != NULL && strlen(message_body) > 0)
    //{
        //return;
    //}

    // Sinon, envoie une réponse 100 (Continue)
    //sendReponse("HTTP/1.1 100 Continue\n");
}
