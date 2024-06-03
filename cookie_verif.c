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

/*The user agent includes stored cookies in the Cookie HTTP request
   header.

   When the user agent generates an HTTP request, the user agent MUST
   NOT attach more than one Cookie header field.

   If the user agent does attach a Cookie header field to an HTTP
   request, the user agent MUST send the cookie-string (defined below)
   as the value of the header field.

   The user agent MUST use an algorithm equivalent to the following
   algorithm to compute the "cookie-string" from a cookie store and a
   request-uri:

   1.  Let cookie-list be the set of cookies from the cookie store that
       meets all of the following requirements:

       *  Either:

             The cookie's host-only-flag is true and the canonicalized
             request-host is identical to the cookie's domain.

          Or:

             The cookie's host-only-flag is false and the canonicalized
             request-host domain-matches the cookie's domain.

       *  The request-uri's path path-matches the cookie's path.

       *  If the cookie's secure-only-flag is true, then the request-
          uri's scheme must denote a "secure" protocol (as defined by
          the user agent).

             NOTE: The notion of a "secure" protocol is not defined by
             this document.  Typically, user agents consider a protocol
             secure if the protocol makes use of transport-layer
             security, such as SSL or TLS.  For example, most user
             agents consider "https" to be a scheme that denotes a
             secure protocol.

       *  If the cookie's http-only-flag is true, then exclude the
          cookie if the cookie-string is being generated for a "non-
          HTTP" API (as defined by the user agent).

   3.  Update the last-access-time of each cookie in the cookie-list to
       the current date and time.

   4.  Serialize the cookie-list into a cookie-string by processing each
       cookie in the cookie-list in order:

       1.  Output the cookie's name, the %x3D ("=") character, and the
           cookie's value.

       2.  If there is an unprocessed cookie in the cookie-list, output
           the characters %x3B and %x20 ("; ").

   NOTE: Despite its name, the cookie-string is actually a sequence of
   octets, not a sequence of characters.  To convert the cookie-string
   (or components thereof) into a sequence of characters (e.g., for
   presentation to the user), the user agent might wish to try using the
   UTF-8 character encoding [RFC3629] to decode the octet sequence.
   This decoding might fail, however, because not every sequence of
   octets is valid UTF-8.
   At least 4096 bytes per cookie (as measured by the sum of the
length of the cookie's name, value, and attributes).
  */

typedef struct
{
    char *name;
    char *value;
    char *domain;
    char *path;
    int secure;
    int http_only;
    int host_only;
    char *max_age;
    time_t last_access_time;
} Cookie;

Cookie *get_cookies_from_store();
void update_last_access_time(Cookie *cookie);
void cookie_header_verif(_Token *node);
void update_last_access_time(Cookie *cookie);
const char default_path[] = "/";

// Magasin de Cookie, j'ai pas capté comment on allait le remplir.
static Cookie cookie_store[] = {
    {"name1", "value1", "domain1", "path1", 0, 0},
    {"name2", "value2", "domain2", "path2", 1, 0},
    {NULL, NULL, NULL, NULL, 0, 0} // Sentinelle.
};

