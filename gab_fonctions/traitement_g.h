#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

#include "redgrammm.h"

/*mes fonctions, dans l'ordre de l'ABNF a peu près j'ai essayé*/

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
bool is_content_length(char* chaine);
bool is_tchar(char c);
bool is_token(char* chaine);
bool is_trailer_part(char* chaine);
bool is_transfer_coding(char* chaine);
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
