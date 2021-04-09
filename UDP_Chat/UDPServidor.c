#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 

void error(char *msg) 
{ 
    perror(msg); 
    exit(0); 
} 

int main(int argc, char *argv[]) 
{ 

    int sock, length, fromlen, n; 
    struct sockaddr_in server; 
    struct sockaddr_in from;
    char buf[1024]; 

    if (argc < 2) 
    { 
        fprintf(stderr, "ERRO: porta nao informada\n"); 
    exit(0); 
    } 




    sock=socket(PF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) 
        error("Opening socket"); 
  
  
    length = sizeof(server); 
    bzero(&server,length); 

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY; 

    server.sin_port=htons(atoi(argv[1])); 




    if (bind(sock,(struct sockaddr *)&server,length)<0) 
        error("binding"); 
    fromlen = sizeof(struct sockaddr_in); 


    while(1) 
	  {
		    bzero(buf,256);
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen); 
        if (n < 0) 
            error("recvfrom"); 
        printf("\n MSG recebida do cliente: %s",buf);
        fflush(stdin); 
		
		    char buffer[256];
		    printf("\nDigite a menssagem: "); 
		    bzero(buffer,256); //limpa o buffer para enviar a proxima msg
		    fgets(buffer,256,stdin);
		    fflush(stdin);  

		    n=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&from,length); 
		    if (n < 0) 
          error("Sendto"); 
    } 

}
