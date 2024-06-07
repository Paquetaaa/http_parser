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
    unsigned char tailleNomB0;          // If tailleNomB0 >> 7 == 1 : encoded on 4 bytes, on 1 byte otherwise 
    unsigned char tailleDonneesB0;      // Same : tailleDonneesB0 >> 7 == 1 ? encoded on 4 bytes : encoded on 1 byte
    unsigned char* nom;                 // length(nom) = tailleNomB0
    unsigned char* donnees;             // length(donnees) == tailleDonneesB0
} nameValuePair;                        // Use of uint8_t necessary ??

// Given
static int createSocket(char *ip, int port);

// From others
void Create_and_Send_AbortRequest(int fd, unsigned short requestId);
void Create_and_Send_BeginRequest(int fd, unsigned short requestId);
void Create_and_Send_GetValuesRequest(int fd);
void sendStdin(int fd, int id, char* data);
void sendWebData(int fd, unsigned char type, unsigned short requestId, char *data, unsigned int len);
size_t readSocket(int fd, char *buf, size_t len);
void readData(int fd, FCGI_Header *h, size_t *len);
int addNameValuePair(FCGI_Header *header, char *name, char *value);
void writeSocket(int fd, FCGI_Header *h, unsigned int len);
char* lecture_reponse(int socket);
void sendRequete();

// Ours
nameValuePair* ecrire_http_header(int* nb_http_headers);
nameValuePair* ecrire_fcgi_header(int* nb_fcgi_headers);
void createRequeteParams(int fd);
void createEmptyParams(int fd);
bool intToBytes(int taille, char *o);