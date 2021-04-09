
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 4444
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "HTTP/1.0 200 OK\nContent-Type: text/html\nContent-Length: ";
    char *hello2 = malloc(100001);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("listen()");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\nServidor Iniciado\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept()");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        char tipo_de_request[3] = "GET";
        if(buffer[0] == tipo_de_request[0])
        {
            if(buffer[1] == tipo_de_request[1])
            {
                if(buffer[2] == tipo_de_request[2])
                {
                    char arquivo_name[60];
                    int cont = 0;
                    int i = 5;
                    char arqhtml[100000];
                    char c;
                    char aux[100000];
                    char qtd_cara_arq[10];
                    while(buffer[i] != ' ')
                    {
                        arquivo_name[cont] = buffer[i];
                        i = i + 1;
                        cont = cont + 1;
                    }
                    printf("Nome do Arquivo : %s\n", arquivo_name);
                    FILE *arq;
                    arq = fopen(arquivo_name, "r");

                    if(arq == NULL)
                    {
                        printf("Deu Merda\n");
                        char *helloerrado = "HTTP/1.0 404 não encontrado";
                        write(new_socket , helloerrado , strlen(helloerrado));
                        printf("Resposta Enviada");
                        close(new_socket);
                    }
                    else
                    {
                        cont = 0;
                        while((c = fgetc(arq)) != EOF)
                        {
                            arqhtml[cont++] = c;
                        }
                        fclose(arq);
                        //printf("Html : %s\n",arqhtml);

                        int indice = 0;
                        for(;indice < strlen(hello);indice++)
                        {
                            aux[indice] = hello[indice];
                        }          
                        sprintf(qtd_cara_arq, "%d", cont); 
                        strcat(aux, qtd_cara_arq);
                        strcat(aux, "\n\n");      
                        strcat(aux, arqhtml);
                        indice += strlen(qtd_cara_arq) + cont + 2;
                        for(int ii=0; ii<indice;ii++)
                        {
                            hello2[ii] = aux[ii];
                        }
                        write(new_socket , hello2 , strlen(hello2));
                        printf("Resposta Enviada");
                        close(new_socket);



                    }
                }
            }
        }
    }
    return 0;
}
