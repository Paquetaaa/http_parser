#ifndef __PARSER__
#define __PARSER__

typedef enum{
    CRLF,
    HTTP_message,
    HTTP_name,
    HTTP_version,
    field_content,
    field_name,
    field_value,
    field_vchar,
    last_chunk,
    message_body,
    method,
    obs_fold,
    obs_text,
    origin_form,
    reason_phrase,
    request_line,
    request_target,
    start_line,
    status_code,
    status_line,
    URI,
    hier_part,
    URI_reference,
    absolute_URI,
    relative_ref,
    relative_part,
    scheme,
    absolute_form,
    absolute_path,
    asterisk_form,
    chunk,
    chunk_data,
    chunk_ex,
    chunk_ext_name,
    chunk_ext_val,
    chunk_size,
    chunk_body,
    authority,
    userinfo,
    host,
    port,
    IP_literal,
    IPvFuture,
    IPv6address,
    h16,
    ls32,
    IPv4address,
    dec_octet,
    reg_name,
    path,
    path_abempty,
    path_absolute,
    path_noscheme,
    path_rootless,
    path_empty,
    segment,
    segment_nz,
    segment_nz_nc,
    pchar,
    query,
    fragment,
    pct_encoded,
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
    header_field,

} TypeNoeud;

typedef struct node
{
    TypeNoeud type;         // Type du noeud
    char* etiquette;        // Etiquette du noeud
    char* pointeur_debut;   // Pointeur vers le debut de la string que represente le noeud 
    int longueur;           // Taille de la string representee par le noeud
    struct node* pere;      // 
    struct node* fils_aine; // 
    struct node* frere;     // 

} Noeud;

/* Routine permettant de creer l'arbre syntaxique de la requete contenue dans le fichier pris en parametre (retourne la racine) */
Noeud* construire_arbre(const char* file_pathname);

/* Routine traitant uniquement la start-line */
void traitement_start_line(Noeud* nd_sl);

/* Routine traitant toutes les requetes */
void traitement(Noeud* nd);

