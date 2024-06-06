// Temporaire, les fonctions de réponse à rajouter à la fin
char* lecture_reponse(int socket)
{
    char* reponse = malloc(16*FASTCGILENGTH);
    int taille;
    FCGI_Header *out = malloc(sizeof(FCGI_Header)*16);
    int i = 0;
    while(read(socket,&out[i],sizeof(out[i])) > 0)
    {
        taille = ntohs(out[i].contentLength);
        if(out[i].type == FCGI_END_REQUEST)
        {
            printf("End request atteint c'est finito \n");
            printf("valeur des champs = appstatus = %d, protocolstatus = %d \n");
            break;
        }
        char *content = malloc(sizeof(char)*taille);
        read(socket,content,taille);
        if(out[i].type == FCGI_STDOUT)
        {
            printf("c'est un stdout : %s\n",content);
        }
        strcat(reponse,content);
        i++;
        if(i == 16)
        {
            break;
        }
        //free(content);
    }
    //reponse = realloc(reponse,strlen(reponse)); realloc a une taille + petite c'est pas fou
    return reponse;
}

// Lit les std_out en entier, formate la réponse et l'envoie
void readAndSendReponse(int socket,char* buffer_reponse,int clientId, struct sockaddr_in *clientAddress)
{
    char* contenu = lecture_reponse(socket);
    message *reponse = (message *)malloc(sizeof(message));

    reponse->buf = contenu;
    reponse->len = strlen(contenu);
    reponse->clientId = clientId;
    reponse->clientAddress =clientAddress;

    sendReponse(reponse);
    free(reponse);
}

// Ouvre la socket, envoie, lit la réponse, et appelle au dessus pour répondre
void Requete(char* buffer_reponse,int clientId, struct sockaddr_in *clientAddress)
{
    int socket = createSocket(IP, PORT);
    int requestID = 1;

    Create_and_Send_BeginRequest(socket, requestID);
    createRequeteParams(socket);
    createEmptyParams(socket);
    sendStdin(socket, requestID,"", strlen(""));
    
    readAndSendReponse(socket,buffer_reponse,clientId,clientAddress);
    
    close(socket); 
}
