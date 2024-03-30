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
                 ["-" privateuse]*/
bool is_langtag(char* chaine)
{
    int taille = strlen(chaine);
    int i = 0;
    // on commence par vérifier que c'est un language
    while((index < taille) && chaine[index] != '-'){
        if(!is_language(chaine)){
            return false;
        }
    }

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
bool is_irregular(char* chaine)
{
    switch(chaine){
        case("en-GB-oed"):  return true;
        case("i-ami"):      return true;
        case("i-bnn"):      return true;
        case("i-default"):  return true;
        case("i-enochian"): return true;
        case("i-hak"):      return true;
        case("i-klingon"):  return true;
        case("i-lux"):      return true;
        case("i-mingo"):    return true;
        case("i-navajo"):   return true;
        case("i-pwn"):      return true;
        case("i-tao"):      return true;
        case("i-tay"):      return true;
        case("i-tsu"):      return true;
        case("sgn-BE-FR"):  return true;
        case("sgn-BE-NL"):  return true;
        case("sgn-CH-DE"):  return true;
        default:            return false;
    }
}

bool is_regular(char* chaine)
{
    switch(chaine){
        case("art-lojban"):  return true;
        case("cel-gaulish"): return true;
        case("no-bok"):      return true;
        case("no-nyn"):      return true;
        case("zh-guoyu"):    return true;
        case("zh-hakka"):    return true;
        case("zh-min"):      return true;
        case("zh-min-nan"):  return true;
        case("zh-xiang"):    return true;
        default:             return false;
    }

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


bool is_host(char* chaine)
{

}


bool is_OWS(char *chaine)
{
    int taille = strlen(chaine);
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
    while()
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

/*protocol-name = token
protocol-version = token
pseudonym = token c'est les mêmes*/

bool is_protocole-name(char* chaine)
{
    return is_token(chaine);
}
bool is_protocole-version(char* chaine)
{
    return is_token(chaine);
}
bool is_pseudonym(char* chaine)
{
    return is_token(chaine);
}

