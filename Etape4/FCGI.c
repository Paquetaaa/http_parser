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
#include <stdbool.h>
#include "api.h"
#include "FCGI.h"

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

int main(void)
{

    // Initialisation de la socket
    int fd = createSocket("127.0.0.1", 9000);

    // Création de la requête FCGI_GET_VALUES
    Create_and_Send_GetValuesRequest(fd);

    // Création de la requête FCGI_BEGIN_REQUEST
    Create_and_Send_BeginRequest(fd, 1);

    // Création de la requête FCGI_ABORT_REQUEST

    Create_and_Send_AbortRequest(fd, 1);

    // Fermeture de la socket
    close(fd);
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
}

#define sendStdin(fd, id, stdin, len) sendWebData(fd, FCGI_STDIN, id, stdin, len)
#define sendData(fd, id, data, len) sendWebData(fd, FCGI_DATA, id, data, len)

// Converti les donnéees de notre requete en données FCGI_STDIN puis les ecrits dans la sockets.
void sendWebData(int fd, unsigned char type, unsigned short requestId, char *data, unsigned int len)
{
    FCGI_Header h;
    if (len > FASTCGILENGTH)
        return;

    h.version = FCGI_VERSION_1;
    h.type = type;
    h.requestId = htons(requestId);
    h.contentLength = len;
    h.paddingLength = 0;
    memcpy(h.contentData, data, len);
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));
}