void cookie_header_verif(_Token *node)
{
    // On récupère la valeur du cookie gràce à la fonction getElementValue de l'api.
    const char *cookie_string = getElementValue(node->node, NULL);
    if (cookie_string == NULL)
    {
        return;
    }

    // Check si le Cookie header apparait plus d'une fois. (MUSTNOT)
    int cookie_header_count = 0;
    _Token *current_node = node;
    while (current_node != NULL)
    {
        if (strcmp(getElementName(current_node->node, NULL), "Cookie") == 0)
        {
            cookie_header_count++;
        }
        current_node = current_node->next;
    }

    if (cookie_header_count > 1)
    {
        fprintf(stderr, "Cookie header should not appear more than once\n");
        return;
    }

    // On récupère les cookies stockés dans le magasin
    Cookie *cookies = get_cookies_from_store();
    char header_value[4096] = "";

    // On va vérifier si le cookie est valide.

    char *cookie_start = cookie_string;
    while (cookie_start)
    {
        char *cookie_end = strchr(cookie_start, ';');
        if (cookie_end)
        {
            *cookie_end = '\0';
        }

        Cookie *cookie = NULL;

        // On check tous les cookies du magasin.
        for (int i = 0; cookies[i].name != NULL; i++)
        {
            char *request_host = /*A FAIRE*/ "";
            char *request_path = /*A FAIRE*/ "";
            int is_secure = strncmp(cookie_string, "https", 5) == 0;

            if (cookie->secure)
            {
                // Ajoute l'attribut "Secure" à la liste des attributs du cookie
                strcat(header_value, "; Secure");
            }
            if (cookie->http_only)
            {
                // Ajoute l'attribut "HttpOnly" à la liste des attributs du cookie
                strcat(header_value, "; HttpOnly");
            }
            if (cookie->path)
            {
                // Si la valeur de l'attribut est vide ou si le premier caractère n'est pas '/',
                // utilise le chemin par défaut.
                if (cookie->path[0] == '\0' || cookie->path[0] != '/')
                {
                    cookie->path = default_path;
                }

                // Ajoute l'attribut "Path" à la liste des attributs du cookie
                strcat(header_value, "; Path=");
                strcat(header_value, cookie->path);
            }
            if (cookie->domain)
            {
                // Si la valeur de l'attribut est vide, ignore le cookie
                if (cookie->domain[0] == '\0')
                {
                    continue;
                }

                // Si le premier caractère est '.', supprime le .
                if (cookie->domain[0] == '.')
                {
                    memmove(cookie->domain, cookie->domain + 1, strlen(cookie->domain));
                }

                // Converti le domaine en minuscules
                for (char *p = cookie->domain; *p; ++p)
                {
                    *p = tolower(*p);
                }

                // Ajoute l'attribut "Domain" à la liste des attributs du cookie
                strcat(header_value, "; Domain=");
                strcat(header_value, cookie->domain);
            }
            if (cookie->max_age)
            {
                // Si le premier caractère n'est pas un chiffre ou un '-', ignore le cookie
                if (!isdigit(cookie->max_age[0]) && cookie->max_age[0] != '-')
                {
                    continue;
                }

                // Si le reste de la valeur de l'attribut contient un caractère non numérique, ignore le cookie
                for (char *p = cookie->max_age + 1; *p; ++p)
                {
                    if (!isdigit(*p))
                    {
                        continue;
                    }
                }

                // Convertis la valeur de l'attribut en un entier
                int delta_seconds = atoi(cookie->max_age);

                // Si delta_seconds est inférieur ou égal à zéro, définis expiry_time sur la date et l'heure les plus anciennes possibles
                // Sinon, définis expiry_time sur la date et l'heure actuelles plus delta_seconds secondes
                time_t expiry_time = delta_seconds <= 0 ? 0 : time(NULL) + delta_seconds;

                // Ajoute l'attribut "Max-Age" à la liste des attributs du cookie
                char max_age_str[20];
                sprintf(max_age_str, "; Max-Age=%ld", (long)expiry_time);
                strcat(header_value, max_age_str);
            }

            if ((cookie->host_only && strcmp(request_host, cookie->domain) != 0) ||
                (!cookie->host_only && !domain_matches(request_host, cookie->domain)) ||
                !path_matches(request_path, cookie->path) ||
                (cookie->secure && !is_secure) ||
                (cookie->http_only && strncmp(cookie_string, "HTTP", 4) != 0))
            {
                continue;
            }

            cookie = &cookies[i];
            break;
        }

        if (!cookie)
        {
            fprintf(stderr, "Invalid cookie: %s\n", cookie_start);
            return;
        }

        update_last_access_time(cookie);

        cookie_start = cookie_end ? cookie_end + 1 : NULL;
    }

    printf("Cookie-Header correct\n");
}

Cookie *get_cookies_from_store()
{
    return cookie_store;
}

void update_last_access_time(Cookie *cookie)
{
    cookie->last_access_time = time(NULL);
}

bool domain_matches(const char *request_host, const char *cookie_domain)
{
    // On vérifie si le domaine de la requête est égal au domaine du cookie.
    return strcmp(request_host, cookie_domain) == 0;
}

bool path_matches(const char *request_path, const char *cookie_path)
{
    // On vérifie si le path de la requête est égal au path du cookie.
    return strcmp(request_path, cookie_path) == 0;
}