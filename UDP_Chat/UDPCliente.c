#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(char *); 

int main(int argc, char *argv[]) 
{ 
    int sock, length, n; 
    struct sockaddr_in server, from; 
    struct hostent *hp; 
    char buffer[256]; 

    if (argc != 3) { 
        printf("Uso correto: servidor porta\n"); 
        exit(1); 
    } 

    sock= socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) 
        error("socket"); 
  
    server.sin_family = AF_INET; //ip localhost
    hp = gethostbyname(argv[1]); 
    if (hp==0) error("host desconhecido"); 
    bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length); 
    server.sin_port = htons(atoi(argv[2])); 
    length=sizeof(struct sockaddr_in); 
	
	while(1)
	{
		printf("\nDigite a menssagem: "); 
		bzero(buffer,256); 
		fgets(buffer,256,stdin);
		fflush(stdin);  

		n=sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&server,length); 
		if (n < 0) error("Sendto"); 
		
		bzero(buffer,256); //limpa o buffer para receber a proxima msg
		
		n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length); 
		if (n < 0) error("recvfrom"); 

		printf("\nMSG recebida do servidor: %s\n",buffer); 
	}
    
    //write(1,"ACK:  ",12); 
    //write(1,buffer,n); 

} 

void error(char *msg) 
{ 
    perror(msg); 
    exit(0); 
} 

