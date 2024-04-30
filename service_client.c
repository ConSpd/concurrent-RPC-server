/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "service.h"
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


struct Sock_Host{
	int sockfd;
	char* host;
};

void *service(struct Sock_Host* info){
	
	// B.3
	/*
	 * Αρχικοποίηση μεταβλητών
	*/
	CLIENT *clnt;
	double  *result_1;
	data  average_1_arg;
	d_ret  *result_2;
	var_array  *result_3;
	
    int n, done, size;
    int t = info->sockfd;
    char str[100],size_c[10],num[20], prod_num[100];
    
	// Β.4
	/*
	 * Εισαγωγή τιμών απο τον client
	*/

	// B.4.1 Είσοδος απο client μεγέθους πίνακα
    send(t,"Enter size: ",100,0);
    n = recv(t,size_c,10,0);
    // Μετατροπή απο string σε int
    size = atoi(size_c);
	

	#ifndef	DEBUG
	clnt = clnt_create (info->host, Service, Srv_1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (info->host);
		exit (1);
	}
	#endif	/* DEBUG */

    // B.4.2 Αρχικοποίηση πίνακα 
	average_1_arg.y.y_len = size;
	average_1_arg.y.y_val = (int *) malloc (size*sizeof(int));
    // Είσοδος τιμών του πίνακα
    for(int i=0;i<size;i++){
        sprintf(str,"Enter number %d: ",i+1);
        send(t,str,100,0);
        n = recv(t,num,20,0);
        num[n] = '\0';
		average_1_arg.y.y_val[i] = atoi(num);
    }
    
	// B.4.3 Είσοδος αριθμού a
    send(t,"Enter a: ",20,0);
    recv(t,num,20,0);
    average_1_arg.a = atof(num);
    
	
	// B.5
	/*
	 * Menu επιλογών 
	*/
	while(1){
		
		// Β.5.1 Εμφάνιση επιλογών στον Client
		bzero(str,100);
		snprintf(str,100,"--- Choose Option ---\n1. Average of Array\n2. Min and Max of Array\n3. Product of a * Y\n4. Exit");
		send(t,str,100,0);
		
		// Β.5.2 Επιλογή τιμής
		n = recv(t,str,5,0);
		str[n] = '\0';
		int choice = atoi(str);
		if(choice == 4)
			break;
		
		// Β.6
		/*
		 * Κλήση λειτουργιών απο RPC server
		*/
		switch (choice){
			case 1:
				result_1 = average_1(&average_1_arg, clnt);
				if (result_1 == (double *) NULL) {
					clnt_perror (clnt, "call failed");
				}
				sprintf(str,"Average is: %lf\n",*result_1);
				send(t,str,100,0);
				break;
			
			case 2:
				result_2 = min_max_1(&average_1_arg, clnt); // Βάζουμε παντού average_1_arg γιατί η είσοδος είναι ίδια
				if (result_2 == (d_ret *) NULL) {
					clnt_perror (clnt, "call failed");
				}
				sprintf(str,"Min = %d\nMax = %d\n",result_2->a[1],result_2->a[0]);
				send(t,str,100,0);
				break;
			case 3:
				result_3 = prod_1(&average_1_arg, clnt);
				if (result_3 == (var_array *) NULL) {
					clnt_perror (clnt, "call failed");
				}
				bzero(prod_num,100);
				for(int i=0;i<size;i++){	// Σε κάθε loop κάνει concat σε string το κάθε στοιχείο του αποτελέσματος για να το στείλει μετά
					sprintf(str,"%.3lf ",result_3->j.j_val[i]);
					strcat(prod_num,str);
				}
				send(t,prod_num,100,0);
				break;
		}
	}

	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */

	free(average_1_arg.y.y_val);
    close(t);
    pthread_exit(NULL);
}



int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];


	// B.1
	/*
	 * Κώδικας Socket
	*/
	struct sockaddr_in serv_addr, cli_addr;
	struct Sock_Host info; 
	info.host = argv[1];
    int cli_len,sockfd,i;
    pthread_t thread[20];
    i = 0;
    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7218);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Δημιουργεί unbound socket επικοινωνίας. Επιστρέφει file descriptor για να χρησιμοποιηθεί σε συναρτήσεις
    if (sockfd < 0)
        printf("Error opening Socket\n");
    
    if(bind(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr))<0)
        printf("Error binding\n");

    listen(sockfd,20);
	
	// B.2
    while(1){
        printf("Waiting for connection...\n");
        cli_len = sizeof(cli_addr);
        info.sockfd = accept(sockfd,(struct sockaddr*) &cli_addr,(socklen_t *) &cli_len);
        if(info.sockfd < 0)
            printf("Error on accept");
        printf("Connected %d \n",i);
        pthread_create(&(thread[i++]),NULL,(void *)service,(struct Sock_Host*) &info);
    }  
	
	exit (0);
}