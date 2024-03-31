#include "traitement_g.h"

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

/*language      = 2*3ALPHA            ; shortest ISO 639 code			
["-" extlang]       ; sometimes followed by			
; extended language subtags			
entre deux et trois alpha et peut être un extlang*/
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



}



/*
Language-Tag  = langtag             ; normal language tags
               / privateuse          ; private use tag
               / grandfathered       ; grandfathered tags
*/
bool is_language_tag(char* chaine)
{
    return (is_langtag(chaine) || is_privateuse(chaine) || is_grandfathered(chaine));
}


/*
extlang       = 3ALPHA              ; selected ISO 639 codes
                 *2("-" 3ALPHA)      ; permanently reserved*/
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

/* script        = 4ALPHA              ; ISO 15924 code*/
// on vérifie juste que la chaine est de longueur 4 et que ce sont bien des alpha
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


/*region        = 2ALPHA              ; ISO 3166-1 code
               / 3DIGIT              ; UN M.49 code*/
// faut vérifier que c'est 2alpha ou 3 digits
bool is_region(char *chaine)
{
    int taille = strlen(chaine);
    if(taille == 2){
        return (isalpha(chaine[0]) && isalpha[chaine[1]]);
    } 
    else if(taille == 3){
        return (isdigit(chaine[0]) && isdigit(chaine[1]) && isdigit(chaine[3]));
    }
    else{
        return false;
    }
}

/*variant       = 5*8alphanum         ; registered variants		
/ (DIGIT 3alphanum)		
	donc les possibilités sont entre 5 et 8 alpha num ou un digit et 3 alphanum	*/
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
    else if(taille == 4 && isidigit(chaine[0])){
        for(int i = 1; i < taille; i++){
            if(!isalnum(chaine[i])){
                return false;
            }
        }
        return true;
        
    }
    return false;
    
}
/*extension     = singleton 1*("-" (2*8alphanum))		*/
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


/*
singleton     = DIGIT               ; 0 - 9
               / %x41-57             ; A - W
               / %x59-5A             ; Y - Z
               / %x61-77             ; a - w
               / %x79-7A             ; y - z
*/
bool is_singleton(char c){
    return (is_number(c)|| (c >= 'A' && c <= 'W') || (c >= 'Y' && c <= 'Z') || (c >= 'a' && c <= 'w') || (c >= 'y' && c <= 'z')); 
}
/*privateuse    = "x" 1*("-" (1*8alphanum))			
donc c'est un x et rien du tout
ou un x et un certain nombre d'enchainement de tiret et entre 1 et 8 alphanum
donc on fait plusieurs cas*/
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
/*

grandfathered = irregular           ; non-redundant tags registered
               / regular             ; during the RFC 3066 era
*/
bool is_grandfathered(char * chaine)
{
    return (is_irregular(chaine) || is_regular(chaine));
}

/*pour regular et irregular c'est juste des grands cases*/
// en fait non faut changer et faire des strcmp sad
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

/*OWS = *( SP / HTAB )
donc c'est un sp, un htab ou pleins*/
bool is_SP(char c)
{
    return c == ' ';
}
bool is_HTAB(char c)
{
    return c == '\t';
}

/*Host = uri-host [ ":" port ]	*/
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

