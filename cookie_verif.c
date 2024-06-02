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
    time_t last_access_time;
} Cookie;

Cookie *get_cookies_from_store();
void update_last_access_time(Cookie *cookie);
void cookie_header_verif(_Token *node);
void update_last_access_time(Cookie *cookie);

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

    // On récupère les cookies stockés dans le magasin
    Cookie *cookies = get_cookies_from_store();
    char header_value[4096] = "";
    int first_cookie = 1;

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

    printf("Cookie-Header is semantically correct\n");
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