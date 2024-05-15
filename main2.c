#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "request.h"

#define MAX_BUFFER_SIZE 1024

// Fonction pour déterminer le type de contenu en fonction du fichier
const char *get_content_type(const char *file_path)
{
    if (strstr(file_path, ".html"))
    {
        return "text/html";
    }
    else if (strstr(file_path, ".css"))
    {
        return "text/css";
    }
    else if (strstr(file_path, ".js"))
    {
        return "application/javascript";
    }
    else if (strstr(file_path, ".jpg") || strstr(file_path, ".jpeg"))
    {
        return "image/jpeg";
    }
    else if (strstr(file_path, ".png"))
    {
        return "image/png";
    }
    else
    {
        return "text/plain";
    }
}

// Fonction pour envoyer une réponse au client
void send_response(message *msg, int status_code, const char *status_text, const char *content_type, const char *body)
{
    char response[MAX_BUFFER_SIZE];
    int body_length = strlen(body);

    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status_code, status_text, content_type, body_length, body);

    writeDirectClient(msg, response, strlen(response));
    endWriteDirectClient(msg);
}

// Fonction pour gérer les requêtes GET
void handle_get_request(message *msg, const char *request_uri)
{
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "www%s", request_uri);

    FILE *file = fopen(file_path, "rb");
    if (file == NULL)
    {
        send_response(msg, 404, "Not Found", "text/plain", "File not found");
        return;
    }

    // Lire le contenu du fichier
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = malloc(file_size);
    fread(file_content, 1, file_size, file);
    fclose(file);

    // Envoyer la réponse
    send_response(msg, 200, "OK", get_content_type(file_path), file_content);
    free(file_content);
}

int main()
{
    message *msg;
    int ret;

    // Initialisation de la bibliothèque
    ret = initRequest();
    if (ret < 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la bibliothèque request\n");
        return 1;
    }

    while (1)
    {
        // Réception des requêtes
        msg = getRequest(80);
        if (msg == NULL)
        {
            continue;
        }

        // Analyse de la requête HTTP
        char *method = strtok(msg->buf, " ");
        char *request_uri = strtok(NULL, " ");
        char *http_version = strtok(NULL, "\r\n");

        if (method == NULL || request_uri == NULL || http_version == NULL)
        {
            send_response(msg, 400, "Bad Request", "text/plain", "Invalid request");
            freeRequest(msg);
            continue;
        }

        // Traitement des requêtes GET
        if (strcmp(method, "GET") == 0)
        {
            handle_get_request(msg, request_uri);
        }
        else
        {
            send_response(msg, 501, "Not Implemented", "text/plain", "Method not supported");
        }

        // Fermeture de la connexion
        requestShutdownSocket(msg);
        freeRequest(msg);
    }

    return 0;
}
