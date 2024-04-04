#include "struct_parser.h"

/* Note :
 * Le fichier http_parser.c contient toutes les fonctions utiles, et uniquement utiles, pour la grammaire complete.
 * Le fichier http_parser.c est a vocation de ne plus contenir de main (au profit du fichier main.c).
 * Il contiendra une routine main tant que la periode de tests est en cours. 
*/

Noeud* construire_arbre(const char* file_pathname){
    if (file_pathname == NULL){
        fprintf(stderr, "<construire_arbre> necessite un fichier en entree\n");
        exit(1);
    }

    FILE* file = fopen(file_pathname, "r");
    if (file == NULL){
        fprintf(stderr, "Erreur fopen <construire_arbre>\n");
        exit(1);
    }

// Determination de la taille du fichier (nombre de caracteres, incluant les CRLF)
    unsigned int taille_requete = 0;
    while(fgetc(file) != -1 ){
        taille_requete++;
    }

// Repositionnement du pointeur au debut du fichier
    fseek(file, 0, SEEK_SET);


// Construction de la racine & de son fils ainee : la start-line
    char* ligne;
    size_t espace_alloc = 0;
    ssize_t taille_ligne;

    if((taille_ligne = getline(&ligne, &espace_alloc, file))  < 0){
        fprintf(stderr, "Erreur getline <construire_arbre>\n");
        exit(1);
    }
/* PROBLEME POUR STOCKER LE POINTEUR VERS LE DEBUT DE LA LIGNE !!
 * Il faudrait voir si on prend un char* (et la ca s'affiche bien mais l'ordre n'est plus maintenu...)
 * ou alors on prend un char** (et la l'ordre est OK mais l'affichage est soit bugue, soit provoque des segfault...)
 * */

// Cas de la racine
    char* ptr_start_line = (char*) malloc(sizeof(char));
    ptr_start_line = ligne;
    char etiquette_tmp[] = "HTTP_message";
    int taille_etiquette = strlen(etiquette_tmp);
    char* etiquette = (char*) malloc(taille_etiquette * sizeof(char));
    strncpy(etiquette, etiquette_tmp, taille_etiquette); 
    Noeud* racine = creer_noeud(HTTP_message, etiquette, ptr_start_line, taille_requete);

// Cas de la start-line
    char nv_etiquette_tmp[] = "start_line";
    taille_etiquette = strlen(nv_etiquette_tmp);
    char* nv_etiquette = (char*) malloc(taille_etiquette * sizeof(char));
    strncpy(nv_etiquette, nv_etiquette_tmp, taille_etiquette);
    Noeud* nd_start_line = creer_noeud(start_line, nv_etiquette, ptr_start_line, taille_ligne -4 -1);

    ajouter_fils(&racine, nd_start_line);

// Cas du noeud separant chaque ligne de la requete
    char etiquette_crlf_tmp[] = "_crlf";
    taille_etiquette = strlen(etiquette_crlf_tmp);
    char* etiquette_crlf = (char*) malloc(taille_etiquette * sizeof(char));
    strncpy(etiquette_crlf, etiquette_crlf_tmp, taille_etiquette);
    Noeud* nd_crlf = creer_noeud(CRLF, etiquette_crlf, ptr_start_line + taille_ligne -5, 4);

    ajouter_fils(&racine, nd_crlf);


// Re-iteration des 2 dernieres etapes pour inserer toutes les lignes de la requete dans l'arbre
    char etiquette_hf_tmp[] = "header_field";
    taille_etiquette = strlen(etiquette_hf_tmp);
    char* etiquette_hf = (char*) malloc(taille_etiquette * sizeof(char));
    strncpy(etiquette_hf, etiquette_hf_tmp, taille_etiquette);

    while((taille_ligne = getline(&ligne, &espace_alloc, file)) != -1){
        char* ptr_ligne_lue = (char*) malloc(sizeof(char)); 
        ptr_ligne_lue = ligne;

        Noeud* nd_hf = creer_noeud(header_field, etiquette_hf, ptr_ligne_lue, taille_ligne);

        Noeud* nd_crlf_bis = creer_noeud(CRLF, etiquette_crlf, ptr_ligne_lue + taille_ligne, 4);

        ajouter_fils(&racine, nd_hf);
        ajouter_fils(&racine, nd_crlf_bis);
    }


    fclose(file);
    return racine;
}

void traitement_start_line(Noeud* nd_sl){
}

