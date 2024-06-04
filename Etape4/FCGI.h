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

void Create_and_Send_BeginRequest(int fd, unsigned short requestId);
void Create_and_Send_AbortRequest(int fd, unsigned short requestId);
