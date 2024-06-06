#include "FCGI.h"
#define PORT 9000
#define IP "127.0.0.1"

// Création de la socket (routine fournie)
static int createSocket(char *ip, int port)
{
    int fd;
    struct sockaddr_in serv_addr;
    int enable = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed\n");
        return (-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    inet_aton(ip, (struct sockaddr *)&serv_addr.sin_addr.s_addr);
    serv_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect failed\n");
        return (-1);
    }

    return fd;
}

// Lit les données de la socket (from others)
size_t readSocket(int fd, char *buf, size_t len)
{
    size_t readlen = 0;
    ssize_t nb = 0;

    if (len == 0)
        return 0;

    do
    {
        // try to read
        do
        {
            nb = read(fd, buf + readlen, len - readlen);
        } while (nb == -1 && errno == EINTR);
        if (nb > 0)
            readlen += nb;
    } while ((nb > 0) && (len != readlen));

    if (nb < 0)
        readlen = -1;
    return readlen;
}

// Lit les données de la socket (from others)
void readData(int fd, FCGI_Header *h, size_t *len)
{
    size_t nb;
    *len = 0;

    nb = sizeof(FCGI_Header) - FASTCGILENGTH;
    if ((readSocket(fd, (char *)h, nb) == nb))
    {
        h->requestId = htons(h->requestId);

        h->contentLength = htons(h->contentLength);
        *len += nb;
        nb = h->contentLength + h->paddingLength;

        if ((readSocket(fd, (char *)h->contentData, nb) == nb))
        {
            *len += nb;
        }
        else
        {
            *len = 0;
        }
    }
}

// Ajoute un nom et une valeur à la requête FCGI (from others)
int addNameValuePair(FCGI_Header *header, char *name, char *value)
{
    char *p;
    unsigned int nameLen = 0, valueLen = 0;
    // Calcul de la taille de name + value
    if (name)
        nameLen = strlen(name);
    if (value)
        valueLen = strlen(value);

    if (valueLen > FASTCGIMAXNVPAIR)
        return -1;
    if ((header->contentLength + ((nameLen > 0x7F) ? 4 : 1) + ((valueLen > 0x7F) ? 4 : 1)) > FASTCGILENGTH)
        return -1;

    p = (header->contentData) + header->contentLength;
    writeLen(nameLen, &p);
    writeLen(valueLen, &p);
    strncpy(p, name, nameLen);
    p += nameLen;
    if (value)
        strncpy(p, value, valueLen);
    header->contentLength += nameLen + ((nameLen > 0x7F) ? 4 : 1);
    header->contentLength += valueLen + ((valueLen > 0x7F) ? 4 : 1);

    return 0;
}

// Ecrit la taille de la requête (from others)
void writeLen(int len, char **p)
{
    if (len > 0x7F)
    {
        *((*p)++) = (len >> 24) & 0x7F;
        *((*p)++) = (len >> 16) & 0xFF;
        *((*p)++) = (len >> 8) & 0xFF;
        *((*p)++) = (len) & 0xFF;
    }
    else
        *((*p)++) = (len) & 0x7F;
}

// On ecrit dans la socket (from others)
void writeSocket(int fd, FCGI_Header *h, unsigned int len)
{
    int w;

    // On converti les données en network byte order avec htons
    h->contentLength = htons(h->contentLength);
    h->paddingLength = htons(h->paddingLength);

    while (len > 0)
    {
        do
        {
            w = write(fd, h, len);
        } while (w == -1 && errno == EINTR);
        len -= w;
    }
}

