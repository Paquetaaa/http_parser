#include "FCGI.h"
#define PORT 9000
#define IP "127.0.0.1"

// Création de la socket
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

// Lit les données de la socket
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

// Lit les données de la socket
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

// Ajoute un nom et une valeur à la requête FCGI.
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

// Ecrit la taille de la requête
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

// On ecrit dans la socket
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

// Crée une requête de type FCGI_GET_VALUES
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
    free(header);
}

// Crée une requête de type FCGI_BEGIN_REQUEST. Donc on envoie le header et le body.
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
    free(header);
    free(begin);
}

// Crée une requête de type FCGI_ABORT_REQUEST.
void Create_and_Send_AbortRequest(int fd, unsigned short requestId)
{
    FCGI_Header *header = malloc(sizeof(FCGI_Header));

    header->version = FCGI_VERSION_1;
    header->type = FCGI_ABORT_REQUEST;
    header->requestId = htons(requestId);
    header->contentLength = 0;
    header->paddingLength = 0;
    writeSocket(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    free(header);
}

#define sendStdin(fd, id, stdin, len) sendWebData(fd, FCGI_STDIN, id, stdin, len)
#define sendData(fd, id, data, len) sendWebData(fd, FCGI_DATA, id, data, len)

// Converti les donnéees de notre requete en données FCGI_STDIN puis les ecrits dans la sockets.
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

/*
Pour params, la bonne moitié faut juste lire les champs "header-fields" dans l'arbre et les balancer
pour le reste, on les genère en regardant la ou il faut

La fonction pour créer les params et les écrire dans la socket : static char* createRequeteParams()

*/

nameValuePair* ecrire_http_header()
{
    void *root = getRootTree();
    _Token *token = searchTree(root, "header_field");
    token = token->next;
    char *header_http = NULL;
    nameValuePair* reponse = malloc(sizeof(nameValuePair)*10);
    int i = 0;
    // token -> node c'est le contenu, -> next c'est l'élement d'après
    while (token != NULL)
    {
        nameValuePair *h = malloc(sizeof(nameValuePair));
        char *reponse = "HTTP_";
        char *value = getElementValue(token->node, NULL);
        char *tag = getElementTag(token->node, NULL);
        strcat(reponse,tag);
        h->tailleNom = strlen(reponse);
        h->tailleDonnees = strlen(value);
        h->nom = tag;
        h->donnees = value;

        token = token->next;
        reponse[i] = h;
        i++;
    }

    reponse = realloc(reponse,i*sizeof(nameValuePair));
    return reponse;
}

nameValuePair *ecrire_fcgi_header()
{
    nameValuePair reponse[4];
    char *server_name = "127.0.0.1";
    char *server_addr = "127.0.0.1";
    char *server_port = "80";
    char *document_root = "/var/www/html";

    void *root = getRootTree();
    _Token *token = searchTree(root, "request_target");
    char *requestURI = getElementValue(token->node, NULL);
    nameValuePair h3;
    _Token *token3 = searchTree(root, "query");
    if(token3 != NULL){
        char *queryString = getElementValue(token3->node, NULL);
        h3.tailleNom = strlen("QUERY_STRING");
        h3.tailleDonnees = strlen(queryString);
        memcpy(h3.nom,"QUERY_STRING",h3.tailleNom);
        h3.donnees = queryString;
    }
    else{
        h3.tailleNom = strlen("QUERY_STRING");
        h3.tailleDonnees =0;
        memcpy(h3.nom,"QUERY_STRING",h3.tailleNom);
        h3.donnees = NULL;
    }

    _Token *token2 = searchTree(root,"method");
    char* methode = getElementValue(token2->node, NULL);
    methode = strtok(methode, " ");
    
    char *current;
    strcpy(current, requestURI);
    char *next = strtok(current, "/");
    while (next != NULL)
    {
        current = next;
        next = strtok(NULL, "/");
    }
    char *scriptname;
    strcpy(scriptname, current);

    char *script_filename = "proxy:fcgi://";
    char *port = "9000/";

    nameValuePair h1,h2,h4;

    strcat(script_filename, port);
    strcat(script_filename, document_root);
    strcat(script_filename, scriptname);
    strcat(reponse, script_filename);

    h1.tailleNom = strlen("SCRIPT_FILENAME");
    h1.tailleDonnees = strlen(reponse);
    memcpy(h1.nom,"SCRIPT_FILENAME",h1.tailleNom);
    h1.donnees = reponse;

    h2.tailleNom = strlen("REQUEST_METHOD");
    h2.tailleDonnees = strlen(methode);
    memcpy(h2.nom,"REQUEST_METHOD",h2.tailleNom);
    h2.donnees = methode;

    h4.tailleNom = strlen("REQUEST_URI");
    h4.tailleDonnees = strlen(requestURI);
    memcpy(h4.nom,"REQUEST_URI",h4.tailleNom);
    h4.donnees = requestURI;

    reponse[0] = h1;
    reponse[1] = h2;
    reponse[2] = h3;
    reponse[3] = h4;

    return reponse;
}


char* en_tetes;

void createRequeteParams(int fd)
{
    unsigned short requestId = 1;
    FCGI_Header *header = malloc(sizeof(FCGI_Header));

    header->type = FCGI_PARAMS;
    header->version = FCGI_VERSION_1;
    header->requestId = requestId;

    nameValuePair *http_headers = ecrire_http_header();
    nameValuePair *fcgi_headers = ecrire_fcgi_header(); 

    int taille_depart = sizeof(http_headers);
    memcpy(en_tetes,http_headers,sizeof(http_headers));
    memcpy(en_tetes+taille_depart,fcgi_headers,sizeof(fcgi_headers));

    strcpy(header->contentData, htons(en_tetes));

    write(fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    
    free(http_headers);
    free(fcgi_headers);

    free(header);
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

    free(header);
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
        free(content);
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