bool is_CRLF(char* chaine);
bool is_VCHAR(char c);
bool is_HTTP_message(char* chaine);
bool is_HTTP_name(char* chaine);
bool is_HTTP_version(char* chaine);
bool is_field_content(char* chaine);
bool is_field_value(char* chaine);
bool is_field_vachar(char c);
bool is_last_chunk(char* chaine);
bool is_message_body(char* chaine);
bool is_method(char* chaine);
bool is_obs_fold(char* chaine);
bool is_obs_text(char* chaine);
bool is_origin_form(char* chaine);
bool is_reason_phrase(char* chaine);
bool is_request_line(char* chaine);
bool is_request_target(char* chaine);
bool is_start_line(char* chaine);
bool is_status_code(char* chaine);
bool is_status_line(char* chaine);
bool is_URI(char* chaine);
bool is_hier_part(char* chaine);
bool is_URI_reference(char* chaine);
bool is_absolute_URI(char* chaine);
bool is_relative_URI(char* chaine);
bool is_relative_part(char* chaine);
bool is_scheme(char* chaine);
bool is_absolute_form(char* chaine);
bool is_absolute_path(char* chaine);
bool is_asterisk_form(char c);
bool is_authority_form(char* chaine);
bool is_chunk(char* chaine);
bool is_chunk_data(char* chaine);
bool is_chunk_ext(char* chaine);
bool is_chunk_ext_name(char* chaine);
bool is_chunk_ext_val(char* chaine);
bool is_chunk_size(char* chaine);
bool is_chunked_body(char* chaine);
bool is_authority(char* chaine);
bool is_userinfo(char* chaine);
bool is_host(char* chaine);
bool is_port(char* chaine);
bool is_IP_litteral(char* chaine);
bool is_IPvFuture(char* chaine);
bool is_IPv6address(char* chaine);
bool is_h16(char* chaine);
bool is_ls32(char* chaine);
bool is_IPv4address(char* chaine);
bool is_dec_octet(char* chaine);
bool is_reg_name(char* chaine);
bool is_path(char* chaine);
bool is_path_abempty(char* chaine);
bool is_path_absolute(char* chaine);
bool is_path_nosheme(char* chaine);
bool is_path_rootless(char* chaine);
bool is_path_empty(char* chaine);
bool is_segment(char* chaine);
bool is_segment_nz(char* chaine);
bool is_segment_nz_nc(char* chaine);
bool is_pchar(char* chaine);
bool is_query(char* chaine);
bool is_fragment(char* chaine);
bool is_pct_encoded(char* chaine);
bool is_unreserved(char c);
bool is_gen_delims(char c);
bool is_sub_delims(char c);
bool is_reserved(char c);
bool is_alphanum(char c);
bool is_language_range(char *chaine);
bool is_language(char* chaine);
bool is_langtag(char* chaine);
bool is_language_tag(char* chaine);
bool is_extlang(char* chaine);
bool is_script(char* chaine);
bool is_region(char *chaine);
bool is_variant(char *chaine);
bool is_extension(char *chaine);
bool is_singleton(char c);
bool is_privateuse(char* chaine);
bool is_grandfathered(char * chaine);
bool is_irregular(char* chaine);
bool is_regular(char* chaine);
bool is_SP(char c);
bool is_HTAB(char c);
bool is_Host(char* chaine);
bool is_OWS(char *chaine);
bool is_BWS(char* chaine);
bool is_connection(char* chaine);
bool is_RWS(char* chaine);
bool is_comment(char* chaine);
bool is_connection_option(char* chaine);
bool is_http_URI(char* chaine);
bool is_https_URI(char* chaine);
bool is_content_length(char* chaine);
bool is_tchar(char c);
bool is_token(char* chaine);
bool is_trailer_part(char* chaine);
bool is_transfer_coding(char* chaine);
bool is_transfer_extension(char* chaine);
bool is_transfer_parameter(char* chaine);
bool is_ctext(char c);
bool is_partial_uri(char* chaine);
bool is_protocol(char* chaine);
bool is_protocole_name(char* chaine);
bool is_protocole_version(char* chaine);
bool is_pseudonym(char* chaine);
bool is_qd_text(char c);
bool is_quoted_pair(char* chaine);
bool is_quoted_string(char* chaine);
bool is_received_by(char* chaine);
bool is_received_protocol(char* chaine);
bool is_rank(char* chaine);
bool is_t_codings(char* chaine);
bool is_t_ranking(char* chaine);
bool is_uri_host(char* chaine);
bool is_content_location(char *chaine);
bool is_content_type(char* chaine);
bool is_date(char* chaine);
bool is_TE(char* chaine);
bool is_Trailer(char* chaine);
bool is_Transfer_Encoding(char* chaine);
bool is_transfer_extension(char* chaine);
bool is_Accept(char* chaine);
bool is_Accept_Charset(char* chaine);
bool is_Accept_Encoding(char* chaine);
bool is_Accept_Language(char* chaine);
bool is_Allow(char* chaine);
bool is_expect(char *chaine);
bool is_from();
bool is_GMT(char *chaine);
bool is_http_date(char *chaine);
bool is_day_name(char *chaine);
bool is_delay_seconds(char *chaine);
bool is_location(char *chaine);
bool is_max_forwards(char *chaine);
bool is_referer(char *chaine);
bool is_retry_after(char *chaine);
bool is_server();
bool is_charset(char *chaine);
bool is_product_version(char *chaine);
bool is_content_coding(char *chaine);
bool is_subtype(char *chaine);
bool is_type(char *chaine);
bool is_codings(char *chaine);
bool is_date2(char *chaine);
bool is_date1(char *chaine);
bool is_day(char *chaine);
bool is_hour(char *chaine);
bool is_minute(char *chaine);
bool is_second(char *chaine);
bool is_year(char *chaine);
bool is_month(char *chaine);
bool is_obs_date(char *chaine);
bool is_etag(char *chaine);
bool is_last_modified(char *chaine);
bool is_accept_ranges(char *chaine);
bool is_if_range(char *chaine);
bool is_range(char *chaine);
bool is_range_unit(char *chaine);
bool is_bytes_unit(char *chaine);
bool is_complete_length(char *chaine);
bool is_first_byte_pos(char *chaine);
bool is_last_byte_pos(char *chaine);
bool is_suffix_length(char *chaine);
bool is_suffix_byte_range_spec(char *chaine);
bool is_other_range_unit(char *chaine);
bool is_other_range_resp(char *chaine);
bool is_other_range_set(char *chaine);
bool is_other_content_range(char *chaine);
bool is_other_range_specifier(char *chaine);
bool is_weak(char *chaine);
bool is_user_agent(char *chaine);
bool is_vary(char *chaine);
bool is_accept_ext(char *chaine);
bool is_accept_params(char *chaine);
bool is_asctime_date(char *chaine);
bool is_date3(char *chaine);
bool is_media_range(char *chaine);
bool is_parameter(char *chaine);
bool is_product(char *chaine);
bool is_qvalue(char *chaine);
bool is_rfc850_date(char *chaine);
bool is_time_of_day(char *chaine);
bool is_weight(char *chaine);
bool is_if_match(char *chaine);
bool is_if_modified_since(char *chaine);
bool is_if_none_match(char *chaine);
bool is_unmodified_since(char *chaine);
bool is_entity_tag(char *chaine);
bool is_acceptable_ranges(char *chaine);
bool is_byte_content_range(char *chaine);
bool is_byte_range(char *chaine);
bool is_byte_range_resp(char *chaine);
bool is_byte_range_set(char *chaine);
bool is_byte_range_spec(char *chaine);
bool is_byte_range_specifier(char *chaine);
bool is_unsatified_range(char *str);
bool is_age(char c);
bool is_Cache_Control(char *str);
bool is_Expires(char *str);
bool is_Pragma(char *str);
bool is_Warning(char *str);
bool is_cache_directive(char *str);
bool is_delta_seconds(char c);
bool is_extension_pragma(char *str);
bool is_pragma_directive(char *str);
bool is_warn_agent(char *str);
bool is_warn_code(char *str);
bool is_warn_date(char *str);
bool is_warn_text(char *str);
bool is_warning_value(char *str);
bool is_Proxy_Authenticate(char *str);
bool is_Proxy_Authorization(char *str);
bool is_WWW_Authenticate(char *str);
bool is_auth_param(char *str);
bool is_auth_scheme(char c);
bool is_challenge(char *str);
bool is_credentials(char *str);
bool is_Authorization(char *str);
bool is_token68(char *str);
bool is_Connection_header(char *str);
bool is_Content_Length_header(char *str);
bool is_Content_Type_header(char *str);
bool is_Trailer_header(char *str);
bool is_Transfer_Encoding_header(char *str);
bool is_Upgrade_header(char *str);
bool is_Via_header(char *str);
bool is_Age_header(char *str);
bool is_Expires_header(char *str);
bool is_Date_header(char *str);
bool is_Location_header(char *str);
bool is_Retry_After_header(char *str);
bool is_Vary_header(char *str);
bool is_Warning_header(char *str);
bool is_Cache_Control_header(char *str);
bool is_Expect_header(char *str);
bool is_Host_header(char *str);
bool is_Max_Forwards_header(char *str);
bool is_Pragma_header(char *str);
bool is_Range_header(char *str);
bool is_TE_header(char *str);
bool is_If_Match_header(char *str);
bool is_If_None_Match_header(char *str);
bool is_If_Modified_Since_header(char *str);
bool is_If_Unmodified_Since_header(char *str);
bool is_If_Range_header(char *str);
bool is_Accept_header(char *str);
bool is_Accept_Charset_header(char *str);
bool is_Accept_Encoding_header(char *str);
bool is_Accept_Language_header(char *str);
bool is_Authorization_header(char *str);
bool is_Proxy_Authorization_header(char *str);
bool is_From_header(char *str);
bool is_Referer_header(char *str);
bool is_User_Agent_header(char *str);
bool is_cookie_name(char *str);
bool is_cookie_value(char *str);
bool is_cookie_octet(char c);
bool is_Cookie_header(char *str);
bool is_Cookie_string(char *str);
bool is_header_field(char *str);
bool is_generic_header(char *str);
bool is_field_name_header(char *str);

#endif