// Crée une requête de type FCGI_GET_VALUES (from others)
void Create_and_Send_GetValuesRequest(int fd)
{
    FCGI_Header *header = malloc(sizeof(FCGI_Header));

    header->version = FCGI_VERSION_1;
    header->type = FCGI_GET_VALUES;
    header->requestId = htons(FCGI_NULL_REQUEST_ID);

    header->contentLength = 0;
    header->paddingLength = 0;

    addNameValuePair(header, FCGI_MAX_CONNS, NULL);
    addNameValuePair(header, FCGI_MAX_REQS, NULL);
    addNameValuePair(header, FCGI_MPXS_CONNS, NULL);
    writeSocket(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    //free(header);
}

// Crée une requête de type FCGI_BEGIN_REQUEST. Donc on envoie le header et le body (from others)
void Create_and_Send_BeginRequest(int fd, unsigned short requestId)
{
    FCGI_Header *header = malloc(sizeof(FCGI_Header));
    FCGI_BeginRequestBody *begin = malloc(sizeof(FCGI_BeginRequestBody));

    header->version = FCGI_VERSION_1;
    header->type = FCGI_BEGIN_REQUEST;
    header->requestId = htons(requestId);
    header->contentLength = sizeof(FCGI_BeginRequestBody);
    header->paddingLength = 0; // Pas besoin de padding.

    begin = (FCGI_BeginRequestBody *)&(header->contentData); // On récupère le contenu du header

    begin->role = FCGI_RESPONDER; // On attend une réponse du serveur
    begin->flags = 0;             // Pas besoin de garder la connexion ouverte avec le serveur.

    writeSocket(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    //free(header);
    //free(begin);
}

// Crée une requête de type FCGI_ABORT_REQUEST (from others)
void Create_and_Send_AbortRequest(int fd, unsigned short requestId)
{
    FCGI_Header *header = malloc(sizeof(FCGI_Header));

    header->version = FCGI_VERSION_1;
    header->type = FCGI_ABORT_REQUEST;
    header->requestId = htons(requestId);
    header->contentLength = 0;
    header->paddingLength = 0;
    writeSocket(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    //free(header);
}

#define sendStdin(fd, id, stdin, len) sendWebData(fd, FCGI_STDIN, id, stdin, len)
#define sendData(fd, id, data, len) sendWebData(fd, FCGI_DATA, id, data, len)

// Converti les donnéees de notre requete en données FCGI_STDIN puis les ecrits dans la sockets (from others)
void sendWebData(int fd, unsigned char type, unsigned short requestId, char *data, unsigned int len)
{
    FCGI_Header *header = malloc(sizeof(FCGI_Header));
    if (len > FASTCGILENGTH) {
        return;
    }
    header->version = FCGI_VERSION_1;
    header->type = type;
    header->requestId = htons(requestId);
    header->contentLength = len;
    header->paddingLength = 0;
    //Ecriture DATA
    memcpy(header->contentData, data, len);
    //Ecriture Socket
    writeSocket(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
}

/**
 * Pour params, la bonne moitié faut juste lire les champs "header-fields" dans l'arbre et les balancer
 * pour le reste, on les genère en regardant la ou il faut
 *
 * La fonction pour créer les params et les écrire dans la socket : static char* createRequeteParams()
 *
 * @warning La sortie est un nameValuePair** !
*/
nameValuePair** ecrire_http_header()
{
    void *root = getRootTree();
    _Token *token = searchTree(root, "header_field");

    token = token->next; // Permet de sauter la start-line

// REMARQUE : les variables "*2" ne sont utiles que pour gerer le cas pathologique ou le premier token prend 2 header-fields a la fois
    nameValuePair** headers_http = (nameValuePair**) malloc(sizeof(nameValuePair*)*10);  // On considere avoir au plus 10 en-tetes HTTP
    nameValuePair* h1 = (nameValuePair*) malloc(sizeof(nameValuePair));
    nameValuePair* h2 = (nameValuePair*) malloc(sizeof(nameValuePair));

    int tailleNom1 = 0, tailleNom2 = 0;
    int tailleData1 = 0, tailleData2 = 0;
    
    int i = 0;  // Compteur du nombre de header

    if (token != NULL) {    // Gere le cas ou le premier token prend 2 header-fields en meme temps !

    // WARNING : le token prend 2 header-fields a la fois !!
        int taille;
        char *header_complet = getElementValue(token->node, &taille);    // /!\ CONTIENT 2 HEADER_FIELDS EN MEME TEMPS !!

        char* header1 = strtok(header_complet, "\r\n");     // Obtient le premier header_field  
        char* header2 = strtok(NULL, "\r\n");               // Obtient le second header_field

    // Rajoute les sentinelles de fin manquantes aux 2 header_fields separes
        strcat(header1, "\0");                              
        strcat(header2, "\0");

        char* tag1 = strtok(header1, ":");   // Obtient le nom (tag) du header1
        strcat(tag1, "\0");
        char* value1 = strtok(NULL, ":");    // Obtient la valeur du header1

        value1 += 1;                         // Supprime l'espace debutant la valeur

        char* tag2 = strtok(header2, ":");   // Obtient le nom (tag) du header2
        strcat(tag2, "\0");
        char* value2 = strtok(NULL, ":");    // Obtient la valeur du header2

        value2 += 1;                         // Supprime l'espace debutant la valeur

    // Ajustement des tailles de chaque tags/valeurs
        tailleNom1 = strlen(tag1);
        tailleNom2 = strlen(tag2);
        tailleData1 = strlen(value1);
        tailleData2 = strlen(value2);


    // Tags contenant "HTTP_" au debut, necessaire pour le serveur PHP
        char *tag1_complet = malloc((tailleNom1 + strlen("HTTP_")) * sizeof(char));
        char *tag2_complet = malloc((tailleNom2 + strlen("HTTP_")) * sizeof(char));

        sprintf(tag1_complet, "%s", "HTTP_");
        strcat(tag1_complet, tag1);

        sprintf(tag2_complet, "%s", "HTTP_");
        strcat(tag2_complet, tag2);

    // 
        char* valeur1_def = malloc(tailleData1 * sizeof(char));
        char* valeur2_def = malloc(tailleData2 * sizeof(char));

        strcpy(valeur1_def, value1);
        strcpy(valeur2_def, value2);

    // Placement des champs avec les var allouees sur le tas
        h1->tailleNomB0 = tailleNom1 + strlen("HTTP_");
        h1->tailleDonneesB0 = tailleData1;

        h1->nom = tag1_complet;
        h1->donnees = valeur1_def;

        h2->tailleNomB0 = tailleNom2 + strlen("HTTP_");
        h2->tailleDonneesB0 = tailleData2;

        h2->nom = tag2_complet;
        h2->donnees = valeur2_def;

        headers_http[i] = h1;

        i++;
        headers_http[i] = h2;

        i++;
        token = token->next;
    }

    // token ->node c'est le contenu, ->next c'est l'élement d'après
    while (token != NULL)
    {
        nameValuePair* h1 = (nameValuePair*) malloc(sizeof(nameValuePair));
        int tailleNom1 = 0;
        int tailleData1 = 0;

    // WARNING : le token prend 2 header-fields a la fois !!
        char *header_complet = getElementValue(token->node, NULL);    // /!\ CONTIENT 2 HEADER_FIELDS EN MEME TEMPS !!

        char* header1 = strtok(header_complet, "\r\n");     // Obtient le premier header_field  

    // Rajoute les sentinelles de fin manquantes aux 2 header_fields separes
        strcat(header1, "\0");                              

        char* tag1 = strtok(header1, ":");   // Obtient le nom (tag) du header1
        strcat(tag1, "\0");
        char* value1 = strtok(NULL, ":");    // Obtient la valeur du header1

        value1 += 1;                         // Supprime l'espace debutant la valeur

    // Ajustement des tailles de chaque tags/valeurs
        tailleNom1 = strlen(tag1);
        tailleData1 = strlen(value1);

    // Tags contenant "HTTP_" au debut, necessaire pour le serveur PHP
        char *tag1_complet = malloc((tailleNom1 + strlen("HTTP_")) * sizeof(char));

        sprintf(tag1_complet, "%s", "HTTP_");
        strcat(tag1_complet, tag1);

    // 
        char* valeur1_def = malloc(tailleData1 * sizeof(char));

        strcpy(valeur1_def, value1);

    // Placement des champs avec les var allouees sur le tas
        h1->tailleNomB0 = tailleNom1 + strlen("HTTP_");
        h1->tailleDonneesB0 = tailleData1;

        h1->nom = tag1_complet;
        h1->donnees = valeur1_def;

        headers_http[i] = h1;

        i++;
        token = token->next;
    }
        
    return headers_http;
}

nameValuePair** ecrire_fcgi_header()
{
    nameValuePair* reponse[4];  // On ne rentre que 4 FCGI_headers (REQUEST_METHOD; SCRIPT_NAME; QUERY STRING; SCRIPT_FILENAME)
    char *server_name = "127.0.0.1";
    char *server_addr = "127.0.0.1";
    char *server_port = "80";
    char *document_root = "/var/www/html";
    char *script_filename_prefixe = "proxy:fcgi://";
    char *port = "9000/";

    nameValuePair* h1 = (nameValuePair*) malloc(sizeof(nameValuePair));
    nameValuePair* h2 = (nameValuePair*) malloc(sizeof(nameValuePair));
    nameValuePair* h3 = (nameValuePair*) malloc(sizeof(nameValuePair));
    nameValuePair* h4 = (nameValuePair*) malloc(sizeof(nameValuePair));

    void *root = getRootTree();

    _Token *token_method = searchTree(root, "method");
    _Token *token_request_target = searchTree(root, "request_target");
    _Token *token_query = searchTree(root, "query");

    char* methode = getElementValue(token_method->node, NULL);
    methode = strtok(methode, " ");
    strcat(methode, "\0");

    char *requestURI = getElementValue(token_request_target->node, NULL);
    requestURI = strtok(requestURI, " ");
    strcat(requestURI, "\0");

// La query-string peut etre nulle (ommise dans un GET par exemple)
    if(token_query != NULL){
        char *queryString = getElementValue(token_query->node, NULL);

        h3->tailleNomB0 = strlen("QUERY_STRING");
        h3->tailleDonneesB0 = strlen(queryString);

        h3->nom = malloc(h3->tailleNomB0 * sizeof(char));
        h3->donnees = malloc(h3->tailleDonneesB0 * sizeof(char));

        memcpy(h3->nom,"QUERY_STRING",h3->tailleNomB0);
        memcpy(h3->donnees, queryString, h3->tailleDonneesB0);
    }
    else{
        h3->tailleNomB0 = strlen("QUERY_STRING");
        h3->tailleDonneesB0 = 0;

        h3->nom = malloc(h3->tailleNomB0 * sizeof(char));
        h3->donnees = malloc(sizeof(char));

        memcpy(h3->nom, "QUERY_STRING", h3->tailleNomB0);
        memcpy(h3->donnees, "", 0);
    }
    
// Cherche le SCRIPT_FILENAME en decomposant la request-target (va de /.../.../ jusqu'a ce que le prochain token soit nul)
    char *current = malloc(strlen(requestURI)+1);

    strcpy(current, requestURI);   // Le premier appel a strtok(requestURI, " ") a donne la methode, toute la request-target est donc obtenue au second appel

    char *next = strtok(current, "/");   // Decompose la request-target selon les "/"

    while (next != NULL)
    {
        current = next;
        next = strtok(NULL, "/");
    }

// A la fin de la boucle, on obtient le SCRIPT_FILENAME "nom.extension"
// Necessaire de remettre "/" au debut !
    char *scriptname = malloc(strlen(current)+2);

    sprintf(scriptname, "%s%s", "/", current);

// Construit le SCRIPT_FILENAME a l'aide de toutes les variables utiles
    char* script_filename_def = (char*) malloc((strlen(script_filename_prefixe) + strlen(port) + strlen(document_root) + strlen(scriptname) +1) * sizeof(char));

    sprintf(script_filename_def, "%s", script_filename_prefixe);
    strcat(script_filename_def, port);
    strcat(script_filename_def, document_root);
    strcat(script_filename_def, scriptname);

// nameValue pair about script filename
    h1->tailleNomB0 = strlen("SCRIPT_FILENAME");
    h1->tailleDonneesB0 = strlen(script_filename_def);

    h1->nom = malloc(h1->tailleNomB0 * sizeof(char));
    h1->donnees = malloc(h1->tailleDonneesB0 * sizeof(char));

    memcpy(h1->nom, "SCRIPT_FILENAME", h1->tailleNomB0);
    memcpy(h1->donnees, script_filename_def, h1->tailleDonneesB0);

// nameValue pair about request method
    h2->tailleNomB0 = strlen("REQUEST_METHOD");
    h2->tailleDonneesB0 = strlen(methode);

    h2->nom = malloc(h2->tailleNomB0 * sizeof(char));
    h2->donnees = malloc(h2->tailleDonneesB0 * sizeof(char));

    memcpy(h2->nom, "REQUEST_METHOD", h2->tailleNomB0);
    memcpy(h2->donnees, methode, h2->tailleDonneesB0);

// nameValue pair about requestURI
    h4->tailleNomB0 = strlen("REQUEST_URI");
    h4->tailleDonneesB0 = strlen(requestURI);

    h4->nom = malloc(h4->tailleNomB0 * sizeof(char));
    h4->donnees = malloc(h4->tailleDonneesB0 * sizeof(char));

    memcpy(h4->nom, "REQUEST_URI", h4->tailleNomB0);
    memcpy(h4->donnees, requestURI, h4->tailleDonneesB0);

    reponse[0] = h1;
    reponse[1] = h2;
    reponse[2] = h3;
    reponse[3] = h4;

    return reponse;
}




void createRequeteParams(int fd)
{   
    unsigned short requestId = 1;
    FCGI_Header *header = malloc(sizeof(FCGI_Header));
    header->type = FCGI_PARAMS;
    header->version = FCGI_VERSION_1;
    header->requestId = htons(requestId);
    header->paddingLength = 0;
    header->reserved = 0;

    
    nameValuePair *http_headers = ecrire_http_header();
    nameValuePair *fcgi_headers = ecrire_fcgi_header(); 

    int taille_depart = sizeof(http_headers);
    int taille_arrivee = sizeof(fcgi_headers);

    unsigned char buffer_entete[FASTCGILENGTH];
    
    memcpy(buffer_entete,http_headers,taille_depart);
    sprintf(buffer_entete+taille_depart,"%s",fcgi_headers);
  //  memcpy(en_tetes+taille_depart,fcgi_headers,taille_arrivee);

    header->contentLength = sizeof(buffer_entete);
    //memcpy(header->contentData,htons(en_tetes),header->contentLength);
  //  sprintf(header->contentData,"%d",htons(en_tetes));

    //write(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    
    //memcpy(fd,&header,sizeof(header)+1);
    //memcpy(fd,buffer_entete,sizeof(buffer_entete));
    write(fd, header, sizeof(header));
    write(fd, buffer_entete,FASTCGILENGTH);

    //free(http_headers);
    //free(fcgi_headers);

    //free(header);
}

void createEmptyParams(int fd)
{
    unsigned short requestId = 1;
    FCGI_Header *header = malloc(sizeof(FCGI_Header));

    header->type = FCGI_PARAMS;
    header->version = FCGI_VERSION_1;
    header->requestId = requestId;

    header->contentLength = 0;
    header->paddingLength = 0;
    header->reserved = 0;

    write(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));

    //free(header);
}

char* lecture_reponse(int socket)
{
    char* reponse = malloc(16*FASTCGILENGTH);
    int taille;
    FCGI_Header *out = malloc(sizeof(FCGI_Header)*16);
    int i = 0;
    while(read(socket,&out[i],sizeof(out[i])) > 0)
    {
        taille = ntohs(out[i].contentLength);
        if(out[i].type == FCGI_END_REQUEST)
        {
            printf("End request atteint c'est finito \n");
            printf("valeur des champs = appstatus = %d, protocolstatus = %d \n");
            break;
        }
        char *content = malloc(sizeof(char)*taille);
        read(socket,content,taille);
        if(out[i].type == FCGI_STDOUT)
        {
            printf("c'est un stdout : %s\n",content);
        }
        strcat(reponse,content);
        i++;
        if(i == 16)
        {
            break;
        }
        //free(content);
    }
    reponse = realloc(reponse,strlen(reponse));
    return reponse;
}


void sendRequete()
{
    int socket = createSocket(IP, PORT);
    
    int requestID = 1;

    Create_and_Send_BeginRequest(socket, requestID);
    createRequeteParams(socket);
    createEmptyParams(socket);
    sendStdin(socket, requestID,"", strlen(""));

    char* reponse = lecture_reponse(socket);
    printf("reponse = %s \n",reponse);
    
    close(socket); // Fermeture de la socket apres chaque requete
}


int main(void)
{
    message *requete;
    
    if ((requete = getRequest(8080)) == NULL)
            return -1; // Il faudra penser a faire tourner le code sur le port 80 (HTTP)
    printf("#########################################\nDemande recue depuis le client %d\n", requete->clientId);
    printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
    printf("Contenu de la demande %.*s\n\n", requete->len, requete->buf);
    
    
    if (parseur(requete->buf, requete->len) == 1)
    {
        sendRequete();
    }
}
