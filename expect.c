#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"
#include "api.h"
#include <time.h>
#include <stdbool.h>

void expect_header_verif(_Token *node, bool flag_v1_1, char *message_body);

void expect_header_verif(_Token *node, bool flag_v1_1, char *message_body)

// Si falg_v1 = TRue c'est du 1.1 sinon c'est 1.0.

{
    char *expect_string = getElementValue(node->node, NULL);

    if (strcmp(expect_string, "Expect: 100-continue") != 0)
    {
        // printf("Expected: Expect: 100-continue\n");
        // printf("Received: %s\n", expect_string);
        exit(1);
    }

    // Si le corps du message a déjà été reçu en partie ou en totalité, omet d'envoyer une réponse 100 (Continue)
    if (message_body != NULL && strlen(message_body) > 0)
    {
        return;
    }

    // Sinon, envoie une réponse 100 (Continue)
    sendReponse("HTTP/1.1 100 Continue\n");
}
