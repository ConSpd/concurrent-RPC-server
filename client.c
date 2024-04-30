#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

// Συνάρτηση ελέγχου συμβολοσειράς εάν είναι αριθμός
// opt 1 = Ακέραιος, opt 2 = Πραγματικός
int isNumber(char* str,int opt){
    int start = 0;
    int d_flag = 1;
    if(strlen(str) == 1)
        return 0;
    
    // Σε περίπτωση που είναι αρνητικός θα έχει το - απο μπροστά
    // έτσι το for θα ξεκινάει απο το δεύτερο χαρακτήρα να ελέγχει
    if (str[0] == '-')
        start = 1;
    
    for(int i=start;i<strlen(str)-1;i++){
        if(opt == 2){
            if((str[i] == '.') && d_flag && (i > 0) && (i<strlen(str))){ // Έλεγχος για τελεία δεξαδικού, αρκεί να μην έχει επαναληφθεί (d_flag) 
                d_flag = 0;                                              //και να μην είναι πρώτος ή τελευταίος χαρακτήρας (i>0 && i<strlen(str))
                continue;
            }
        }
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int main(int argc,char** argv){

    /*
     * A.1
     * Δημιουργία socket
    */

    struct sockaddr_in serv_addr;
    struct hostent *server;
    int sockfd,done,t;
    char str[100];

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    server = gethostbyname("127.0.0.1");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(7218); // Βάζουμε οι ίδιοι port χωρίς να χρειάζεται να εισαχθεί απο το command line

    printf("Trying to Connect...\n");

    if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        printf("Connection Failure\n");
    else{
        printf("Connected!\n");
        char msg[100], size_c[10], num[20];
        int size,n,opt;

        /*
         * A.2
         * Αρχικοποίηση δεδομένων
        */

        // Α.2.1 Εισαγωγή μεγέθους πίνακα
        t = recv(sockfd,msg,100,0);
        do{
            printf("%s",msg);
            fgets(size_c,10,stdin);
        }while(!isNumber(size_c,1));
        send(sockfd,size_c,strlen(size_c),0);
        size = atoi(size_c);

        // Α.2.2 Εισαγωγή τιμών πίνακα
        for(int i=0;i<size;i++){
            n = recv(sockfd,msg,100,0);
            msg[n] = '\0';
            do{
                printf("%s",msg);
                fgets(num,20,stdin);
            }while(!isNumber(num,1));
            send(sockfd,num,20,0);
        }

        // Α.2.3 Εισαγωγή αριθμού a
        recv(sockfd,msg,30,0);
        do{
            printf("%s",msg);
            fgets(num,20,stdin);
        }while(!isNumber(num,2));
        send(sockfd,num,20,0);

        /*
         * A.3 
         * Τμήμα που εμφανίζεται το menu στον client
        */

        while(1){
            // A.3.1 Εμφάνιση μενού
            n = recv(sockfd,msg,100,0);
            msg[n] = '\0';
            printf("%s\n",msg);
            
            // A.3.2 Εισαγωγή & αποστολή επιλογής απο τον χρήστη
            printf("> ");
            fgets(msg,100,stdin);
            opt = atoi(msg);
            while((opt < 1) || (4 < opt)){
                printf("Enter a valid selection\n> ");
                fgets(msg,100,stdin);
                opt = atoi(msg);
            }
            send(sockfd,msg,5,0);

            if(!strcmp(msg,"4"))
                break;
            
            // Α.3.3 Εκτύπωση ληφθέντος μηνύματος απο server
            t = recv(sockfd,msg,100,0);
            printf("%s\n\n",msg);
        }
    }
    
    close(sockfd);

    return 0;
}