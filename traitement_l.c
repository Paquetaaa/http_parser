#include "redgrammm.h"
#include "traitement_l.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>




//REGLE DU PABZ 
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

//REGLE DE LUCAS

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
