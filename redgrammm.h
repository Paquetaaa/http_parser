#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

// ABNF TEST
/*SEPARATEUR*/
/*" \t\n-_"*/

/*PONCTUATION*/
/*"," / "." / "!" / "?" / ":"*/

typedef enum
{
    UNRESERVED,
    RESERVED,
    GEN_DELIMS,
    SUB_DELIMS,
    LANGUAGE_RANGE,
    LANGUAGE_TAG,
    LANGTAG,
    LANGUAGE,
    EXTLANG,
    SCRIPT,
    REGION,
    VARIANT,
    EXTENSION,
    SINGLETON,
    PRIVATEUSE,
    GRANDFATHERED,
    IRREGULAR,
    REGULAR,
    BWS,
    CONNECTION,
    CONTENT_LENGTH,
    HOST,
    OWS,
    RWS,
    TE,
    TRAILER,
    TRANSFER_ENCODING,
    UPGRADE,
    VIA,
    COMMENT,
    CONNECTION_OPTION,
    CTEXT,
    HTTP_URI,
    HTTPS_URI,
    PARTIAL_URI,
    PROTOCOL,
    PROTOCOL_NAME,
    PROTOCOL_VERSION,
    PSEUDONYM,
    QDTEXT,
    QUOTED_PAIR,
    QUOTED_STRING,
    RANK,
    RECEIVED_BY,
    RECEIVED_PROTOCOL,
    T_CODINGS,
    T_RANKING,
    TCHAR,
    TOKEN,
    TRAILER_PART,
    TRANSFER_CODING,
    TRANSFER_EXTENSION,
    TRANSFER_PARAMETER,
    URI_HOST,
    ACCEPT,
    ACCEPT_CHARSET,
    ACCEPT_ENCODING,
    ACCEPT_LANGUAGE,
    ALLOW,
    CONTENT_ENCODING,
    CONTENT_LANGUAGE,
    CONTENT_LOCATION,
    CONTENT_TYPE,
    DATE,


    // STEPHANE met tes trucs entre les miens et ceux de Lucas


    UNSATISFIED_RANGE,
    AGE,
    CACHE_CONTROL,
    EXPIRES,
    PRAGMA,
    WARNING,
    CACHE_DIRECTIVE,
    DELTA_SECONDS,
    EXTENSION_PRAGMA,
    PRAGMA_DIRECTIVE,
    WARN_AGENT,
    WARN_CODE,
    WARN_DATE,
    WARN_TEXT,
    WARNING_VALUE,
    PROXY_AUTHENTICATE,
    PROXY_AUTHORIZATION,
    WWW_AUTHENTICATE,
    AUTH_PARAM,
    AUTH_SCHEME,
    CHALLENGE,
    CREDENTIALS,
    AUTHORIZATION,
    TOKEN68,
    CONNECTION_HEADER,
    CONTENT_LENGTH_HEADER,
    CONTENT_TYPE_HEADER,
    TRAILER_HEADER,
    TRANSFER_ENCODING_HEADER,
    UPGRADE_HEADER,
    VIA_HEADER,
    AGE_HEADER,
    EXPIRES_HEADER,
    LOCATION_HEADER,
    LOCATION_HEADER,
    VARY_HEADER,
    WARNING_HEADER,
    CACHE_CONTROL_HEADER,
    EXPECT_HEADER,
    HOST_HEADER,
    MAX_FORWARDS_HEADER,
    PRAGMA_HEADER,
    RANGE_HEADER,
    TE_HEADER,
    IF_MATCH_HEADER,
    IF_NONE_MATCH_HEADER,
    IF_MODIFIED_SINCE_HEADER,
    IF_UNMODIFIED_SINCE_HEADER,
    IF_RANGE_HEADER,
    ACCEPT_HEADER,
    ACCEPT_CHARSET_HEADER,
    ACCEPT_ENCODING_HEADER,
    ACCEPT_LANGUAGE_HEADER,
    AUTHORIZATION_HEADER,
    PROXY_AUTHORIZATION_HEADER,
    FROM_HEADER,
    REFERER_HEADER,
    USER_AGENT_HEADER,
    COOKIE_PAIR,
    COOKIE_NAME,
    COOKIE_VALUE,
    COOKIE_OCTET,
    COOKIE_HEADER,
    COOKIE_STRING,
    HEADER_FIELD,

} TypeNoeud;


typedef struct node
{
    TypeNoeud type;         // Type du noeud
    char* etiquette;        // Etiquette du noeud
    char* pointeur_debut;   // Debut de la string que represente le noeud 
    int longueur;            // Taille de la string representee par le noeud
    struct node* pere;      // 
    struct node* fils_aine; // 
    struct node* frere;     // 

} Noeud;

Noeud* creer_noeud(TypeNoeud type, char* etiquette, char* pointeur_debut, int longueur);
Noeud* chercher_noeud(Noeud* racine, char* mot, int longueur);
Noeud* construire_arbre(char* requete);
void ajouter_frere(Noeud** nd, Noeud* frere);
void ajouter_fils(Noeud** nd, Noeud* fils);
void afficher_arbre(Noeud* racine, int niveau);

