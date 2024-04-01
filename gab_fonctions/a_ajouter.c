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

