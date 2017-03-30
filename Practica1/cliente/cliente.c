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
}
 
void Cliente(int clienteSockfd){
  /*Buffer de dados a ser mandado para o servidor*/
  char buffer_para_servidor[256];
  printf("Digite uma mensage para el servidor: ");
   do {
       printf("\nIngrese una opcion\n");
       printf("1-Ingresar registro\n");
       printf("2-Ver registros\n");
       printf("3-Borrar registro\n");
       printf("4-Buscar registro\n");
       printf("5-salir\n\n");
       char caracter;
       
       //scanf("%s",buffer_para_servidor);
       int i = 0;
       scanf("%s",buffer_para_servidor);
       char *data = (char*)&i;
       write(clienteSockfd, buffer_para_servidor, sizeof(i));
       if(strcmp(buffer_para_servidor, "1") == 0){
           
           char nombre[32];
           caracter = mygetch();
           getInput("Ingrese el nombre:", nombre, 33);
           write(clienteSockfd, nombre, sizeof(nombre));
           char tipo[32];
           getInput("Ingrese el tipo:", tipo, 33);
           write(clienteSockfd, tipo, sizeof(tipo));
           int edad;
           printf("Ingrese Edad: ");
           scanf("%i",&edad);
           //caracter = mygetch();
           char raza[16];
           caracter = mygetch();
           getInput("Ingrese la raza:", raza, 17);
           int estatura;
           printf("Ingrese Estatura: ");
           scanf("%d",&estatura);
           int peso;
           printf("Ingrese Peso: ");
           scanf("%d",&peso);
           char sexo;
           printf("Ingrese Sexo: ");
           scanf("%s",&sexo);
           while( sexo != 'H' && sexo != 'M' ){
               printf("\nCaracter no valido.\n Por favor, intente de nuevo: ");
               scanf("%s", &sexo);
           }
           
           press();
       }else if(strcmp(buffer_para_servidor, "2") == 0){
           
           char perro[8];
           caracter = mygetch();
           getInput("Ingrese el id de la mascota a ver:", perro, 9);
           write(clienteSockfd, perro, sizeof(perro));
           
           press();
       }else if(strcmp(buffer_para_servidor, "3") == 0){
           
           char perro[8];
           caracter = mygetch();
           getInput("Ingrese el id de la mascota a borrar:", perro, 9);
           write(clienteSockfd, perro, sizeof(perro));
           
           press();
       }else if(strcmp(buffer_para_servidor, "4") == 0){
           
           char perro[32];
           caracter = mygetch();
           getInput("Ingrese el nombre de la mascot a buscar:", perro, 33);
           write(clienteSockfd, perro, sizeof(perro));
           
           press();
       }else if(strcmp(buffer_para_servidor, "5") == 0){
           write(clienteSockfd, "salir", 5);
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
 
      /*Mensagem para sair*/
    } while (strcmp(buffer_para_servidor, "salir") != 0);
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
//    printf("\nIngrese una opcion\n");
//    printf("1-Ingresar registro\n");
//    printf("2-Ver registros\n");
//    printf("3-Borrar registro\n");
//    printf("4-Buscar registro\n");
//    printf("5-salir\n\n");
        Cliente(descritorCliente);
//    if(i==1){
//        struct nodo *perro;
//        perro = ingresar();
//        printf("%s",perro->nombre);
//        Cliente(descritorCliente);
//        press();
//    }else if(i==2){
//        //verRegistro();
//        //Cliente(descritorCliente);
//        press();
//    }else if(i==3){
//        //borrar();
//        press();
//    }else if(i==4){
//        //mostrar();
//        press();
//    }else if(i==5){
//        //escribirHash();
//        break;
//    }else if(i==6){
//        //for(i=0;i<1721;i++){
//        //    h[i].key=0;
//        //}
//        //generar();
//        //press();
//    }
    }while(i!=0);
    /*chama funcao do cliente passando o descritor*/
    //Cliente(descritorCliente);
    exit(0);
}

struct nodo *ingresar(){

    char nombre[32];
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    int peso;
    char sexo;
    int key;
    int id;
    char caracter;
    
    //INGRESO DE VARIABLES
    caracter = mygetch();
    getInput("Ingrese el nombre:", nombre, 33);
    getInput("Ingrese el tipo:", tipo, 33);
    printf("Ingrese Edad: ");
    scanf("%i",&edad);
    caracter = mygetch();
    getInput("Ingrese la raza:", raza, 17);
    printf("Ingrese Estatura: ");
    scanf("%d",&estatura);
    printf("Ingrese Peso: ");
    scanf("%d",&peso);
    printf("Ingrese Sexo: ");
    scanf("%s",&sexo);
    while( sexo != 'H' && sexo != 'M' ){
        printf("\nCaracter no valido.\n Por favor, intente de nuevo: ");
        scanf("%s", &sexo);
    }
    
    struct nodo *temp;
    temp = malloc(sizeof(struct nodo));
    if(temp == NULL){
        perror("Cannot malloc temp1 in Ingresar");
        exit(-1);
    }
    
    for (int u=0;u<32;u++){
        
        if(nombre[u] == '\n'){
            nombre[u] = ' ';
        }
    }
    for (int u=0;u<32;u++){
        
        if(tipo[u] == '\n'){
            tipo[u] = ' ';
        }
    }
    for (int u=0;u<32;u++){
        
        if(raza[u] == '\n'){
            raza[u] = ' ';
        }
    }
    //key = convertir(temp->nombre);
    strcat(nombre," ");
    strncpy(temp->nombre,nombre,32);
    strncpy(temp->tipo,tipo,32);
    temp->edad = edad;
    strncpy(temp->raza,raza,16);
    temp->estatura = estatura;
    temp->peso = peso;
    temp->sexo = sexo;
    temp->key = key;
    temp->id=0;
    return temp;
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
