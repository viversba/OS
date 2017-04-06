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
    /*Descritor do cliente*/
    int clienteSockfd;
    /*Estrutura do Cliente*/
    struct sockaddr_in serv_addr;
    /*Tamanho da estrutura*/
    socklen_t addrlen = sizeof (serv_addr);
 
    /*Socket familia TCP declarado SOCK_STREAM e criado*/
    clienteSockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd < 0)
    {
        printf("Error en Socket\n");
        exit(1);
    }
    /*Zera a estrutura*/
    bzero((char *) & serv_addr, sizeof (serv_addr));
    /*Seta a familia*/
    serv_addr.sin_family = AF_INET;
    /*Define o IP nesse caso o localhost*/
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /*Define a porta de conexao*/
    serv_addr.sin_port = htons(6881);
   /*Faz a conexao com o servidor*/
   if(connect(clienteSockfd,(struct sockaddr *)&serv_addr, addrlen) < 0)
     {
            printf("Error en Socket\n");
        exit(1);
    }
        return clienteSockfd;
};

int entero(char num[8]){
	int tam = strlen(num);
	//printf("\nentero %s\n",num);
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
  /*Buffer de dados a ser mandado para o servidor*/
  
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
		   int peso=0;
		   char sexo='\0';
           struct nodo *temp;
		   temp = malloc(sizeof(struct nodo));
		   printf("Ingrese nombre:\n");
		   scanf("%s",nombre);
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
		   scanf("%i",&peso);
		   temp->peso=peso;
		   printf("Ingrese sexo:  (H|M)\n");
		   scanf("%s",&sexo);
		   while( sexo != 'H' && sexo != 'M' ){
		      printf("\nCaracter no valido.\n Por favor, intente de nuevo: ");
		      scanf("%c", &sexo);
		   }
		   temp->sexo=sexo;
		   write(clienteSockfd,temp,sizeof(struct nodo));
           recv(clienteSockfd,confirmacion,17,0);
		   printf("\n%s\n",confirmacion);
           press();
       }else if(strcmp(opcion, "20") == 0){
	      	//RECIBIR TAMANO
	      	char sizeD[10];
	        recv(clienteSockfd,sizeD,10,0);
	        char perro[8];
	        char idmas3[11];
			i=0;
			while(sizeD[i]=='0'){
				i = i+1;
			}
			int k;
			int u=0;
			for(k=i;k<11;k++){
				idmas3[u] = sizeD[k];
				u++;
			}
			printf("Existen %s registros. ",idmas3);
	        caracter = mygetch();
	        //FIN RECIBIR TAMANO

	        //ENVIAR ID
	        int r=1;
			do{
		        getInput("Ingrese el id de la mascota a ver:", perro, 9);
			    tam = strlen(perro);
			    char id[8]="";
				if(tam<8){
					int p;
					for(p=0;p<(8-tam);p++){
						strcat(id,"0");
					}
				}
				strcat(id,perro);
		        write(clienteSockfd, id, 8);
	        //FIN ENVIAR ID

	        //RECIBIR TEXTO
		        char texto[120]="";
		        char tamano[3]="";
		        recv(clienteSockfd,tamano,3,0);
				recv(clienteSockfd,texto,120,0);
			   	tam = entero(tamT);
			   	printf("%s",texto);
				if(strcmp(texto, "No")!=0)break;
				printf("No existe registro. ");
				write(clienteSockfd, "No", 2);
			}while(r==1);
			write(clienteSockfd, "Si", 2);
        	press();
	        //FIN RECIBIR TEXTO
	    }
    else if(strcmp(opcion, "30") == 0){
        //RECIBIR TAMANO
	      	//RECIBIR TAMANO
      	char sizeD[10];
        recv(clienteSockfd,sizeD,10,0);
        char perro[8];
        char idmas3[11];
		for(i=1;i<11;i++){
			idmas3[i-1] = sizeD[i];
		}
		i=0;
		while(idmas3[i]=='0'){
			i = i+1;
		}
		int u = 10-i;
		char idmas2[u];
		int z=0;
		for(u=i;u<10;u++){
			idmas2[z] = idmas3[u];
			z++;
		}
		char ult[sizeof(idmas2)];
		for(i=0;i<(sizeof(idmas2) - 1);i++){
			ult[i] = idmas2[i];
		}
		printf("Existen %s registros. ",ult);
        caracter = mygetch();
        //FIN RECIBIR TAMANO

        //ENVIAR ID
        int r=2;
		do{
	        getInput("Ingrese el id de la mascota a borrar:", perro, 9);
		    tam = strlen(perro);
		    char id[8]="";
			if(tam<8){
				int p;
				for(p=0;p<(8-tam);p++){
					strcat(id,"0");
				}
			}
			strcat(id,perro);
	        //printf("ID  %s",id);
	        fflush(stdout);
	        write(clienteSockfd, id, 8);
	        fflush(stdout);
        //FIN ENVIAR ID

        //RECIBIR TEXTO
	        char texto[120];
	        char tamano[2];
	        recv(clienteSockfd,tamano,2,0);
		   	//tam = entero(tamT);
		   	if(strcmp(tamano, "no")){
		   		printf("\nRegistro eliminado exitosamente\n");
		   	}
		   	else{
		   		printf("\nRegistro eliminado exitosamente\n");
		   	}
		   	printf("%s",texto);
		   	fflush(stdout);
		}while(r==1);
    	press();
	        //FIN RECIBIR TEXTO
    }
    else if(strcmp(opcion, "40") == 0){
           
           char perro[32];
           	caracter = mygetch();
           	getInput("Ingrese el nombre de la mascot a buscar:", perro, 33);
           	write(clienteSockfd, perro, sizeof(perro));
           	char texto[200];
           	printf("\nPosicion\tCodigo\t\tId\t\tnombre\n");
           	do{
           		recv(clienteSockfd,texto,200,0);
           		if(strcmp(texto, "salir") != 0 && strcmp(texto, "unable") != 0){
           			printf("%s",texto);
           		}
           		/*
           		if(strcmp(texto, "unable") == 0){
           			printf("\nEste nombre no esta en la base de datos \n");
           			Cliente(clienteSockfd);
           			sprintf(texto,"salir");
           			break;
           		}*/
           	}while(strcmp(texto, "salir") != 0);
           	press();
       }else if(strcmp(opcion, "50") == 0){
           
           //write(clienteSockfd, "salir", 5);
           close(clienteSockfd);
           exit(0);

       }else if(i==6){
           //for(i=0;i<1721;i++){
           //    h[i].key=0;
           //}
           //generar();
           //press();
       }
       
       fflush(stdin);
        /*Escreve para o servidor*/
 
        //write(clienteSockfd, buffer_para_servidor, sizeof (buffer_para_servidor));
 		write(clienteSockfd,"00",2);
      /*Mensagem para sair*/
    } while (strcmp(opcion, "50") != 0);
    /**Encerra o descritor*/
    close(clienteSockfd);
}
 
int main(){
    
    struct sockaddr_in serv_addr;
    /*Tamanho da estrutura*/
    socklen_t addrlen = sizeof (serv_addr);
    /*Define o descritor cliente*/
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

//METODO DE PRESIONAR EL BOTON PARA CONTINUAR
void press(){
    char caracter;
    printf("Presione cualquier tecla para continuar: \n");
    caracter = mygetch();
    caracter = mygetch();
    system("clear");
    //	printf("\nEl caracter que ingreso es %c", caracter);
}

void getInput(char *question, char *inputBuffer, int bufferLength){
    //press();
    printf("%s  (Max %d letras)\n", question, bufferLength - 1);
    fgets(inputBuffer, bufferLength, stdin);
    
    if (inputBuffer[strlen(inputBuffer) -1] != '\n')
    {
        int dropped = 0;
        while (fgetc(stdin) != '\n')
            dropped++;
        
        if (dropped > 0) // if they input exactly (bufferLength - 1)
            // characters, there's only the \n to chop off
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
