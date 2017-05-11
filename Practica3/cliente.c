#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

int mygetch();
int configuracaoCliente();
void Cliente();
void press();
void getInput();
struct nodo *ingresar();

struct nodo{
    int key;
    int id;
    char nombre[32];
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    float peso;
    char sexo;
    int cuantos;
    struct nodo * next;
    struct nodo * final;
}nodo;

int configuracaoCliente(){
    int clienteSockfd;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof (serv_addr);
    clienteSockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd < 0)
    {
        printf("Error en Socket\n");
        exit(1);
    }
    bzero((char *) & serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(6881);
   if(connect(clienteSockfd,(struct sockaddr *)&serv_addr, addrlen) < 0)
     {
            printf("Error en Socket\n");
        exit(1);
    }
        return clienteSockfd;
};

int entero(char num[8]){
	int tam = strlen(num);
	int i, salida = 0;
	int ayuda = 0;
	for(i=0; i<tam; i++){
		salida = salida*10;
		ayuda = num[i]-48;
		salida+=ayuda;
	}
	return salida;
}
 
void Cliente(int clienteSockfd){
  
  	printf("Digite uma mensage para el servidor: ");
  	char opcion[2];
   	do {
	    printf("\nIngrese una opcion\n");
	    printf("1-Ingresar registro\n");
	    printf("2-Ver registros\n");
	    printf("3-Borrar registro\n");
	    printf("4-Buscar registro\n");
	    printf("5-salir\n\n");
	    char caracter;
	    int i = 0;
	    char opcion[2];
	    scanf("%s",opcion);
	    strcat(opcion,"0");
        write(clienteSockfd, opcion,2);
		printf("opcion = %s\n",opcion);
       	int tam;
		char tamT[3];
		char confirmacion[20];
		char tamano[2];
		char buffer_para_servidor[256];

       	if(strcmp(opcion, "10") == 0){
		   char nombre[32]="\0";
		   char tipo[32]="\0";
		   int edad=0;
		   char raza[16]="\0";
		   int estatura=0;
		   float peso=0;
		   char sexo='\0';
           struct nodo *temp;
		   temp = malloc(sizeof(struct nodo));
		   printf("Ingrese nombre:\n");
		   scanf("%s",nombre);
		   strcat(nombre, " ");
		   strncpy(temp->nombre,nombre,32);
		   printf("Ingrese tipo:\n");
		   scanf("%s",tipo);
		   strncpy(temp->tipo,tipo,32);
		   printf("Ingrese edad:\n");
		   scanf("%i",&edad);
		   temp->edad=edad;
		   printf("Ingrese raza:\n");
		   scanf("%s",raza);
		   strncpy(temp->raza,raza,16);
		   printf("Ingrese estatura:\n");
		   scanf("%i",&estatura);
		   temp->estatura=estatura;
		   printf("Ingrese peso:\n");
		   scanf("%f",&peso);
		   temp->peso=peso;
		   printf("Ingrese sexo:  (H|M)\n");
		   scanf("%s",&sexo);
		   while( sexo != 'H' && sexo != 'M' ){
		      printf("\nCaracter no valido.\n Por favor, intente de nuevo: ");
		      scanf("%c", &sexo);
		   }
		   temp->sexo=sexo;
			fflush(stdout);
		   write(clienteSockfd,temp,sizeof(struct nodo));
           recv(clienteSockfd,confirmacion,17,0);
           press();
       	}
		else if(strcmp(opcion, "20") == 0){
	      	char sizeD[10];
	        recv(clienteSockfd,sizeD,10,0);
	        char perro[12];
			printf("Existen %s registros. ",sizeD);
	        int r=1;
			do{
		        printf("Ingrese el id de la mascota a ver:");
			    scanf("%s",perro);
				printf("id = %s",perro);
				fflush(stdout);
		        send(clienteSockfd, perro, 12,0);
				char titulo[120]="";
		        char texto[1000]="";
				recv(clienteSockfd,titulo,120,0);
			   	tam = entero(tamT);
				if(strcmp(titulo, "No")!=0){
					recv(clienteSockfd,texto,1000,0);
					FILE *ap;
					ap = fopen("Historia.txt", "w");
					fwrite(texto,strlen(texto),1, ap);
					fclose(ap);
					system("nano Historia.txt");
					sleep(2);
					ap = fopen("Historia.txt","r+");
					fread(texto,sizeof(texto),1,ap);
					send(clienteSockfd, texto, 1000, 0);
					fclose(ap);
					break;
				}
				printf("No existe registro. ");
				send(clienteSockfd, "No", 2,0);
			}while(r==1);
			send(clienteSockfd, "Si", 2, 0);
        	press();
	    }
    	else if(strcmp(opcion, "30") == 0){
			char sizeD[10];
			recv(clienteSockfd,sizeD,10,0);
			char perro[12];
			printf("Existen %s registros. ",sizeD);
			caracter = mygetch();
				printf("Ingrese el id de la mascota a borrar:\n");
				scanf("%s", perro);
				write(clienteSockfd, perro, 12);
				char tamano[2];
				recv(clienteSockfd,tamano,4,0);
				if(strcmp(tamano, "no")==0){
					printf("\nEl registro no existe por lo tanto no se elimino nada\n");
				}
				else{
					printf("\nRegistro eliminado exitosamente \n");
				}
				fflush(stdout);
			press();
    	}
    	else if(strcmp(opcion, "40") == 0){
           
           	char perro[32]="";
           	caracter = mygetch();
           	getInput("Ingrese el nombre de la mascot a buscar:", perro, 33);
           	write(clienteSockfd, perro, sizeof(perro));
           	char texto[200];
           	printf("\nPosicion\tCodigo\t\tId\t\tnombre\n");
           	do{
           		recv(clienteSockfd,texto,200,0);
           		if(strcmp(texto, "salir") != 0 && strcmp(texto, "unable") != 0 && strcmp(texto, "no") != 0){
           			printf("%s",texto);
           		}else{
					break;
				}
           	}while(strcmp(texto, "salir") != 0);
           	press();
       	}
		else if(strcmp(opcion, "50") == 0){
           
           close(clienteSockfd);
           exit(0);
       	}
		else{
       	}
       	fflush(stdin);
    } while (strcmp(opcion, "50") != 0);
    close(clienteSockfd);
}
 
int main(){
    
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof (serv_addr);
    int descritorCliente;
    descritorCliente = configuracaoCliente();
    
    
    int i;
    i=0;
    do{  
        Cliente(descritorCliente);
    }while(i!=0);
    exit(0);
}

int mygetch(){
    struct termios oldt,newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
};

void press(){
    char caracter;
    printf("Presione cualquier tecla para continuar: \n");
    caracter = mygetch();
    caracter = mygetch();
    system("clear");
}

void getInput(char *question, char *inputBuffer, int bufferLength){
    printf("%s  (Max %d letras)\n", question, bufferLength - 1);
    fgets(inputBuffer, bufferLength, stdin);
    
    if (inputBuffer[strlen(inputBuffer) -1] != '\n')
    {
        int dropped = 0;
        while (fgetc(stdin) != '\n')
            dropped++;
        
        if (dropped > 0) 
        {
            printf("Nombre muy largo, se ha pasado por %d letras, intente otra vez!\n", dropped );
            getInput(question, inputBuffer, bufferLength);
        }
    }
    else
    {
        inputBuffer[strlen(inputBuffer) -1] = '\0';
    }
}
