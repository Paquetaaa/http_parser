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
#include <stdbool.h>
#include "api.h"

void Create_and_Send_AbortRequest(int fd, unsigned short requestId);
void Create_and_Send_BeginRequest(int fd, unsigned short requestId);
void Create_and_Send_GetValuesRequest(int fd);
void sendWebData(int fd, unsigned char type, unsigned short requestId, char *data, unsigned int len);
size_t readSocket(int fd, char *buf, size_t len);
void readData(int fd, FCGI_Header *h, size_t *len);
int addNameValuePair(FCGI_Header *header, char *name, char *value);
void writeSocket(int fd, FCGI_Header *h, unsigned int len);
void writeLen(int len, char **p);


char* ecrire_http_header();
char* ecrire_fcgi_header();
static FCGI_Header* createRequeteParams(int fd);
static FCGI_Header* createEmptyParams(int fd);