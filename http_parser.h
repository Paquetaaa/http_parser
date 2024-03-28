typedef enum{


} TypeNoeud;

typedef struct node
{
    TypeNoeud type;         // Type du noeud
    char* etiquette;        // Etiquette du noeud
    char* pointeur_debut;   // Debut de la string que represente le noeud 
    int longueur;           // Taille de la string representee par le noeud
    struct node* pere;      // 
    struct node* fils_aine; // 
    struct node* frere;     // 

} Noeud;


Noeud* construire_arbre(void);

