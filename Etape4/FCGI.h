#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "api.h"
#include "fastcgi.h"
#include "request.h"

typedef struct blabla {
    unsigned char tailleNomB0;
    unsigned char tailleDonneesB0;
    unsigned char* nom;
    unsigned char* donnees;
} nameValuePair;

// Given
static int createSocket(char *ip, int port);

// From others
int addNameValuePair(FCGI_Header *header, char *name, char *value);
void writeLen(int len, char **p);

// Ours
nameValuePair* ecrire_http_header(int* nb_http_headers);
nameValuePair* ecrire_fcgi_header(int* nb_fcgi_headers);
void createRequeteParams(int fd);
void createEmptyParams(int fd);
void Create_and_Send_AbortRequest(int fd, unsigned short requestId);
void Create_and_Send_BeginRequest(int fd, unsigned short requestId);
void Create_and_Send_GetValuesRequest(int fd);
void sendRequete();
void sendStdin(int fd, int id, char* data);
void sendWebData(int fd, unsigned char type, unsigned short requestId, char *data, unsigned int len);
char* lecture_reponse(int socket);
