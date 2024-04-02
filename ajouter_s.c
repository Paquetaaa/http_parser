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
    n = creer_noeud(RESERVED,"reserved",token,strlen(token))

else if(is_byte_range_resp(token))
    n = creer_noeud(RESERVED,"reserved",token,strlen(token));

else if(is_byte_range_spec(token))
    n = creer_noeud(RESERVED,"reserved",token,strlen(token));

else if(is_byte_range_specifier(token))
    n = creer_noeud(RESERVED,"reserved",token,strlen(token));

else if(is_byte_range_set(token))
    n = creer_noeud(RESERVED,"reserved",token,strlen(token));