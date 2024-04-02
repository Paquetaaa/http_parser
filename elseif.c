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