void traitement(Noeud* nd){
/*
    if(is_expect(token)){
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));
    }
    else if(is_GMT(token)){
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));
    }
    else if(is_http_date(token)){
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));
    }
    else if(is_day_name(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_day_name_l(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_delay_seconds(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_location(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_max_forwards(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_referer(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_retry_after(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_server(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_charset(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_product(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_content_coding(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_subtype(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_type(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_codings(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_date2(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_date1(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_date3(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_day(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_hour(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_minute(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_second(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_year(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_month(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_obs_date(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));


    else if(is_etag(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_last_modified(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_accept_ranges(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_if_range(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_range(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_range_unit(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_bytes_unit(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_complete_length(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_first_byte_pos(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_last_byte_pos(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_suffix_length(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_suffix_byte_range_spec(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_other_content_range(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_other_range_resp(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_other_range_set(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_other_range_unit(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_other_range_specifier(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_weak(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_user_agent(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_vary(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_accept_ext(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_accept_params(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_asctime_date(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_media_range(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_parameter(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_product(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));


    else if(is_qvalue(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_rfc850_date(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_time_of_day(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_weight(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_if_match(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_if_modified_since(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_if_none_match(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_unmodified_since(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_entity_tag(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_acceptable_ranges(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_byte_content_range(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_byte_range(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_byte_range_resp(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_byte_range_spec(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_byte_range_specifier(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));

    else if(is_byte_range_set(token))
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));
        
    else if (is_unsatisfied_range(token[0]))
    {
        n = creer_noeud(UNSATISFIED_RANGE, "unsatisfied-range", token, strlen(token));
    }
    else if (is_age(token[0]))
    {
        n = creer_noeud(AGE, "age", token, strlen(token));
    }
    else if (is_Cache_Control(token[0]))
    {
        n = creer_noeud(CACHE_CONTROL, "Cache-Control", token, strlen(token));
    }
    else if (is_Expires(token[0]))
    {
        n = creer_noeud(EXPIRES, "expires", token, strlen(token));
    }
    else if (is_Pragma(tokn[0]))
    {
        n = creer_noeud(PRAGMA, "pragma", token, strlen(token));
    }
    else if (is_Warning(token[0]))
    {
        n = creer_noeud(WARNING, "Warning", token, strlen(token));
    }
    else if (is_cache_directive(token[0]))
    {
        n = creer_noeud(CACHE_DIRECTIVE, "cache-directive", token, strlen(token));
    }
    else if (is_delta_seconds(token[0]))
    {
        n = creer_noeud(DELTA_SECONDS, "delta-seconds", token, strlen(token));
    }
    else if (is_extension_pragma(token[0]))
    {
        n = creer_noeud(EXTENSION_PRAGMA, "extension-pragma", token, strlen(token));
    }
    else if (is_pragma_directive(token[0]))
    {
        n = creer_noeud(PRAGMA_DIRECTIVE, "pragma-directive", token, strlen(token));
    }
    else if (is_warn_agent(token[0]))
    {
        n = creer_noeud(WARN_AGENT, "warn-agent", token, strlen(token));
    }
    else if (is_warn_code(token[0]))
    {
        n = creer_noeud(WARN_CODE, "warn-code", token, strlen(token));
    }
    else if (is_warn_date(token[0]))
    {
        n = creer_noeud(WARN_DATE, "warn-date", token, strlen(token));
    }
    else if (is_warn_text(token[0]))
    {
        n = creer_noeud(WARN_TEXT, "warn-text", token, strlen(token));
    }
    else if (is_warning_value(token[0]))
    {
        n = creer_noeud(WARNING_VALUE, "warning-value", token, strlen(token));
    }
    else if (is_Proxy_Authenticate(token[0]))
    {
        n = creer_noeud(PROXY_AUTHENTICATE, "Proxy-Authenticate", token, strlen(token));
    }
    else if (is_Proxy_Authorization(token[0]))
    {
        n = creer_noeud(PROXY_AUTHORIZATION, "Proxy-Authorization", token, strlen(token));
    }
    else if (is_WWW_Authenticate(token[0]))
    {
        n = creer_noeud(WWW_AUTHENTICATE, "WWW-Authenticate", token, strlen(token));
    }
    else if (is_auth_param(token[0]))
    {
        n = creer_noeud(AUTH_PARAM, "auth-param", token, strlen(token));
    }
    else if (is_auth_scheme(token[0]))
    {
        n = creer_noeud(AUTH_SCHEME, "auth-scheme", token, strlen(token));
    }
    else if (is_challenge(token[0]))
    {
        n = creer_noeud(CHALLENGE, "challenge", token, strlen(token));
    }
    else if (is_credentials(token[0]))
    {
        n = creer_noeud(CREDENTIALS, "credentials", token, strlen(token));
    }
    else if (is_Authorization(token[0]))
    {
        n = creer_noeud(AUTHORIZATION, "Authorization", token, strlen(token));
    }
    else if (is_token68(token[0]))
    {
        n = creer_noeud(TOKEN68, "token68", token, strlen(token));
    }
    else if (is_Connection_header(token[0]))
    {
        n = creer_noeud(CONNECTION_HEADER, "Connection-header", token, strlen(token));
    }
    else if (is_Content_Length_header(token[0]))
    {
        n = creer_noeud(CONTENT_LENGTH_HEADER, "Content-Length-header", token, strlen(token));
    }
    else if (is_Content_Type_header(token[0]))
    {
        n = creer_noeud(CONTENT_TYPE_HEADER, "Content-Type-header", token, strlen(token));
    }
    else if (is_Trailer_header(token[0]))
    {
        n = creer_noeud(TRAILER_HEADER, "Trailer-header", token, strlen(token));
    }
    else if (is_Transfer_Encoding_header(token[0]))
    {
        n = creer_noeud(TRANSFER_ENCODING_HEADER, "Transfer-Encoding-header", token, strlen(token));
    }
    else if (is_Upgrade_header(token[0]))
    {
        n = creer_noeud(UPGRADE_HEADER, "Upgrade-header", token, strlen(token));
    }
    else if (is_Via_header(token[0]))
    {
        n = creer_noeud(VIA_HEADER, "Via-header", token, strlen(token));
    }
    else if (is_Age_header(token[0]))
    {
        n = creer_noeud(AGE_HEADER, "Age-header", token, strlen(token));
    }
    else if (is_Expires_header(token[0]))
    {
        n = creer_noeud(EXPIRES_HEADER, "Expires-header", token, strlen(token));
    }
    else if (is_Date_header(token[0]))
    {
        n = creer_noeud(DATE_HEADER, "Date-header", token, strlen(token));
    }
    else if (is_Location_header(token[0]))
    {
        n = creer_noeud(LOCATION_HEADER, "Location-header", token, strlen(token));
    }
    else if (is_Retry_After_header(token[0]))
    {
        n = creer_noeud(RETRY_AFTER_HEADER, "Retry-After-header", token, strlen(token));
    }
    else if (is_Vary_header(token[0]))
    {
        n = creer_noeud(VARY_HEADER, "Vary-header", token, strlen(token));
    }
    else if (is_Warning_header(token[0]))
    {
        n = creer_noeud(WARNING_HEADER, "Warning-header", token, strlen(token));
    }
    else if (is_Cache_Control_header(token[0]))
    {
        n = creer_noeud(CACHE_CONTROL_HEADER, "Cache-Control-header", token, strlen(token));
    }
    else if (is_Expect_header(token[0]))
    {
        n = creer_noeud(EXPECT_HEADER, "Expect-header", token, strlen(token));
    }
    else if (is_Host_header(token[0]))
    {
        n = creer_noeud(HOST_HEADER, "Host-header", token, strlen(token));
    }
    else if (is_Max_Forwards_header(token[0]))
    {
        n = creer_noeud(MAX_FORWARDS_HEADER, "Max-Forwards-header", token, strlen(token));
    }
    else if (is_Pragma_header(token[0]))
    {
        n = creer_noeud(PRAGMA_HEADER, "Pragma-header", token, strlen(token));
    }
    else if (is_Range_header(token[0]))
    {
        n = creer_noeud(RANGE_HEADER, "Range-header", token, strlen(token));
    }
    else if (is_TE_header(token[0]))
    {
        n = creer_noeud(TE_HEADER, "TE-header", token, strlen(token));
    }
    else if (is_If_Match_header(token[0]))
    {
        n = creer_noeud(IF_MATCH_HEADER, "If-Match-header", token, strlen(token));
    }
    else if (is_If_None_Match_header(token[0]))
    {
        n = creer_noeud(IF_NONE_MATCH_HEADER, "If-None-Match-header", token, strlen(token));
    }
    else if (is_If_Modified_Since_header(token[0]))
    {
        n = creer_noeud(IF_MODIFIED_SINCE_HEADER, "If-Modified-Since-header", token, strlen(token));
    }
    else if (is_If_Unmodified_Since_header(token[0]))
    {
        n = creer_noeud(IF_UNMODIFIED_SINCE_HEADER, "If-Unmodified-Since-header", token, strlen(token));
    }
    else if (is_If_Range_header(token[0]))
    {
        n = creer_noeud(IF_RANGE_HEADER, "If-Range-header", token, strlen(token));
    }
    else if (is_Accept_header(token[0]))
    {
        n = creer_noeud(ACCEPT_HEADER, "Accept-header", token, strlen(token));
    }
    else if (is_Accept_Charset_header(token[0]))
    {
        n = creer_noeud(ACCEPT_CHARSET_HEADER, "Accept-Charset-header", token, strlen(token));
    }
    else if (is_Accept_Encoding_header(token[0]))
    {
        n = creer_noeud(ACCEPT_ENCODING_HEADER, "Accept-Encoding-header", token, strlen(token));
    }
    else if (is_Accept_Language_header(token[0]))
    {
        n = creer_noeud(ACCEPT_LANGUAGE_HEADER, "Accept-Language-header", token, strlen(token));
    }
    else if (is_Authorization_header(token[0]))
    {
        n = creer_noeud(AUTHORIZATION_HEADER, "Authorization-header", token, strlen(token));
    }
    else if (is_Proxy_Authorization_header(token[0]))
    {
        n = creer_noeud(PROXY_AUTHORIZATION_HEADER, "Proxy-Authorization-header", token, strlen(token));
    }
    else if (is_From_header(token[0]))
    {
        n = creer_noeud(FROM_HEADER, "From-header", token, strlen(token));
    }
    else if (is_Referer_header(token[0]))
    {
        n = creer_noeud(REFERER_HEADER, "Referer-header", token, strlen(token));
    }
    else if (is_User_Agent_header(token[0]))
    {
        n = creer_noeud(USER_AGENT_HEADER, "User-Agent-header", token, strlen(token));
    }
    else if (is_cookie_pair(token[0]))
    {
        n = creer_noeud(COOKIE_PAIR, "cookie-pair", token, strlen(token));
    }
    else if (is_cookie_name(token[0]))
    {
        n = creer_noeud(COOKIE_NAME, "cookie-name", token, strlen(token));
    }
    else if (is_cookie_value(token[0]))
    {
        n = creer_noeud(COOKIE_VALUE, "cookie-value", token, strlen(token));
    }
    else if (is_cookie_octet(token[0]))
    {
        n = creer_noeud(COOKIE_OCTET, "cookie-octet", token, strlen(token));
    }
    else if (is_Cookie_header(token[0]))
    {
        n = creer_noeud(COOKIE_HEADER, "Cookie-header", token, strlen(token));
    }
    else if (is_Cookie_string(token[0]))
    {
        n = creer_noeud(COOKIE_STRING, "Cookie-string", token, strlen(token));
    }
    else if (is_header_field(token[0]))
    {
        n = creer_noeud(HEADER_FIELD, "header-field", token, strlen(token));
    }
    else if(is_unreserved(token[0])){
        n = creer_noeud(UNRESERVED,"unreserved",token,strlen(token));
    }
    else if(is_reserved(token[0])){
        n = creer_noeud(RESERVED,"reserved",token,strlen(token));
    }
    else if(is_gen_delims(token[0])){
        n = creer_noeud(GEN_DELIMS,"gen_delims",token,strlen(token));
    }
    else if(is_sub_delims(token[0])){
        n = creer_noeud(SUB_DELIMS,"sub_delims",token,strlen(token));
    }
    else if(is_alphanum(token[0])){
        n = creer_noeud(ALPHANUM,"alphanum",token,strlen(token));
    }
    else if(is_language_range(token)){
        n = creer_noeud(LANGUAGE_RANGE,"language_range",token,strlen(token));
    }
    else if(is_language(token)){
        n = creer_noeud(LANGUAGE,"language",token,strlen(token));
    }
    else if(is_langtag(token)){
        n = creer_noeud(LANGTAG,"langtag",token,strlen(token));
    }
    else if(is_language_tag(token)){
        n = creer_noeud(LANGUAGE_TAG,"language_tag",token,strlen(token));
    }
    else if(is_extlang(token)){
        n = creer_noeud(EXTLANG,"extlang",token,strlen(token));
    }
    else if(is_script(token)){
        n = creer_noeud(SCRIPT,"script",token,strlen(token));
    }
    else if(is_region(token)){
        n = creer_noeud(REGION,"region",token,strlen(token));
    }
    else if(is_variant(token)){
        n = creer_noeud(VARIANT,"variant",token,strlen(token));
    }
    else if(is_extension(token)){
        n = creer_noeud(EXTENSION,"extension",token,strlen(token));
    }
    else if(is_singleton(token[0])){
        n = creer_noeud(SINGLETON,"singleton",token,strlen(token));
    }
    else if(is_privateuse(token)){
        n = creer_noeud(PRIVATEUSE,"privateuse",token,strlen(token));
    }
    else if(is_grandfathered(token)){
        n = creer_noeud(GRANDFATHERED,"grandfathered",token,strlen(token));
    }
    else if(is_irregular(token)){
        n = creer_noeud(IRREGULAR,"irregular",token,strlen(token));
    }
    else if(is_regular(token)){
        n = creer_noeud(REGULAR,"regular",token,strlen(token));
    }
    else if(is_host(token)){
        n = creer_noeud(HOST,"host",token,strlen(token));
    }
    else if(is_OWS(token)){
        n = creer_noeud(OWS,"OWS",token,strlen(token));
    }
    else if(is_BWS(token)){
        n = creer_noeud(BWS,"BWS",token,strlen(token));
    }
    else if(is_connection(token)){
        n = creer_noeud(CONNECTION,"connection",token,strlen(token));
    }
    else if(is_RWS(token)){
        n = creer_noeud(RWS,"RWS",token,strlen(token));
    }
    else if(is_comment(token)){
        n = creer_noeud(COMMENT,"comment",token,strlen(token));
    }
    else if(is_http_URI(token)){
        n = creer_noeud(HTTP_URI,"http_uri",token,strlen(token));
    }
    else if(is_content_length(token)){
        n = creer_noeud(CONTENT_LENGTH,"content_length",token,strlen(token));
    }
    else if(is_tchar(token[0])){
        n = creer_noeud(TCHAR,"tchar",token,strlen(token));
    }
    else if(is_token(token)){
        n = creer_noeud(TOKEN,"token",token,strlen(token));
    }
    else if(is_trailer_part(token)){
        n = creer_noeud(TRAILER_PART,"trailer_part",token,strlen(token));
    }
    else if(is_transfer_coding(token)){
        n = creer_noeud(TRANSFER_CODING,"transfer_coding",token,strlen(token));
    }
    else if(is_transfer_extension(token)){
        n = creer_noeud(TRANSFER_EXTENSION,"transfer_extension",token,strlen(token));
    }
    else if(is_transfer_parameter(token)){
        n = creer_noeud(TRANSFER_PARAMETER,"transfer_parameter",token,strlen(token));
    }
    else if(is_ctext(token[0])){
        n = creer_noeud(CTEXT,"ctext",token,strlen(token));
    }
    else if(is_partial_uri(token)){
        n = creer_noeud(PARTIAL_URI,"partial_uri",token,strlen(token));
    }
    else if(is_protocol(token)){
        n = creer_noeud(PROTOCOL,"protocol",token,strlen(token));
    }
    else if(is_protocol_name(token)){
        n = creer_noeud(PROTOCOL_NAME,"protocol_name",token,strlen(token));
    }
    else if(is_protocol_version(token)){
        n = creer_noeud(PROTOCOL_VERSION,"protocol_version",token,strlen(token));
    }
    else if(is_pseudonym(token)){
        n = creer_noeud(PSEUDONYM,"pseudonym",token,strlen(token));
    }
    else if(is_qd_text(token[0])){
        n = creer_noeud(QD_TEXT,"qd_text",token,strlen(token));
    }
    else if(is_quoted_pair(token)){
        n = creer_noeud(QUOTED_PAIR,"quoted_pair",token,strlen(token));
    }
    else if(is_quoted_string(token)){
        n = creer_noeud(QUOTED_STRING,"quoted_string",token,strlen(token));
    }
    else if(is_received_by(token)){
        n = creer_noeud(RECEIVED_BY,"received_by",token,strlen(token));
    }
    else if(is_received_protocol(token)){
        n = creer_noeud(RECEIVED_PROTOCOL,"received_protocol",token,strlen(token));
    }
    else if(is_rank(token)){
        n = creer_noeud(RANK,"rank",token,strlen(token));
    }
    else if(is_t_codings(token)){
        n = creer_noeud(T_CODINGS,"t_codings",token,strlen(token));
    }
    else if(is_t_ranking(token)){
        n = creer_noeud(T_RANKING,"t_ranking",token,strlen(token));
    }
    else if(is_uri_host(token)){
        n = creer_noeud(URI_HOST,"uri_host",token,strlen(token));
    }
    else if(is_content_location(token)){
        n = creer_noeud(CONTENT_LOCATION,"content_location",token,strlen(token));
    }
    else if(is_content_type(token)){
        n = creer_noeud(CONTENT_TYPE,"content_type",token,strlen(token));
    }
    else if(is_date(token)){
        n = creer_noeud(DATE,"date",token,strlen(token));
    }
*/ 
}
/*
bool is_CRLF(char* chaine){
    int taille = strlen(chaine);

    if (taille == 4 && strncmp(chaine, "\r\n", 4) == 0){
        return true;
    }

    return false;
}

bool is_VCHAR(char c){
    return (c >= 0x33 && c <= 0x126);
}

bool is_HTTP_message(char* chaine){
    int taille = strlen(chaine);
    char copie[taille];
    strncpy(copie, chaine, taille);


    char* token = strtok(copie, " ");
    int taille_token = strlen(token);

    char* possible_start_line[taille_token -4];
    char* possible_CRLF_start_line[4];

    strncpy(possible_CRLF_start_line, token + taille_token -4, 4);

    if (!is_start_line(token) && (!is_start_line(possible_start_line) || !is_CRLF(possible_CRLF_start_line))){
        return false;
    }

    while((token = strtok(NULL, " ")) != NULL){
        taille_token = strlen(token);

        if (taille_token == 4){
            if(!is_CRLF(token)){
                return false;
            }
        } else{
            char possible_header_field[taille_token -4];
            char possible_CRLF[4];
            strncpy(possible_header_field, token, taille_token -4);
            strncpy(possible_CRLF, token + taille_token-4, 4);

            if ((!is_header_field(possible_header_field) || !is_CRLF(possible_CRLF)) && !is_message_body(token))
                return false;
        }
    }

    return true;
}

bool is_HTTP_name(char* chaine){
    int taille = strlen(chaine);

    if (taille == 4 && strncmp(chaine, "HTTP", 4) == 0){
        return true;
    }

    return false;
}

bool is_HTTP_version(char* chaine){
    int taille = strlen(chaine);

    if (!taille == 8){
        return false;
    }

    char* possible_http_name;
    strncpy(possible_http_name, chaine, 4);
    return (is_HTTP_name(possible_http_name) && chaine[4] == '/' && isdigit(chaine[5]) && chaine[6] == '.' && isdigit(chaine[7]));
}
*/
/* A FAIRE !! */
/*
bool is_field_content(char* chaine){
    return false;
}

bool is_field_name(char* chaine){
    return is_token(chaine);
}

bool is_field_value(char* chaine){
    int taille = strlen(chaine);

    if (taille == 0){
        return true;
    }

    char copie[taille];
    strncpy(copie, chaine, taille);

    char* token = strtok(copie, " ");
    while(token != NULL){
        if(!is_field_content(token) && !is_obs_fold(token)){
            return false;
        }

        token = strtok(NULL, " ");
    }
    
    return true;
}

bool is_field_vchar(char c){
    return (is_VCHAR(c) || is_obs_text(c));
}

bool is_last_chunk(char* chaine){
    int taille = strlen(chaine);

    if(chaine[0] != '0'){
        return false;
    }

    char copie[taille];
    char possible_CRLF[4];
    strncpy(possible_CRLF, chaine + taille -4, 4);
    strncpy(copie, chaine, taille);

    if(!is_CRLF(possible_CRLF)){
        return false;
    }

    char* token = strtok(copie, "0");
    if (token != NULL && token != "\r\n" && !is_chunk_ext(token)){
        return false;
    }

    return true;
}
*/
/* PAS SUR DE CELUI-CI */
/*
bool is_message_body(char* chaine){
    return true;
}

bool is_method(char* chaine){
    return is_token(chaine);
}

bool is_obs_fold(char* chaine){
    int taille = strlen(chaine);

    if (taille < 5){
        return false;
    }

    char possible_CRLF[4];
    strncpy(possible_CRLF, chaine, 4);
    if (!is_CRLF(possible_CRLF)){
        return false;
    }

    for (int i = 4; i < taille; i++){
        if (chaine[i] != ' ' && chaine[i] !='\t'){
            return false;
        }
    }

    return true;
}

bool is_obs_text(char c){
    return (c >= 0x80 && c <= 0xff);
}

bool is_origin_form(char* chaine){
    int taille = strlen(chaine);
    char copie[taille];
    strncpy(copie, chaine, taille);

    char* token = strtok(copie, "?");

    if (strcmp(token, copie) == 0){
        return is_absolute_path(chaine);
    }

    if (!is_absolute_path(token)){
        return false;
    }

    token = strtok(NULL, "?");
    if(!is_query(token)){
        return false;
    }

    return true;
}

bool is_reason_phrase(char* chaine){
    int taille = strlen(chaine);

    for(int i = 0; i < taille; i++){
        if (chaine[i] != '\t' && chaine[i] != ' ' && !is_VCHAR(chaine[i]) && !is_obs_text(chaine[i])){
            return false;
        }
    }

    return true;
}

bool is_request_line(char* chaine){
    int taille = strlen(chaine);

    char copie[taille];
    strncpy(copie, chaine, taille);

    char* token = strtok(copie, " ");
    if (!is_method(token)){
        return false;
    }

    token = strtok(NULL, " ");
    if (!is_request_target(token)){
        return false;
    }

    token = strtok(NULL, " ");
*/
    /* FAIRE LA DISTINCTION ENTRE LE CRLF COLLE ET SEPARE PAR UN OU PLUSIEURS ESPACES !! */
