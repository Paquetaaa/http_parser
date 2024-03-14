#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct{

    const char space;
} sp;

typedef struct{   // Hesitation a juste mettre typedef char digit

    char d;
} digit;

typedef struct{   // Hesitation a juste mettre typedef char alpha

    char a;
} alpha;

typedef struct{

    const char tab;
} htab;

typedef struct {
  
    const char n;
} lf;

typedef struct {
  
    const char r;
} cr;

typedef struct {
    
    digit* nbr;    // Doit contenir au moins 1 digit
} nombre;

typedef struct {

    bool b_vir;
    bool b_dot;
    bool b_excl;
    bool b_inter;
    bool b_ddot;

    char p;
} ponct;

typedef struct {

    bool b_sp;
    bool b_htab;
    bool b_tiret;
    bool b_under;

    char s;
} separateur;

typedef struct {

    const char start[6];
} debut;

typedef struct {
    
    const char fin[4];
} fin;

typedef struct {

    alpha* m;      // Doit contenir au moins 1 alpha
    separateur s;
} mot;

typedef struct {

    debut deb;
    
    mot m1;
    ponct p1;
    mot m2;
    ponct p2;

    nombre n1;
    separateur s1;
    nombre n2;
    separateur s2;

    ponct p_opt;

    fin f;
    lf rl;
} message;



