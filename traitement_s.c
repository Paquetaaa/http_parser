#include 'traitement_s.h'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>



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

bool is_user_agent(char *chaine){

}

bool is_vary(char *chaine){
    
}

bool is_accept_ext(char *chaine){
    
}

bool is_accept_params(char *chaine){
    
}

bool is_asctime_date(char *chaine){
    
}

bool is_date3(char *chaine){
    
}

bool is_media_range(char *chaine){
    
}

bool is_parameter(char *chaine){
    
}

bool is_product(char *chaine){
    
}

bool is_qvalue(char *chaine){
    
}

bool is_rfc850_date(char *chaine){
    
}

bool is_time_of_day(char *chaine){
    
}

bool is_weight(char *chaine){
    
}

bool is_if_match(char *chaine){
    
}

bool is_if_modified_since(char *chaine){
    
}

bool is_if_none_match(char *chaine){
    
}

bool is_unmodified_since(char *chaine){
    
}

bool is_entity_tag(char *chaine){
    
}

bool is_acceptable_ranges(char *chaine){
    
}

bool is_byte_content_range(char *chaine){
    
}

bool is_byte_range(char *chaine){
    
}

bool is_byte_range_resp(char *chaine){
    
}

bool is_byte_range_set(char *chaine){
    
}

bool is_byte_range_spec(char *chaine){
    
}

bool is_byte_range_specifier(char *chaine){
    
}

