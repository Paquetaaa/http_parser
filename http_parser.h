typedef enum
{
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
    char *etiquette;        // Etiquette du noeud
    char *pointeur_debut;   // Debut de la string que represente le noeud
    int longueur;           // Taille de la string representee par le noeud
    struct node *pere;      //
    struct node *fils_aine; //
    struct node *frere;     //

} Noeud;

Noeud *construire_arbre(void);