/*
    if (!is_HTTP_version(token)){
        return false;
    }


}
*/
/*
bool is_unreserved(char c)
{
    return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

bool is_gen_delims(char c)
{
    switch(c) {
        case ':': return true;
        case '/': return true;
        case '?': return true;
        case '#': return true;
        case '[': return true;
        case ']': return true;
        case '@': return true;
        default: return false;
    }
}

bool is_sub_delims(char c)
{
    
    // cas spécial pour ' parce qu'on peut pas écrire '''
    if(&c == "'"){
        return true;
    }
    switch(c){
        case '!': return true;
        case '$': return true;
        case '&': return true;
        case '(': return true;
        case ')': return true;
        case '*': return true;
        case '+': return true;
        case ',': return true;
        case ';': return true;
        case '=': return true;
        default: return false;
    }
}
bool is_reserved(char c)
{
    return (is_gen_delims || is_sub_delims);
}
/*
language-range   = (1*8ALPHA *("-" 1*8alphanum)) / "*"
alphanum         = ALPHA / DIGIT
*/
/*
bool is_alphanum(char c)
{
    return isalnum(c);
}
bool is_language_range(char *chaine)
{
    int taille = strlen(chaine);
    if (taille == 1 && chaine[0] == '*') {
        return true;
    } 
    /*on est dans la premier partie du truc : il faut que ce soit
        entre un et 8 alpha, un - et entre 1 et 8 alphanum donc on fait 3 compteurs et a la fin on vérifie
    */
   /*
    else {
        int alpha = 0;
        int alphanum = 0;
        int tiret = 0;
        bool enchainement = true;

        for (int i = 0; i < taille; i++) {
            if (isalpha(chaine[i])) {
                alpha++;
                alphanum++;
            } else if (isdigit(chaine[i]) || chaine[i] == '-') {
                alphanum++;
                if (chaine[i] == '-') {
                    tiret++;
                }
                if (i > 0 && chaine[i - 1] == '-') {
                    enchainement = false;
                }
            } else {
                return false;
            }
        }

        return (tiret == 1 && (alpha >= 1 && alpha <= 8) && (alphanum >= 1 && alphanum <= 8) && enchainement);
    }
}
*/

/*language      = 2*3ALPHA            ; shortest ISO 639 code			
["-" extlang]       ; sometimes followed by			
; extended language subtags			
entre deux et trois alpha et peut être un extlang*/
/*
bool is_language(char* chaine)
{
    int taille = strlen(chaine);
    int index = 0;
    if (taille < 2 || taille > 3) {
        return false;
    }
    for (int i = 0; i < taille; i++) {
        if (!isalpha(chaine[i])) {
            return false;
        }
    }
    while (index < taille && chaine[index] != '-') {
        index++;
    }
    if (index == taille) {
        return true;
    } else {
        if (index < taille - 1) {
            return is_extlang(chaine + index + 1);
        } else {
            return false;
        }
    }
}
*/

/*langtag       = language
                 ["-" script]
                 ["-" region]
                 *("-" variant)
                 *("-" extension)
                 ["-" privateuse]
                 donc on a forcement un langage, ptetre un script et une région, un certain nombre de variant
                 un certain nombre d'extension
                 et ptetre une privateuse
                 j'utilise la même chose que ce qu'a fait lucas pour la grammaire simple
                 */
/*
bool is_langtag(char* chaine)
{
    int taille = strlen(chaine);
    int i = 0;
    char* delimiteur = "-";
    char* token = strtok(chaine,delimiteur);
    if(token == NULL || !is_language(token)){
        return false;
    }
    token = strtok(NULL,delimiteur);


    return true;
}
*/


/*
Language-Tag  = langtag             ; normal language tags
               / privateuse          ; private use tag
               / grandfathered       ; grandfathered tags
*/
/*
bool is_language_tag(char* chaine)
{
    return (is_langtag(chaine) || is_privateuse(chaine) || is_grandfathered(chaine));
}
*/

/*
extlang       = 3ALPHA              ; selected ISO 639 codes
                 *2("-" 3ALPHA)      ; permanently reserved*/
/*
bool is_extlang(char* chaine)
{
    int taille = strlen(chaine);
    if(taille == 11){
        bool premier;
        bool deuxieme;
        bool troisieme;
        premier = isalpha(chaine[0]) && isalpha(chaine[1]) && isalpha(chaine[2]);
        deuxieme = (chaine[3] == '-') && isalpha(chaine[4]) && isalpha(chaine[5] && isalpha(chaine[6]));
        troisieme = (chaine[7] == '-') && isalpha(chaine[8]) && isalpha(chaine[9] && isalpha(chaine[10]));
        return premier && deuxieme && troisieme;
    }
    else{
        return false;
    }
}
*/

/* script        = 4ALPHA              ; ISO 15924 code*/
// on vérifie juste que la chaine est de longueur 4 et que ce sont bien des alpha
/*
bool is_script(char* chaine)
{
    int taille = strlen(chaine);
    if(taille == 4){
        return(isalpha(chaine[0]) && isalpha(chaine[1]) && isalpha(chaine[2]) && isalpha(chaine[3]));
    }
    else{
        return false;
    }
}
*/


/*region        = 2ALPHA              ; ISO 3166-1 code
               / 3DIGIT              ; UN M.49 code*/
// faut vérifier que c'est 2alpha ou 3 digits
/*
bool is_region(char *chaine)
{
    int taille = strlen(chaine);
    if(taille == 2){
        return (isalpha(chaine[0]) && isalpha(chaine[1]));
    } 
    else if(taille == 3){
        return (isdigit(chaine[0]) && isdigit(chaine[1]) && isdigit(chaine[2]));
    }
    else{
        return false;
    }
}
*/

/*variant       = 5*8alphanum         ; registered variants		
/ (DIGIT 3alphanum)		
	donc les possibilités sont entre 5 et 8 alpha num ou un digit et 3 alphanum	*/
/*
bool is_variant(char *chaine)
{
    int taille = strlen(chaine);
    if(taille >= 5 && taille <= 8){
        for(int i = 0; i < taille; i++){
            if(!isalnum(chaine[i])){
                return false;
            }
        }
        return true;
    }
    else if(taille == 4 && isdigit(chaine[0])){
        for(int i = 1; i < taille; i++){
            if(!isalnum(chaine[i])){
                return false;
            }
        }
        return true;
        
    }
    return false;
    
}
*/