/*Connection = *( "," OWS ) connection-option *( OWS "," [ OWS			
connection-option ] )
donc c'est un certain nombre de , OWS une connection option et un certains nombre du reste*/
bool is_connection(char* chaine)
{
    int taille = strlen(chaine);
    int index = 0;
    
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

/*comment = "(" *( ctext / quoted-pair / comment ) ")"		*/
bool is_comment(char* chaine)
{

}


bool is_connection_option(char* chaine)
{
    return is_token(chaine);
}





/*Content-Length = 1*DIGIT	= au moins un digit*/
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

/*
t-ranking = OWS ";" OWS "q=" rank			
tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." /			
^ / "_" / "`" / "|" / "~" / DIGIT / ALPHA			
token = 1*tchar			
*/

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

/*trailer-part = *( header-field CRLF )		*/
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

/*transfer-coding = "chunked" / "compress" / "deflate" / "gzip" /			
transfer-extension			
transfer-extension = token *( OWS ";" OWS transfer-parameter )			
transfer-parameter = token BWS "=" BWS ( token / quoted-string )			
*/
bool is_transfer_coding(char* chaine)
{
    return(!strcmp(chaine,"chunked") || !strcmp(chaine,"compress") || !strcmp(chaine,"deflate") || !strcmp(chaine,"gzip") || is_transfer_extension(chaine));
}
bool is_transfer_extension(char* chaine)
{
    if(!is_token(chaine[0])){
        return false;
    }
    // pas fini

}
bool is_transfer_parameter(char* chaine)
{
    // pas fini
}






/*	ctext = HTAB / SP / %x21-27 ; '!'-'''			
	/ %x2A-5B ; '*'-'['			
	/ %x5D-7E ; ']'-'~'			
	/ obs-text			*/

bool is_ctext(char c)
{
    return(c == '\t' || c == ' ' || (c >= 0x21 && c <= 0x27) || (c >= 0x2A && c <= 0x5B) || (c >= 0x5D && c <= 0x7E) ||  (c >= 0x80 && c <= 0xFF));
}


/*partial-URI = relative-part [ "?" query ]*/
bool is_partial_uri(char* chaine)
{
     char* delimiteur = "? \t\n";
    char* token = strtok(chaine,delimiteur);
    if(token == NULL || !is_relative_part(token)){
        return false;
    }
    token = strtok(NULL,delimiteur);
    if(token != NULL){
        if(!is_query(token)){  // a check parce que j'ai l'impression que ca vérifie pas que dans le deuxieme cas
            return false;       // on ai bien la relative part
        }
    }
    return true; 
}









/*protocol = protocol-name [ "/" protocol-version ]		*/
bool is_protocol(char* chaine)
{
    char* delimiteur = "/ \t\n";
    char* token = strtok(chaine,delimiteur);
    if(token == NULL || !is_protocol_name(token)){
        return false;
    }
    token = strtok(NULL,delimiteur);
    if(token != NULL){
        if(!is_protocole_version(token)){
            return false;
        }
    }
    return true; 

}

/*protocol-name = token
protocol-version = token
pseudonym = token c'est les mêmes*/

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
bool is_qd_text(char c)
{
    return (c == '\t' || c == ' ' || c == '!' || (c >= 0x23 && c <= 0x5B) || (c >= 0x5D && c <= 0x7E) || (c >= 0x80 && c <= 0xFF));
}
/*quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )*/
bool is_quoted_pair(char* chaine)
{
    if(&chaine[0] != '\\'){
        return false;
    }
    else{
        return(chaine[1] == '\t' || chaine[1] == ' ' ||(chaine[1] >= 0x33 && chaine[1] <= 0x126) ||(chaine[1] >= 0x80 && chaine[1] <= 0xFF));
    }
}
/*quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE			*/
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
/*received-by = ( uri-host [ ":" port ] ) / pseudonym*/
bool is_received_by(char* chaine)
{
    if(is_pseudonym(chaine)){
        return true;
    }
    // même chose faut trouver avant le strtok comment je fais pour check que le premier
    char* recopie = strdup(chaine);
    char* delim = ":";
    char* token = strtok(chaine,delim);
    if(token == NULL || is_port(token)){
        return false;
    }
    // aussi ajouter pour vérifier le début
}



/*received-protocol = [ protocol-name "/" ] protocol-version*/
// on vérifie que si il y a un / avant on a un protocol-name et apres un protocol version
// si on a rien on vérifie qu'on a juste un protocol-version
// a vérifier parce que ca marche pas je pense
bool is_received_protocol(char* chaine)
{
    char* separateur = "/";
    char* token;
    char* same = strdup(chaine);
    token = strktok(chaine,separateur);
    if(token == NULL){
        return is_protocole_version(chaine);
    }
    if(token != NULL && is_protocole_version(token)){
        return true;
    }
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