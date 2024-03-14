#include <stdio.h>
#include <string.h>

typedef struct
{
    char method[20];
    char requestTarget[500];
    char host[500];
    char transferEncoding[500];
    char transferCoding[500];
} HttpRequest;

HttpRequest parse_http_request(const char *request)
{
    HttpRequest httpRequest;

    // Initialiser httpRequest
    memset(&httpRequest, 0, sizeof(HttpRequest));

    // Analyser la ligne de requête
    sscanf(request, "%s %s", httpRequest.method, httpRequest.requestTarget);

    // Rechercher et analyser les en-têtes
    char *headerStart = strstr(request, "Host:");
    if (headerStart != NULL)
    {
        sscanf(headerStart, "Host: %s", httpRequest.host);
    }

    headerStart = strstr(request, "Transfer-Encoding:");
    if (headerStart != NULL)
    {
        sscanf(headerStart, "Transfer-Encoding: %s", httpRequest.transferEncoding);
        char *codingStart = strstr(httpRequest.transferEncoding, ",");
        if (codingStart != NULL)
        {
            sscanf(codingStart, ",%s", httpRequest.transferCoding);
        }
    }

    return httpRequest;
}

void lire_et_analyser_requete(FILE *Fplecture)
{
    char requete[500];

    // Lire une ligne du fichier
    if (fgets(requete, sizeof(requete), Fplecture) == NULL)
    {
        printf("Erreur lors de la lecture du fichier\n");
        return;
    }

    // Analyser la requête
    HttpRequest httpRequest = parse_http_request(requete);

    // Afficher les résultats
    printf("Method: %s\n", httpRequest.method);
    printf("Request Target: %s\n", httpRequest.requestTarget);
    printf("Host: %s\n", httpRequest.host);
    printf("Transfer-Encoding: %s\n", httpRequest.transferEncoding);
    printf("Transfer-Coding: %s\n", httpRequest.transferCoding);
}

int main()
{
    FILE *Fplecture = fopen("test0.txt", "r");
    if (Fplecture == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return 1;
    }

    lire_et_analyser_requete(Fplecture);

    fclose(Fplecture);

    return 0;
}