/*extension     = singleton 1*("-" (2*8alphanum))		*/
/*
bool is_extension(char *chaine)
{
    int taille = strlen(chaine);
    if (taille == 1) {
        return is_singleton(chaine[0]);
    }

    int index = 1;
    while (index < taille) {
        if (chaine[index] == '-') {
            index++;
        } else {
            return false; 
        }
        
        int count = 0;
        while (index < taille && isalnum(chaine[index]) && count <= 8) {
            index++;
            count++;
        }
        if (count == 0 || count > 8) {
            return false;
        }
    }
    return true; 
}
*/


/*
singleton     = DIGIT               ; 0 - 9
               / %x41-57             ; A - W
               / %x59-5A             ; Y - Z
               / %x61-77             ; a - w
               / %x79-7A             ; y - z
*/
/*
bool is_singleton(char c)
{
    return (is_number(c)|| (c >= 'A' && c <= 'W') || (c >= 'Y' && c <= 'Z') || (c >= 'a' && c <= 'w') || (c >= 'y' && c <= 'z')); 
}
*/
/*privateuse    = "x" 1*("-" (1*8alphanum))			
donc c'est un x et rien du tout
ou un x et un certain nombre d'enchainement de tiret et entre 1 et 8 alphanum
donc on fait plusieurs cas*/
/*
bool is_privateuse(char* chaine)
{
    int taille = strlen(chaine);
    if(taille == 1){
        return (chaine[0] == 'x');
    }
    else if(chaine[0] == 'x'){
        int index = 1;
        while(index < taille){
            if(chaine[index] == '-'){
                index++;
            }
            else{
                return false;
            }
            int count = 0;
            while (index < taille && isalnum(chaine[index]) && count <= 8) {
                index++;
                count++;
            }
            if (count == 0 || count > 8) {
                return false;
            }
        }
    }
    return true;
}
*/

/*
grandfathered = irregular           ; non-redundant tags registered
               / regular             ; during the RFC 3066 era
*/
/*
bool is_grandfathered(char * chaine)
{
    return (is_irregular(chaine) || is_regular(chaine));
}

/*pour regular et irregular c'est juste des grands cases*/
// en fait non faut changer et faire des strcmp sad
/*
bool is_irregular(char* chaine)
{
    int cas1 = strcmp(chaine,"en-GB-oed");
    int cas2 = strcmp(chaine,"i-ami");
    int cas3 = strcmp(chaine,"i-bnn");
    int cas4 = strcmp(chaine,"i-default");
    int cas5 = strcmp(chaine,"i-enochian");
    int cas6 = strcmp(chaine,"i-hak");
    int cas7 = strcmp(chaine,"i-klingon");
    int cas8 = strcmp(chaine,"i-lux");
    int cas9 = strcmp(chaine,"i-mingo");
    int cas10 = strcmp(chaine,"i-navajo");
    int cas11 = strcmp(chaine,"i-pwn");
    int cas12 = strcmp(chaine,"i-tao");
    int cas13 = strcmp(chaine,"i-tay");
    int cas14 = strcmp(chaine,"i-tsu");
    int cas15 = strcmp(chaine,"sgn-BE-FR");
    int cas16 = strcmp(chaine,"sgn-BE-NL");
    int cas17 = strcmp(chaine,"sgn-CH-DE");
    return(!cas1 || !cas2 || !cas3 || !cas4 || !cas5 || !cas6 || !cas7 || !cas8 || !cas9 || !cas10 || !cas11 || !cas12
    || !cas13 || !cas14 || !cas15 || !cas16 || !cas17);
    
    
}

bool is_regular(char* chaine)
{
    int cas1 = strcmp(chaine,"art-lojban");
    int cas2 = strcmp(chaine,"cel-gaulish");
    int cas3 = strcmp(chaine,"no-bok");
    int cas4 = strcmp(chaine,"no-nyn");
    int cas5 = strcmp(chaine,"zh-guoyu");
    int cas6 = strcmp(chaine,"zh-hakka");
    int cas7 = strcmp(chaine,"zh-min");
    int cas8 = strcmp(chaine,"zh-min-nan");
    int cas9 = strcmp(chaine,"zh-xiang");
    return(!cas1 || !cas2 || !cas3 || !cas4 || !cas5 || !cas6 || !cas7 || !cas8 || !cas9 );
    
    

}
*/

/*OWS = *( SP / HTAB )
donc c'est un sp, un htab ou pleins*/
/*
bool is_SP(char c)
{
    return c == ' ';
}
bool is_HTAB(char c)
{
    return c == '\t';
}
*/
/*Host = uri-host [ ":" port ]	*/
/*
bool is_Host(char* chaine)
{
    char* delimiteur = ":";
    char* token = strtok(chaine,delimiteur);
    if(is_uri_host(chaine)){
        return true;
    }
    if(token != NULL && is_port(token)){
        return true;
    }
    if(token == NULL){
        return false;
    }
}


bool is_OWS(char *chaine)
{
    int taille = strlen(chaine);
    if(taille == 0){
        return true;
    }
    for (int index = 0; index < taille ; index++) {
        if (!is_SP(chaine[index]) && !is_HTAB(chaine[index])) {
            return false;
        }
    }
    return true;
}

bool is_BWS(char* chaine)
{
    return is_OWS(chaine);
}
*/
/*Connection = *( "," OWS ) connection-option *( OWS "," [ OWS			
connection-option ] )
donc c'est un certain nombre de , OWS une connection option et un certains nombre du reste*/
/*
bool is_connection(char* chaine)
{
    int taille = strlen(chaine);
    int index = 0;
    return true;
    
}



bool is_RWS(char* chaine)
{
    int taille = strlen(chaine);
    if (taille == 0) {
        return false;
    }

    for (int i = 0; i < taille; i++) {
        if (!is_SP(chaine[i]) && !is_HTAB(chaine[i])) {
            return false;
        }
    }

    return true;
}
*/

/*comment = "(" *( ctext / quoted-pair / comment ) ")"		*/
/*
bool is_comment(char* chaine)
{
    return true;
}


bool is_connection_option(char* chaine)
{
    return is_token(chaine);
}
*/
/*https-URI = "https://" authority path-abempty [ "?" query ] [ "#"*/ 
/*
bool is_http_URI(char* chaine)
{
    if(strncmp(chaine, "http://", 7) != 0) {
        return false;
    }
    else {
        char* authority = chaine + 7;
        char *path_start = strchr(authority, '/');
        if (path_start == NULL) {
            path_start = authority + strlen(authority);
        }

        char *query_start = strchr(path_start, '?');
        if (query_start != NULL) {
            if (!is_query(query_start + 1)) { 
                return false; 
            }
            path_start = query_start;
        }
        char *fragment_start = strchr(path_start, '#');
        if (fragment_start != NULL) {
            if (!is_fragment(fragment_start + 1)) { 
                return false; 
            }
            path_start = fragment_start;
        }
        return true;
    }
}
bool is_https_URI(char* chaine)
{
    if(strncmp(chaine, "https://", 8) != 0) {
        return false;
    }
    else {
        char* authority = chaine + 7;
        char *path_start = strchr(authority, '/');
        if (path_start == NULL) {
            path_start = authority + strlen(authority);
        }

        char *query_start = strchr(path_start, '?');
        if (query_start != NULL) {
            if (!is_query(query_start + 1)) { 
                return false; 
            }
            path_start = query_start;
        }
        char *fragment_start = strchr(path_start, '#');
        if (fragment_start != NULL) {
            if (!is_fragment(fragment_start + 1)) { 
                return false; 
            }
            path_start = fragment_start;
        }
        return true;
    }
}
*/


/*Content-Length = 1*DIGIT	= au moins un digit*/
/*
bool is_content_length(char* chaine)
{
    int taille = strlen(chaine);
    if(taille == 0){
        return false;
    }
    for(int i = 0; i < taille; i++){
        if(!isdigit(chaine[i])){
            return false;
        }
    }
    return true;
}
bool is_TE(char* chaine)
{
    return true;
}
bool is_Trailer(char* chaine)
{
    return true;
}

bool is_Transfer_Encoding(char* chaine)
{
    return true;
}
bool is_transfer_extension(char* chaine)
{
    return true;
}
bool is_Accept(char* chaine)
{
    return true;
}
bool is_Accept_Charset(char* chaine)
{
    return true;
}
bool is_Accept_Encoding(char* chaine)
{
    return true;
}
bool is_Accept_Language(char* chaine)
{
    return true;
}
bool is_Allow(char* chaine)
{
    return true;
}
*/

/*
t-ranking = OWS ";" OWS "q=" rank			
tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /			
^ / "_" / "`" / "|" / "~" / DIGIT / ALPHA			
token = 1*tchar			
*/
/*
bool is_tchar(char c)
{
    // on fait les cas spéciaux dans un if
    if(isdigit(c) || isalpha(c) ||( c == "'")){
        return true;
    }
    else{
        switch(c){
        case('!'): return true;
        case('#'): return true;
        case('$'): return true;
        case('%'): return true;
        case('&'): return true;
        case('*'): return true;
        case('+'): return true;
        case('-'): return true;
        case('.'): return true;
        case('^'): return true;
        case('_'): return true;
        case('`'): return true;
        case('|'): return true;
        case('~'): return true;
        default: return false;
    }
    }
    
}
// token = suite de au moins un tchar
bool is_token(char* chaine)
{
    int taille = strlen(chaine);
    if(taille == 0){
        return false;
    }
    else{
        for(int i = 0; i < taille; i++){
            if(!is_tchar(chaine[i])){
                return false;
            }
        }
    }
    return true;
}
*/
/*trailer-part = *( header-field CRLF )		*/
/*
bool is_trailer_part(char* chaine)
{
    char* delimiteur = "\r\n";
    char* token = strtok(chaine,delimiteur);
    while(token != NULL){
        if(!is_header_field(token)){
            return false;
        }
        token = strtok(NULL,delimiteur);
    }
    return true;
}
*/

/*transfer-coding = "chunked" / "compress" / "deflate" / "gzip" /			
transfer-extension			
transfer-extension = token *( OWS ";" OWS transfer-parameter )			
transfer-parameter = token BWS "=" BWS ( token / quoted-string )			
*/
/*
bool is_transfer_coding(char* chaine)
{
    return(!strcmp(chaine,"chunked") || !strcmp(chaine,"compress") || !strcmp(chaine,"deflate") || !strcmp(chaine,"gzip") || is_transfer_extension(chaine));
}
*/
/*transfer-extension = token *( OWS ";" OWS transfer-parameter )			*/
/*
bool is_transfer_extension(char* chaine)
{
    if(!is_token(chaine[0])){
        return false;
    }
    // pas fini
    return true;

}
bool is_transfer_parameter(char* chaine)
{
    // pas fini
    return true;
}
*/





/*	ctext = HTAB / SP / %x21-27 ; '!'-'''			
	/ %x2A-5B ; '*'-'['			
	/ %x5D-7E ; ']'-'~'			
	/ obs-text			*/
/*
bool is_ctext(char c)
{
    return(c == '\t' || c == ' ' || (c >= 0x21 && c <= 0x27) || (c >= 0x2A && c <= 0x5B) || (c >= 0x5D && c <= 0x7E) ||  (c >= 0x80 && c <= 0xFF));
}
*/

/*partial-URI = relative-part [ "?" query ]*/
/*
bool is_partial_uri(char* chaine)
{
    char* recopie = strdup(chaine);
    char* delimiteur = "? \t\n";
    char* token = strtok(recopie,delimiteur);
    if(token == NULL || !is_relative_part(token)){
        return false;
    }
    token = strtok(NULL,delimiteur);
    if(token != NULL){
        if(!is_query(token)){ 
            free(recopie ); // a check parce que j'ai l'impression que ca vérifie pas que dans le deuxieme cas
            return false;       // on ai bien la relative part
        }
    }
    free(recopie);
    return true; 
}
*/



/*protocol = protocol-name [ "/" protocol-version ]		*/
/*
bool is_protocol(char* chaine)
{
    char* recopie = strdup(chaine);
    char* delimiteur = "/ \t\n";
    char* token = strtok(recopie,delimiteur);
    if(token == NULL || !is_protocol_name(token)){
        return false;
    }
    token = strtok(NULL,delimiteur);
    if(token != NULL){
        if(!is_protocole_version(token)){
            return false;
        }
    }
    free(recopie);
    return true; 

}
*/
/*protocol-name = token
protocol-version = token
pseudonym = token c'est les mêmes*/
/*
bool is_protocole_name(char* chaine)
{
    return is_token(chaine);
}
bool is_protocole_version(char* chaine)
{
    return is_token(chaine);
}
bool is_pseudonym(char* chaine)
{
    return is_token(chaine);
}
/*qdtext = HTAB / SP / "!" / %x23-5B ; '#'-'['		
/ %x5D-7E ; ']'-'~'		
/ obs-text		*/
/*
bool is_qd_text(char c)
{
    return (c == '\t' || c == ' ' || c == '!' || (c >= 0x23 && c <= 0x5B) || (c >= 0x5D && c <= 0x7E) || (c >= 0x80 && c <= 0xFF));
}
*/
/*quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )*/
/*
bool is_quoted_pair(char* chaine)
{
    if(&chaine[0] != "\\"){
        return false;
    }
    else{
        return(chaine[1] == '\t' || chaine[1] == ' ' ||(chaine[1] >= 0x33 && chaine[1] <= 0x126) ||(chaine[1] >= 0x80 && chaine[1] <= 0xFF));
    }
}
*/
/*quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE			*/
/*
bool is_quoted_string(char* chaine)
{
    int taille = strlen(chaine);
    if(chaine[0] != '"' || chaine[taille - 1] != '"'){
        return false;
    }
    for(int i = 1; i < taille -1; i++){
        if(!is_qd_text(chaine[i]) && !is_quoted_pair(&chaine[i])){
            return false;
        }
    }
    return true;

}
*/
/*received-by = ( uri-host [ ":" port ] ) / pseudonym*/
/*
bool is_received_by(char* chaine)
{
    if(is_pseudonym(chaine)){
        return true;
    }
    // même chose faut trouver avant le strtok comment je fais pour check que le premier
    else{
        char* recopie = strdup(chaine);
        char* partie1 = NULL;
        char* partie2 = NULL;
        char* delim = ":\t\n";
        char* token = strtok(recopie,delim);
        if(token == NULL || is_port(token)){
            return false;
        }
        strcpy(partie1,token);
        token = strtok(NULL,delim);
        if(token != NULL){
            strcpy(partie2,token);
        }
        free(recopie);
        return(is_uri_host(partie1) && is_port(partie2));
    }
    
    
}
*/
/*received-protocol = [ protocol-name "/" ] protocol-version*/
// on vérifie que si il y a un / avant on a un protocol-name et apres un protocol version
// si on a rien on vérifie qu'on a juste un protocol-version
// a vérifier parce que ca marche pas je pense
/*
bool is_received_protocol(char* chaine)
{
    char* separateur = "/\t\n";
    char* recopie = strdup(chaine);
    char* token = strtok(recopie,separateur);
    
    char* partie1 = NULL;
    char* partie2 = NULL;
    if(token == NULL){
        return is_protocole_version(chaine);
    }
    else{
        strcpy(partie1,token);
        token = strtok(NULL,separateur);
        if(token != NULL){
            strcpy(partie2,token);
        }
        free(recopie);
        return(is_protocole_name(partie1) && is_protocole_version(partie2));

    }
    
}


*/
/* rank = ( "0" [ "." *3DIGIT ] ) / ( "1" [ "." *3"0" ] )		*/
/*
bool is_rank(char* chaine)
{
    int taille = strlen(chaine);
    if(chaine[0] == '0') {
        if(taille == 1) {
            return true;
        } else {
            if(chaine[1] == '.') {
                for(int i = 2; i < taille; i += 3) {
                    if(i + 2 < taille && !isdigit(chaine[i]) || !isdigit(chaine[i+1]) || !isdigit(chaine[i+2])) {
                        return false;
                    }
                }
                return true;
            } else {
                return false;
            }
        }
    } else if(chaine[0] == '1') {
        if(taille == 1) {
            return true;
        } else {
            if(chaine[1] == '.') {
                for(int i = 2; i < taille; i += 3) {
                    if(i + 2 < taille && (chaine[i] != '0' || chaine[i+1] != '0' || chaine[i+2] != '0')) {
                        return false;
                    }
                }
                return true;
            } else {
                return false;
            }
        }
    } else {
        return false;
    }
}







*/
/*t-codings = "trailers" / ( transfer-coding [ t-ranking ] )*/
/*
bool is_t_codings(char* chaine)
{
    if(!strcmp(chaine,"trailers")){
        return true;
    }
    else{
        // mais peut y avoir transfer coding et t-ranking donc faut séparer les deux et vérifier
        char* delim = " ;";
        char* recopie = strdup(chaine);
        char* partie1 = malloc(sizeof(recopie) + 1);
        char* token = strtok(recopie,delim);
        if(token == NULL){
            free(recopie);
            free(partie1);
            return is_transfer_coding(recopie);
        }
        else{
            strcpy(partie1,token);
            token = strtok(NULL,delim);
            return(is_transfer_coding(partie1) && is_t_ranking(token));
        }

    }
}
*/
/*t-ranking = OWS ";" OWS "q=" rank		*/
/*
bool is_t_ranking(char* chaine)
{
    char *partie1 = NULL;
    char *partie2 = NULL;
    char *partie3 = NULL;

    char *recopie = strdup(chaine); 
    char *delim = ";";

    char *token = strtok(recopie, delim);
    if (token == NULL) {
        free(recopie);
        return false;
    }
    partie1 = strdup(token);

    token = strtok(NULL, delim);
    if (token == NULL) {
        free(partie1);
        free(recopie);
        return false;
    }
    partie2 = strdup(token);

    token = strtok(NULL, "");
    if (token != NULL) {
        partie3 = strdup(token);
    } else {
        partie3 = strdup(""); 
    }

    free(recopie);

    return (is_OWS(partie1) && is_OWS(partie2) && is_rank(partie3));
}

bool is_uri_host(char* chaine)
{
    return is_host(chaine);    
}

bool is_content_location(char *chaine)
{
    return (is_absolute_uri(chaine) ||is_partial_uri(chaine));
}

bool is_content_type(char* chaine)
{
    return (is_media_type(chaine));
}

bool is_date(char* chaine)
{
    return is_http_date(chaine);
}


bool is_expect(char *chaine){
    if(strcmp(chaine,"100-continue")==0)
        return true;
    else 
        return false;
}

bool is_from(){}

bool is_GMT(char *chaine){
    if(strcmp(chaine,"GMT")==0)
        return true;
    else 
        return false;
}

bool is_http_date(char *chaine){
    if(is_imf_fixdate(chaine) || is_obs_date(chaine))
        return true;
    else
        return false;
}

bool is_day_name(char *chaine){
    if(strcmp(chaine,"Mon")==0)
        return true;
    else if(strcmp(chaine,"Tue")==0)
        return true;
    else if(strcmp(chaine,"Wed")==0)
        return true;
    else if(strcmp(chaine,"Thu")==0)
        return true;
    else if(strcmp(chaine,"Fri")==0)
        return true;
    else if(strcmp(chaine,"Sat")==0)
        return true;
    else if(strcmp(chaine,"Sun")==0)
        return true;
    else 
        return false;
}

bool is_day_name_l(char *chaine){
    if(strcmp(chaine,"Monday")==0)
        return true;
    else if(strcmp(chaine,"Tuesday")==0)
        return true;
    else if(strcmp(chaine,"Wednesday")==0)
        return true;
    else if(strcmp(chaine,"Thursday")==0)
        return true;
    else if(strcmp(chaine,"Friday")==0)
        return true;
    else if(strcmp(chaine,"Saturday")==0)
        return true;
    else if(strcmp(chaine,"Sunday")==0)
        return true;
    else 
        return false;
}

bool is_delay_seconds(char *chaine){
    l = strlen(chaine);
    for(int i=0; i<l; i++){
        if(!=isdigit(chaine[i]))
            return false;
    }
    return true;
}

bool is_location(char *chaine){
    if(is_uri_reference(chaine))
        return true;
    else 
        return false;
}

bool is_max_forwards(char *chaine){
    l = strlen(chaine);
    for(int i=0; i<l; i++){
        if(!=isdigit(chaine[i]))
            return false;
    }
    return true;
    }

bool is_referer(char *chaine){
    if(is_absolute_uri(chaine) || is_partial_uri(chaine))
        return true;
    else 
        return false;
}

bool is_retry_after(char *chaine){
    if(is_http_date(chaine) || is_delay_seconds(chaine))
        return true;
    else
        return false;
}

bool is_server(){}

bool is_charset(char *chaine){
    if(is_token(chaine))
        return true;
    else 
        return false;
}

bool is_product_version(char *chaine){
    if(is_token(chaine))
        return true;
    else 
        return false;
}

bool is_content_coding(char *chaine){
    if(is_token(chaine))
        return true;
    else 
        return false;
}

bool is_subtype(char *chaine){
    if(is_token(chaine))
        return true;
    else 
        return false;
}

bool is_type(char *chaine){
    if(is_token(chaine))
        return true;
    else 
        return false;
}

bool is_codings(char *chaine){
    if(is_content_coding(chaine) || strcmp(chaine,'identity')==0 || strcmp(chaine,"*")==0)
        return true;
    else 
        return false;
}


bool is_date2(char *chaine){
    if(!=isdigit(chaine[0]) ||!=isdigit(chaine[1]))
        return false;
    else if(chaine[2]!='-')
        return false;
    else if(chaine[4]== 'J' && chaine[5]== 'a' && chaine[6] == 'n'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'F' && chaine[5]== 'e' && chaine[6] == 'b'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'M' && chaine[5]== 'a' && chaine[6] == 'r'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'A' && chaine[5]== 'p' && chaine[6] == 'r'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'M' && chaine[5]== 'a' && chaine[6] == 'y'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'J' && chaine[5]== 'u' && chaine[6] == 'n'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'J' && chaine[5]== 'u' && chaine[6] == 'l'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'A' && chaine[5]== 'u' && chaine[6] == 'g'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'S' && chaine[5]== 'e' && chaine[6] == 'p'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'O' && chaine[5]== 'c' && chaine[6] == 't'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'N' && chaine[5]== 'o' && chaine[6] == 'v'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    else if(chaine[4]== 'D' && chaine[5]== 'e' && chaine[6] == 'c'){
        if(chaine[7]!='-')
            return false;
        else if(isdigit(chaine[8]) && isdigit(chaine[9]))
            return true;
        else
            return false;
    }
    return false;
}


bool is_date1(char *chaine){
    else if(chaine[2]!=' ' && chaine[2]!= '\t' && chaine[2]!='\n')
}

bool is_day(char *chaine){
    int len=strlen(chaine);
    if(len==2 && isdigit(chaine[0]) && isdigit(chaine[1]))
        return true;
    else 
        return false;
}

bool is_hour(char *chaine){
    int len=strlen(chaine);
    if(len==2 && isdigit(chaine[0]) && isdigit(chaine[1]))
        return true;
    else 
        return false;
}

bool is_minute(char *chaine){
    int len=strlen(chaine);
    if(len==2 && isdigit(chaine[0]) && isdigit(chaine[1]))
        return true;
    else 
        return false;
}

bool is_second(char *chaine){
    int len=strlen(chaine);
    if(len==2 && isdigit(chaine[0]) && isdigit(chaine[1]))
        return true;
    else 
        return false;
}

bool is_year(char *chaine){
    int len=strlen(chaine);
    if(len==4 && isdigit(chaine[0]) && isdigit(chaine[1]) && isdigit(chaine[2]) && isdigit(chaine[3]))
        return true;
    else 
        return false;
}

bool is_month(char *chaine){
    if(strcmp(chaine,"Jan")==0)
        return true;
    else if(strcmp(chaine,"Feb")==0)
        return true;
    else if(strcmp(chaine,"Mar")==0)
        return true;
    else if(strcmp(chaine,"Apr")==0)
        return true;
    else if(strcmp(chaine,"May")==0)
        return true;
    else if(strcmp(chaine,"Jun")==0)
        return true;
    else if(strcmp(chaine,"Jul")==0)
        return true;
    else if(strcmp(chaine,"Aug")==0)
        return true;
    else if(strcmp(chaine,"Sep")==0)
        return true;
    else if(strcmp(chaine,"Oct")==0)
        return true;
    else if(strcmp(chaine,"Nov")==0)
        return true;
    else if(strcmp(chaine,"Dec")==0)
        return true;
    else
        return false;
}

bool is_obs_date(char *chaine){
    if(is_rfc850_date(chaine) || is_asctime_date(chaine))
        return true;
}

bool is_etag(char *chaine){
    if(is_entity_tag(chaine))
        return true;
    else 
        return false;
}

bool is_last_modified(char *chaine){
    if(is_http_date(chaine))
        return true;
    else 
        return false;
}

bool is_accept_ranges(char *chaine){
    if(is_acceptable_ranges(chaine))
        return true;
    else 
        return false;
}

bool is_if_range(char *chaine){
    if(is_entity_tag(chaine) || is_http_date(chaine))
        return true;
    else 
        return false;
}

bool is_range(char *chaine){
    if(is_byte_ranges_specifier(chaine) || is_other_ranges_specifier(chaine))
        return true;
    else 
        return false;
}

bool is_range_unit(char *chaine){
    if(is_bytes_unit(chaine) || is_other_range_unit(chaine))
        return true;
    else 
        return false;
}

bool is_bytes_unit(char *chaine){
    if(strcmp(chaine,"bytes")==0)
        return true;
    else
        return false;
}

bool is_complete_length(char *chaine){
    l = strlen(chaine);
    for(int i=0; i<l; i++){
        if(!=isdigit(chaine[i]))
            return false;
    }
    return true;
}

bool is_first_byte_pos(char *chaine){
    l = strlen(chaine);
    for(int i=0; i<l; i++){
        if(!=isdigit(chaine[i]))
            return false;
    }
    return true;
}

bool is_last_byte_pos(char *chaine){
    l = strlen(chaine);
    for(int i=0; i<l; i++){
        if(!=isdigit(chaine[i]))
            return false;
    }
    return true;
}

bool is_suffix_length(char *chaine){
    l = strlen(chaine);
    for(int i=0; i<l; i++){
        if(!=isdigit(chaine[i]))
            return false;
    }
    return true;
}

bool is_suffix_byte_range_spec(char *chaine){
    if(chaine[0]!="-")
        return false;
    else{
        char delim= "-";
        char *tok = strtok(chaine, delim);
        if(is_suffix_length(tok))
            return true;
    }
}

bool is_other_range_unit(char *chaine){
    if(is_token(chaine))
        return true;
    else 
        return false;
}

bool is_other_range_resp(char *chaine){
    len = strlen(chaine);
    int i =0;
    char debut = 0x00;
    char fin = 0x7F;
    while(i<len){
        if(chaine[i]>=debut && chaine[i]<=fin)
            i++;
        else
            return false;
    }
    return true;
}

bool is_other_range_set(char *chaine){
    len = strlen(chaine);
    int i =0;
    char debut = 0x00;
    char fin = 0x7F;
    while(i<len){
        if(chaine[i]>=debut && chaine[i]<=fin)
            i++;
        else
            return false;
    }
    return true;
}

bool is_other_content_range(char *chaine){
    char *token = strtok(chaine, " \t\n-_");
    if(!=is_other_range_unit(token))
        return false;
    token = strtok(NULL, " \t\n-_");
    if(!=is_other_range_resp(token))
        return false;
    else
        return true;
}

bool is_other_range_specifier(char *chaine){
    char *token = strtok(chaine, "=");
    if(!=is_other_range_unit(token))
        return false;
    token = strtok(NULL, "=");
    if(!=is_other_range_set(token))
        return false;
    else
        return true;
}

bool is_weak(char *chaine){
    len=strlen(chaine);
    if(len==1 && chaine[0]=='W')
        return true;
    else 
        return false;
}

bool is_unsatified_range(char *str)
{
    while (*str == '/')
    {
        str++;
    }
    return is_complete_length(str);
}

bool is_age(char c)
{
    return (is_delta_seconds(c));
}

bool is_Cache_Control(char *str)
{
    char *directive = strtok(str, ",");
    while (*directive != NULL)
    {
        while (*directive == ' ')
        {
            directive++;
        }
        char *end = directive + strlen(directive) - 1;
        while (end > directive && *end == ' ')
        {
            end--;
        }

        if (is_cache_directive(directive))
        {
            return false;
        }

        directive = strtok(NULL, ",");
    }
    return true;
}
bool is_Expires(char *chaine)
{
    return (is_HTTP_date(chaine));
}

bool is_Pragma(char *str)
{
    char *directive = strtok(str, ",");
    while (*directive != NULL)
    {
        while (*directive == ' ')
        {
            directive++;
        }
        char *end = directive + strlen(directive) - 1;
        while (end > directive && *end == ' ')
        {
            end--;
        }

        if (is_pragma_directive(directive))
        {
            return false;
        }

        directive = strtok(NULL, ",");
    }
    return true;
}
bool is_Warning(char *str)
{
    char *directive = strtok(str, ",");
    while (*directive != NULL)
    {
        while (*directive == ' ')
        {
            directive++;
        }
        char *end = directive + strlen(directive) - 1;
        while (end > directive && *end == ' ')
        {
            end--;
        }

        if (is_warning_value(directive))
        {
            return false;
        }

        directive = strtok(NULL, ",");
    }
    return true;
}
bool is_cache_directive(char *str)
{
    // On recupere le premier token
    char *token = strtok(str, "=");
    if (token == NULL || !is_token(token))
    {
        return false;
    }

    token = strtok(NULL, "=");
    if (token != NULL && !is_token(token) && !is_quoted_string(token))
    {
        return false;
    }

    return true;
}

bool is_delta_seconds(char c)
{
    return (isdigit(c));
}

bool is_extension_pragma(char *str)
{
    // On recupere le premier token
    char *token = strtok(str, "=");
    if (token == NULL || !is_token(token))
    {
        return false;
    }
    // Commence bien par un token valide

    token = strtok(NULL, "=");
    if (token != NULL && !is_token(token) && !is_quoted_string(token))
    {
        return false;
    }

    return true;
}

bool is_pragma_directive(char *str)
{
    if ((strncmp(str, "no-cache", 8) == 0) || is_extension_pragma(str) == true)
    {
        return true;
    }
    return false;
}
// warn-agent = ( uri-host [ ":" port ] ) / pseudonym
bool is_warn_agent(char *str)
{

    char *uri_host = strtok(str, ":");
    if (uri_host == NULL || !is_uri_host(uri_host))
    {
        return is_pseudonym(str); // Cas ou c'est un pseudonyme
    }

    char *port = strtok(NULL, ":");
    if (port != NULL && !is_port(port))
    {
        return false;
    }

    return true;
}
bool is_warn_code(char *chaine)
{
    return (isdigit(chaine[0]) && isdigit(chaine[1]) && isdigit(chaine[2]));
}
bool is_warn_date(char *chaine)
{
    if (strlen(chaine) < 2)
    {
        return false;
    }
    if (chaine[0] != '"' || chaine[strlen(chaine) - 1] != '"')
    {
        return false;
    }
    char *date = strndup(chaine + 1, strlen(chaine) - 2); // On enlève les guillemets

    bool result = is_HTTP_date(date);
    free(date);

    return result;
}
bool is_warn_text(char *str)
{
    return is_quoted_string(str);
}
// warning-value = warn-code SP warn-agent SP warn-text [ SP warn-date ]
bool is_warning_value(char *str)
{
    char *warn_code = strtok(str, " ");
    if (warn_code == NULL || !is_warn_code(warn_code))
    {
        return false;
    }

    char *warn_agent = strtok(NULL, " ");
    if (warn_agent == NULL || !is_warn_agent(warn_agent))
    {
        return false;
    }

    char *warn_text = strtok(NULL, " ");
    if (warn_text == NULL || !is_warn_text(warn_text))
    {
        return false;
    }

    char *warn_date = strtok(NULL, " ");
    if (warn_date != NULL && !is_warn_date(warn_date)) // Cas ou on quelque chose ensuite mais c'est pas une W-date
    {
        return false;
    }

    return true;
}
bool is_Proxy_Authenticate(char *str)
{
    char *directive = strtok(str, ",");
    while (*directive != NULL)
    {
        while (*directive == ' ')
        {
            directive++;
        }
        char *end = directive + strlen(directive) - 1;
        while (end > directive && *end == ' ')
        {
            end--;
        }

        if (is_challenge(directive))
        {
            return false;
        }

        directive = strtok(NULL, ",");
    }
    return true;
}

bool is_Proxy_Authorization(char *str)
{
    return is_credentials(str);
}
bool is_WWW_Authenticate(char *str)
{
    char *directive = strtok(str, ",");
    while (*directive != NULL)
    {
        while (*directive == ' ')
        {
            directive++;
        }
        char *end = directive + strlen(directive) - 1;
        while (end > directive && *end == ' ')
        {
            end--;
        }

        if (is_challenge(directive))
        {
            return false;
        }

        directive = strtok(NULL, ",");
    }
    return true;
}
// auth-param = token BWS "=" BWS ( token / quoted-string )
bool is_auth_param(char *str)
{
    return is_tranfer_parameter(*str);
}

bool is_auth_scheme(char c)
{
    return (is_token(c));
}
// challenge = auth-scheme [ 1*SP ( token68 / [ ( "," / auth-param ) *( OWS "," [ OWS auth-param ] ) ] ) ]
bool is_challenge(char *str)
{
    char *auth_scheme = strtok(str, " ");
    if (auth_scheme == NULL || !is_auth_scheme(auth_scheme))
    {
        return false;
    }

    char *token68_or_params = strtok(NULL, " ");
    if (token68_or_params != NULL)
    {
        if (is_token68(token68_or_params))
        {
            return true;
        }
        else
        {
            char *auth_param = strtok(token68_or_params, ",");
            while (auth_param != NULL)
            {
                if (!is_auth_param(auth_param))
                {
                    return false;
                }
                auth_param = strtok(NULL, ",");
            }
        }
    }

    return true;
}
// credentials = auth-scheme [ 1*SP ( token68 / [ ( "," / auth-param ) *( OWS "," [ OWS auth-param ] ) ] ) ]
bool is_credentials(char *str)
{

    char *auth_scheme = strtok(str, " ");
    if (auth_scheme == NULL || !is_auth_scheme(auth_scheme))
    {
        return false;
    }

    char *token68_or_params = strtok(NULL, " ");
    if (token68_or_params != NULL)
    {
        if (is_token68(token68_or_params))
        {
            return true;
        }
        else
        {
            char *auth_param = strtok(token68_or_params, ",");
            while (auth_param != NULL)
            {
                if (!is_auth_param(auth_param))
                {
                    return false;
                }
                auth_param = strtok(NULL, ",");
            }
        }
    }

    return true;
}

bool is_Authorization(char *str)
{
    return is_credentials(str);
}
bool is_token68(char *str)
{
    int len = strlen(str);
    int i = 0;

    // Vérifier les caractères alphanumériques et spéciaux
    for (; i < len; i++)
    {
        if (!isalnum(str[i]) && str[i] != '-' && str[i] != '.' && str[i] != '_' && str[i] != '~' && str[i] != '+' && str[i] != '/')
        {
            break;
        }
    }

    // Vérifier les caractères égal
    for (; i < len; i++)
    {
        if (str[i] != '=')
        {
            return false;
        }
    }

    return true;
}
bool is_Connection_header(char *str)
{

    if (strncmp(str, "Connection:", 11) != 0)
    {
        return false;
    }

    char *connection = str + 11;
    while (*connection == ' ')
    {
        connection++;
    }

    bool is_connection = is_Connection(connection);

    char *end = connection + strlen(connection) - 1;
    while (end > connection && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != connection + strlen(connection) - 1;

    return is_connection && !ends_with_whitespace;
}
bool is_Content_Length_header(char *str)
{
    if (strncmp(str, "Content-Length:", 15) != 0)
    {
        return false;
    }

    char *content_length = str + 15;
    while (*content_length == ' ')
    {
        content_length++;
    }

    bool is_content_length = is_Content_Length(content_length);

    char *end = content_length + strlen(content_length) - 1;
    while (end > content_length && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != content_length + strlen(content_length) - 1;

    return is_content_length && !ends_with_whitespace;
}
bool is_Content_Type_header(char *str)
{

    if (strncmp(str, "Content-Type:", 13) != 0)
    {
        return false;
    }

    char *content_type = str + 13;
    while (*content_type == ' ')
    {
        content_type++;
    }

    bool is_content_type = is_Content_Type(content_type);

    char *end = content_type + strlen(content_type) - 1;
    while (end > content_type && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != content_type + strlen(content_type) - 1;

    return is_content_type && !ends_with_whitespace;
}
bool is_Trailer_header(char *str)
{
    if (strncmp(str, "Trailer:", 8) != 0)
    {
        return false;
    }

    char *trailer = str + 8;
    while (*trailer == ' ')
    {
        trailer++;
    }

    bool is_trailer = is_Trailer(trailer);

    char *end = trailer + strlen(trailer) - 1;
    while (end > trailer && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != trailer + strlen(trailer) - 1;

    return is_trailer && !ends_with_whitespace;
}
bool is_Transfer_Encoding_header(char *str)
{
    if (strncmp(str, "Transfer-Encoding:", 18) != 0)
    {
        return false;
    }
    char *transfer_encoding = str + 18;
    while (*transfer_encoding == ' ')
    {
        transfer_encoding++;
    }

    bool is_transfer_encoding = is_Transfer_Encoding(transfer_encoding);

    char *end = transfer_encoding + strlen(transfer_encoding) - 1;

    while (end > transfer_encoding && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != transfer_encoding + strlen(transfer_encoding) - 1;

    return is_transfer_encoding && !ends_with_whitespace;
}
bool is_Upgrade_header(char *str)
{
    if (strncmp(str, "Upgrade:", 8) != 0)
    {
        return false;
    }

    char *upgrade = str + 8;
    while (*upgrade == ' ')
    {
        upgrade++;
    }

    bool is_upgrade = is_Upgrade(upgrade);

    char *end = upgrade + strlen(upgrade) - 1;
    while (end > upgrade && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != upgrade + strlen(upgrade) - 1;

    return is_upgrade && !ends_with_whitespace;
}
bool is_Via_header(char *str)
{
    if (strncmp(str, "Via:", 4) != 0)
    {
        return false;
    }

    char *via = str + 4;
    while (*via == ' ')
    {
        via++;
    }

    bool is_via = is_Via(via);

    char *end = via + strlen(via) - 1;
    while (end > via && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != via + strlen(via) - 1;

    return is_via && !ends_with_whitespace;
}
bool is_Age_header(char *str)
{

    if (strncmp(str, "Age:", 4) != 0)
    {
        return false;
    }

    char *age = str + 4;
    while (*age == ' ')
    {
        age++;
    }

    bool is_age = is_Age(age);

    char *end = age + strlen(age) - 1;
    while (end > age && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != age + strlen(age) - 1;

    return is_age && !ends_with_whitespace;
}
bool is_Expires_header(char *str)
{

    if (strncmp(str, "Expires:", 8) != 0)
    {
        return false;
    }

    char *expires = str + 8;
    while (*expires == ' ')
    {
        expires++;
    }

    bool is_expires = is_Expires(expires);

    char *end = expires + strlen(expires) - 1;
    while (end > expires && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != expires + strlen(expires) - 1;

    return is_expires && !ends_with_whitespace;
}
bool is_Date_header(char *str)
{

    if (strncmp(str, "Date:", 5) != 0)
    {
        return false;
    }

    char *date = str + 5;
    while (*date == ' ')
    {
        date++;
    }

    bool is_date = is_Date(date);

    char *end = date + strlen(date) - 1;
    while (end > date && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != date + strlen(date) - 1;

    return is_date && !ends_with_whitespace;
}
bool is_Location_header(char *str)
{

    if (stnrcmp(str, "Location:", 9) != 0)
    {
        return false;
    }

    char *location = str + 9;
    while (*location == ' ')
    {
        location++;
    }

    bool is_location = is_Location(location);

    char *end = location + strlen(location) - 1;
    while (end > location && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != location + strlen(location) - 1;

    return is_location && !ends_with_whitespace;
}
bool is_Retry_After_header(char *str)
{

    if (strncmp(str, "Retry-After:", 12) != 0)
    {
        return false;
    }

    char *retry_after = str + 12;
    while (*retry_after == ' ')
    {
        retry_after++;
    }

    bool is_retry_after = is_Retry_After(retry_after);

    char *end = retry_after + strlen(retry_after) - 1;
    while (end > retry_after && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != retry_after + strlen(retry_after) - 1;

    return is_retry_after && !ends_with_whitespace;
}
bool is_Vary_header(char *str)
{
    if (strncmp(str, "Vary:", 5) != 0)
    {
        return false;
    }

    char *vary = str + 5;
    while (*vary == ' ')
    {
        vary++;
    }

    bool is_vary = is_Vary(vary);

    char *end = vary + strlen(vary) - 1;
    while (end > vary && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != vary + strlen(vary) - 1;

    return is_vary && !ends_with_whitespace;
}
bool is_Warning_header(char *str)
{
    if (strncmp(str, "Warning:", 8) != 0)
    {
        return false;
    }

    char *warning = str + 8;
    while (*warning == ' ')
    {
        warning++;
    }

    bool is_warning = is_Warning(warning);

    char *end = warning + strlen(warning) - 1;
    while (end > warning && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != warning + strlen(warning) - 1;

    return is_warning && !ends_with_whitespace;
}
bool is_Cache_Control_header(char *str)
{
    if (strncmp(str, "Cache-Control:", 14) != 0)
    {
        return false;
    }

    char *cache_control = str + 14;
    while (*cache_control == ' ')
    {
        cache_control++;
    }

    bool is_cache_control = is_Cache_Control(cache_control);

    char *end = cache_control + strlen(cache_control) - 1;
    while (end > cache_control && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != cache_control + strlen(cache_control) - 1;

    return is_cache_control && !ends_with_whitespace;
}
bool is_Expect_header(char *str)
{

    if (strncmp(str, "Expect:", 7) != 0)
    {
        return false;
    }

    char *expect = str + 7;
    while (*expect == ' ')
    {
        expect++;
    }

    bool is_expect = is_Expect(expect);

    char *end = expect + strlen(expect) - 1;
    while (end > expect && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != expect + strlen(expect) - 1;

    return is_expect && !ends_with_whitespace;
}
bool is_Host_header(char *str)
{

    if (strncmp(str, "Host:", 5) != 0)
    {
        return false;
    }

    char *host = str + 5;
    while (*host == ' ')
    {
        host++;
    }

    bool is_host = is_Host(host);

    char *end = host + strlen(host) - 1;
    while (end > host && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != host + strlen(host) - 1;

    return is_host && !ends_with_whitespace;
}
bool is_Max_Forwards_header(char *str)
{

    if (strncmp(str, "Max-Forwards:", 13) != 0)
    {
        return false;
    }

    char *max_forwards = str + 13;
    while (*max_forwards == ' ')
    {
        max_forwards++;
    }

    bool is_max_forwards = is_Max_Forwards(max_forwards);

    char *end = max_forwards + strlen(max_forwards) - 1;
    while (end > max_forwards && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != max_forwards + strlen(max_forwards) - 1;

    return is_max_forwards && !ends_with_whitespace;
}
bool is_Pragma_header(char *str)
{

    if (strncmp(str, "Pragma:", 6) != 0)
    {
        return false;
    }

    char *pragma = str + 6;
    while (*pragma == ' ')
    {
        pragma++;
    }

    bool is_pragma = is_Pragma(pragma);

    char *end = pragma + strlen(pragma) - 1;
    while (end > pragma && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != pragma + strlen(pragma) - 1;

    return is_pragma && !ends_with_whitespace;
}
bool is_Range_header(char *str)
{

    if (strncmp(str, "Range:", 6) != 0)
    {
        return false;
    }

    char *range = str + 6;
    while (*range == ' ')
    {
        range++;
    }

    bool is_range = is_Range(range);

    char *end = range + strlen(range) - 1;
    while (end > range && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != range + strlen(range) - 1;

    return is_range && !ends_with_whitespace;
}
bool is_TE_header(char *str)
{

    if (strncmp(str, "TE:", 3) != 0)
    {
        return false;
    }

    char *te = str + 3;
    while (*te == ' ')
    {
        te++;
    }

    bool is_te = is_TE(te);

    char *end = te + strlen(te) - 1;
    while (end > te && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != te + strlen(te) - 1;

    return is_te && !ends_with_whitespace;
}
bool is_If_Match_header(char *str)
{
    if (strncmp(str, "If-Match:", 9) != 0)
    {
        return false;
    }

    char *if_match = str + 9;
    while (*if_match == ' ')
    {
        if_match++;
    }

    bool is_if_match = is_If_Match(if_match);

    char *end = if_match + strlen(if_match) - 1;
    while (end > if_match && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != if_match + strlen(if_match) - 1;

    return is_if_match && !ends_with_whitespace;
}
bool is_If_None_Match_header(char *str)
{
    if (strncmp(str, "If-None-Match:", 14) != 0)
    {
        return false;
    }

    char *if_none_match = str + 14;
    while (*if_none_match == ' ')
    {
        if_none_match++;
    }

    bool is_if_none_match = is_If_None_Match(if_none_match);

    char *end = if_none_match + strlen(if_none_match) - 1;
    while (end > if_none_match && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != if_none_match + strlen(if_none_match) - 1;

    return is_if_none_match && !ends_with_whitespace;
}
bool is_If_Modified_Since_header(char *str)
{
    if (strncmp(str, "If-Modified-Since:", 18) != 0)
    {
        return false;
    }

    char *if_modified_since = str + 18;
    while (*if_modified_since == ' ')
    {
        if_modified_since++;
    }

    bool is_if_modified_since = is_If_Modified_Since(if_modified_since);

    char *end = if_modified_since + strlen(if_modified_since) - 1;
    while (end > if_modified_since && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != if_modified_since + strlen(if_modified_since) - 1;

    return is_if_modified_since && !ends_with_whitespace;
}
bool is_If_Unmodified_Since_header(char *str)
{
    if (strncmp(str, "If-Unmodified-Since:", 20) != 0)
    {
        return false;
    }

    char *if_unmodified_since = str + 20;
    while (*if_unmodified_since == ' ')
    {
        if_unmodified_since++;
    }

    bool is_if_unmodified_since = is_If_Unmodified_Since(if_unmodified_since);

    char *end = if_unmodified_since + strlen(if_unmodified_since) - 1;
    while (end > if_unmodified_since && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != if_unmodified_since + strlen(if_unmodified_since) - 1;

    return is_if_unmodified_since && !ends_with_whitespace;
}
bool is_If_Range_header(char *str)
{

    if (strncmp(str, "If-Range:", 9) != 0)
    {
        return false;
    }

    char *if_range = str + 9;
    while (*if_range == ' ')
    {
        if_range++;
    }

    bool is_if_range = is_If_Range(if_range);

    char *end = if_range + strlen(if_range) - 1;
    while (end > if_range && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != if_range + strlen(if_range) - 1;

    return is_if_range && !ends_with_whitespace;
}
bool is_Accept_header(char *str)
{

    if (strncmp(str, "Accept:", 7) != 0)
    {
        return false;
    }

    char *accept = str + 7;
    while (*accept == ' ')
    {
        accept++;
    }

    bool is_accept = is_Accept(accept);

    char *end = accept + strlen(accept) - 1;
    while (end > accept && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != accept + strlen(accept) - 1;

    return is_accept && !ends_with_whitespace;
}
bool is_Accept_Charset_header(char *str)
{
    if (strncmp(str, "Accept-Charset:", 15) != 0)
    {
        return false;
    }

    char *accept = str + 15;
    while (*accept == ' ')
    {
        accept++;
    }

    bool is_accept = is_Accept_Charset(accept);

    char *end = accept + strlen(accept) - 1;
    while (end > accept && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != accept + strlen(accept) - 1;

    return is_accept && !ends_with_whitespace;
}
bool is_Accept_Encoding_header(char *str)
{

    if (strncmp(str, "Accept-Encoding:", 16) != 0)
    {
        return false;
    }

    char *accept = str + 16;

    while (*accept == ' ')
    {
        accept++;
    }

    bool is_accept = is_Accept_Encoding(accept);

    char *end = accept + strlen(accept) - 1;
    while (end > accept && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != accept + strlen(accept) - 1;

    return is_accept && !ends_with_whitespace;
}
bool is_Accept_Language_header(char *str)
{

    if (strncmp(str, "Accept-Language:", 16) != 0)
    {
        return false;
    }

    char *accept = str + 16;
    while (*accept == ' ')
    {
        accept++;
    }

    bool is_accept = is_Accept_Language(accept);

    char *end = accept + strlen(accept) - 1;
    while (end > accept && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != accept + strlen(accept) - 1;

    return is_accept && !ends_with_whitespace;
}
bool is_Authorization_header(char *str)
{
    if (strncmp(str, "Authorization:", 14) != 0)
    {
        return false;
    }

    // Ignore les espaces blancs optionnels après "Authorization
    char *auth = str + 14;
    while (*auth == ' ')
    {
        auth++;
    }

    // Vérifie si la chaîne restante est un Authorization valide
    bool is_auth = is_Authorization(auth);

    // Ignore les espaces blancs optionnels à la fin

    char *end = auth + strlen(auth) - 1;
    while (end > auth && *end == ' ')
    {
        end--;
    }

    // Vérifie si la chaîne se termine par un espace blanc

    bool ends_with_whitespace = end != auth + strlen(auth) - 1;

    return is_auth && !ends_with_whitespace;
}
bool is_Proxy_Authorization_header(char *str)
{
    // Vérifie si la chaîne commence par "Prox....:"
    if (strncmp(str, "Proxy-Authorization:", 20) != 0)
    {
        return false;
    }

    // Ignore les espaces blancs optionnels après "Proxyblalab:"
    char *prox = str + 20;
    while (*prox == ' ')
    {
        prox++;
    }

    // Vérifie si la chaîne restante est un prox valide
    bool is_prox = is_Proxy_Authorization(prox);

    // Ignore les espaces blancs optionnels à la fin
    char *end = prox + strlen(prox) - 1;
    while (end > prox && *end == ' ')
    {
        end--;
    }

    // Vérifie si la chaîne se termine par un espace blanc
    bool ends_with_whitespace = end != prox + strlen(prox) - 1;

    return is_prox && !ends_with_whitespace;
}
// From-header = "From" ":" OWS From OWS
bool is_From_header(char *str)
{

    // Vérifie si la chaîne commence par "From:"
    if (strncmp(str, "From:", 5) != 0)
    {
        return false;
    }

    // Ignore les espaces blancs optionnels après "From:"
    char *from = str + 5;
    while (*from == ' ')
    {
        from++;
    }

    // Vérifie si la chaîne restante est un Referer valide
    bool is_from = is_From(from);

    // Ignore les espaces blancs optionnels à la fin
    char *end = from + strlen(from) - 1;
    while (end > from && *end == ' ')
    {
        end--;
    }

    // Vérifie si la chaîne se termine par un espace blanc
    bool ends_with_whitespace = end != from + strlen(from) - 1;

    return is_from && !ends_with_whitespace;
}
// Referer-header = "Referer" ":" OWS Referer OWS
bool is_Referer_header(char *str)
{
    // Vérifie si la chaîne commence par "Referer:"
    if (strncmp(str, "Referer:", 8) != 0)
    {
        return false;
    }

    // Ignore les espaces blancs optionnels après "Referer:"
    char *referer = str + 8;
    while (*referer == ' ')
    {
        referer++;
    }

    // Vérifie si la chaîne restante est un Referer valide
    bool is_referer = is_Referer(referer);

    // Ignore les espaces blancs optionnels à la fin
    char *end = referer + strlen(referer) - 1;
    while (end > referer && *end == ' ')
    {
        end--;
    }

    // Vérifie si la chaîne se termine par un espace blanc
    bool ends_with_whitespace = end != referer + strlen(referer) - 1;

    return is_referer && !ends_with_whitespace;
}
bool is_User_Agent_header(char *str)
{

    if (strncmp(str, "User-Agent:", 11) != 0)
    {
        return false;
    }

    char *user_agent = str + 11;
    while (*user_agent == ' ')
    {
        user_agent++;
    }

    bool is_user_agent = is_User_Agent(user_agent);

    char *end = user_agent + strlen(user_agent) - 1;
    while (end > user_agent && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != user_agent + strlen(user_agent) - 1;

    return is_user_agent && !ends_with_whitespace;
}
// cookie-pair       = cookie-name "=" cookie-value
bool is_cookie_pair(char *str)
{

    int len = strlen(str);
    int i = 0;
    for (; i < len; i++)
    {
        if (str[i] == '=')
        {
            break;
        }
    }
    if (i == len)
    {
        return false;
    }
    char *name = strndup(str, i);
    char *value = strndup(str + i + 1, len - i - 1);

    bool result = is_cookie_name(name) && is_cookie_value(value);
    free(name);
    free(value);

    return result;
}
// cookie-name       = token
bool is_cookie_name(char *str)
{
    return (is_token(str));
}
// cookie-value      = ( DQUOTE *cookie-octet DQUOTE ) / *cookie-octet
bool is_cookie_value(char *str)
{
    int len = strlen(str);
    if (str[0] == '"' && str[len - 1] == '"')
    {
        for (int i = 1; i < len - 1; i++)
        {
            if (!is_cookie_octet(str[i]))
            {
                return false;
            }
        }
        return true;
    }
    for (int i = 0; i < len; i++)
    {
        if (!is_cookie_octet(str[i]))
        {
            return false;
        }
    }
    return true;
}
// cookie-octet      = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E
bool is_cookie_octet(char c)
{
    return isalnum(c) || c == '!' || c == '#' || c == '$' || c == '%' || c == '&' || c == "'" || c == '(' || c == ')' || c == ',' || c == '\'' || c == '*' || c == '+' || c == '-' || c == '.' || c == '^' || c == '_' || c == '`' || c == '|' || c == '~' || c == ':' || c == '/' || c == '?' || c == '@' || c == '[' || c == ']' || c == ';' || c == '{' || c == '}' || c == '=' || c == '<' || c == '>' || c == ';';
}
bool is_Cookie_header(char *str)
{

    if (strncmp(str, "Cookie:", 7) != 0)
    {
        return false;
    }

    char *cookie = str + 7;
    while (*cookie == ' ')
    {
        cookie++;
    }

    bool is_cookie = is_Cookie(cookie);

    char *end = cookie + strlen(cookie) - 1;
    while (end > cookie && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != cookie + strlen(cookie) - 1;

    return is_cookie && !ends_with_whitespace;
}
bool is_Cookie_string(char *str)
{
    char *cookie_paire = strtok(str, ";");
    while (cookie_paire != NULL)
    {
        while (cookie_paire == ' ')
        {
            cookie_paire++; // On avance le pointeur
        }
        if (!is_cookie_pair(cookie_paire))
        {
            return false;
        }
        cookie_paire = strtok(NULL, ";");
    }
    return true;
}
bool is_field_name_header(char *str)
{

    if (strncmp(str, "field-name:", 11) != 0)
    {
        return false;
    }

    char *accept = str + 11;

    while (*accept == ' ')
    {

        accept++;
    }

    bool is_accept = is_field_value(accept);

    char *end = accept + strlen(accept) - 1;
    while (end > accept && *end == ' ')
    {
        end--;
    }
    bool ends_with_whitespace = end != accept + strlen(accept) - 1;

    return is_accept && !ends_with_whitespace;
}

bool is_header_field(char *str)
{
    return is_Connection_header(str) || is_Content_Length_header(str) || is_Cookie_header(str) || is_Transfer_Encoding_header(str) || is_Expect_header(str) || is_Host_header(str) || is_Accept_header(str) || is_Accept_Charset_header(str) || is_Accept_Encoding_header(str) || is_Accept_Language_header(str) || is_Referer_header(str) || is_User_Agent_header(str) || is_field_name_header(str);
}
*/
