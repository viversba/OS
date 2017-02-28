#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>


void ingresar();
int convertir();
void mostrar();
void generar();
void calcular();
void borrar();
void verRegistro();
void save();
void getInput();
int mygetch();
int TamanioArchivo();
void cargarHash();
void press();
void borrarArchivo();

struct names{
    char nombre[32];
};
struct types{
    char tipo[32];
};
struct breeds{
    char breed[16];
};

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
	struct nodo * next;
}nodo;
struct nodo h[1721];

int main(){
	int i;
	for(i=0;i<1721;i++){
		h[i].key=0;
	}
	FILE *ap;
	ap = fopen("dataDogs.dat","r");
	if(ap==NULL){
			
	}else{
		cargarHash();
	}
	i=0;
	do{
	printf("\nIngrese una opcion\n");
	printf("1-Ingresar registro\n");
	printf("2-Ver registros\n");
	printf("3-Borrar registro\n");
	printf("4-Buscar registro\n");
	printf("5-salir\n\n");
	scanf("%d",&i);
	if(i==1){
           	ingresar();
		    press();
	}else if(i==2){
			verRegistro();
		    press();
	}else if(i==3){
			//borrar();
            borrarArchivo();
            cargarHash();
		    press();
	}else if(i==4){
			mostrar();
		    press();
	}else if(i==5){
			break;
	}else if(i==6){
			for(i=0;i<1721;i++){
				h[i].key=0;
			}
	 		generar();
		    press();
	}
	}while(i!=0);
	return 0;
}

void ingresar(){

  //DECLARACION DE NOMBRES
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
	key = convertir(temp->nombre);
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
	calcular(temp);
    save(temp);
};

void verRegistro(){
    FILE *ap;
    ap = fopen("dataDogs.dat","r");
    int cantidadListas = TamanioArchivo(ap);
    if(ap == NULL){
        perror("Can't open dataDogs.dat in verRegistro");
        exit(-1);
    }
    if (cantidadListas == 0){
        printf("No hay registros\n");
        return;
    }
    printf("El numero de registros presentes es de %d",TamanioArchivo(ap));
	printf("ingrese el numero del registro a ver: ");
	int ver;
	int paso=0;
	struct nodo * temp;
	temp = (struct nodo *)malloc(sizeof(struct nodo));
	ap = fopen("historia.txt","w");
	if(ap==NULL){perror("No se puede crear"); exit(-1);}
    int r;
    ap = fopen("historia.txt","a+");
    if(ap == NULL){perror("No se puede crear o abrir el archivo"); exit(-1);}
    
    
    FILE *fp; //Creamos un archivo temporal.
    int i; //Variable para el ciclo
    int aEliminar; //Número del perro a eliminar
    struct nodo *perro = malloc(sizeof(struct nodo));
    fp=fopen("dataDogs.dat","r");
    char nom[32];
    if (fp==NULL){
        printf("No se encontro el archivo dataDogs.dat");
        return;
    }
    else{
        scanf("%d",&ver);
        while(ver>cantidadListas){
            printf("El registro no existe. Por favor, intente otra vez: ");
            scanf("%d",&ver);
            printf("%i",ver);
        }
        ver = ver-1;
        for(i = 0 ; i < cantidadListas ; i++){
            fseek(fp, (i)*sizeof(struct nodo),SEEK_SET);//mueve fp hasta el registro i
            if(i==(ver)){//guarda los registros diferentes al que se va a elminar en el archivo temporal
                fread(perro, sizeof(struct nodo), 1, fp);
            }
        }
        fclose(fp);
    }
    
    int key = convertir(perro->nombre);
    key = key%1721;
    key = key*6000;
    ver = key;
	do{
		int llave = (int)(ver/6000);
        printf("%d",h[llave].key);         
		if(h[llave].key>=0){
			if(h[llave].id==ver){
				temp->id = h[llave].id;
				strncpy(temp->nombre,h[llave].nombre,32);
				strncpy(temp->tipo,h[llave].tipo,32);
				temp->edad = h[llave].edad;
				strncpy(temp->raza,h[llave].raza,16);
				temp->estatura = h[llave].estatura;
				temp->peso = h[llave].peso;
				temp->sexo = h[llave].sexo;
				r = fwrite(temp,sizeof(struct nodo),1,ap);
				fclose(ap);
   				if(r != 1){perror("No se puede escribir en el archivo"); exit(-1);}
				system("gedit historia.txt");
				break;
			}else{
				temp = h[llave].next;
				while(temp!=NULL){
					if(temp->id==ver){
					r = fwrite(temp,sizeof(struct nodo),1,ap);
					fclose(ap);
   					if(r != 1){perror("No se puede escribir en el archivo"); exit(-1);}
					system("gedit historia.txt");
					paso=1;				
					break;
					}else{
						temp = temp->next;				
					}
				}
			}
		}
        if(paso==0)printf("No existe registro, intenta de nuevo: ");
	}while(paso==0);
    free(temp);
}

void calcular(struct nodo *next){

	char nombre[32];
    int o;
    //Asignación nombre
    for(o=0;o<32;o++){
        nombre[o] = toupper(next->nombre[o]);
    }
    //Asignación tipo
    char tipo[32];
    for(o=0;o<32;o++){
        tipo[o] = toupper(next->tipo[o]);
    }
    //Asignación raza
    int edad = next->edad;
    char raza[16];
    for(o=0;o<16;o++){
        raza[o] = toupper(next->raza[o]);
    }
	strncpy(raza,next->raza,16);
	int estatura = next->estatura;
	int peso = next->peso;
	char sexo = next->sexo;
	int key = next->key;
	int llave = key%1721;
	int i;
	while(i>=0){
	if((h[llave]).key==0){
		h[llave].key = key;
   		strncpy(h[llave].nombre,nombre,32);
		strncpy(h[llave].tipo,tipo,32);
		h[llave].edad = edad;
		strncpy(h[llave].raza,raza,16);
		h[llave].estatura = estatura;
		h[llave].peso = peso;
		h[llave].sexo=sexo;
		h[llave].id=llave*6000;
		h[llave].next = NULL;
		break;
	}else{
		struct nodo * datos;
		datos = (struct nodo *)malloc(sizeof(struct nodo));
		strncpy(datos->nombre,nombre,32);
		strncpy(datos->tipo,tipo,32);
		datos->edad = edad;
		strncpy(datos->raza,raza,16);
		datos->estatura = estatura;
		datos->peso = peso;
		datos->sexo = sexo;
		datos->next = NULL;
		if(strcmp(h[llave].nombre,nombre)==0){
			if((h[llave].next)==NULL){
				datos->id = (llave*6000)+1;
				datos->key = key;				
				h[llave].next = datos;
			}else{
				struct nodo * temp;
				temp= (struct nodo *)malloc(sizeof(struct nodo));
				temp = h[llave].next;
				int k=1;
				int n=1;
				do{
					n++;
					if(temp->next==NULL){
						datos->key = key;
						datos->id = ((llave*6000)+n);
						temp->next=datos;
						k=1;
						break;
					}else{
						temp = temp->next;	
					}
				}while(k>=0);
				if(k==1){
					break;
				}
			}			
		}else{
			if(llave==1720){
				llave = 0;
			}else{
			llave= llave+1;
			}
		}
	}
	}
}

void mostrar(){
	
    char name[32];
	char nombre[32];
	int i=0;
	int id=0;
	int o = 0;
	int m =0;
	struct nodo * next;
	struct nodo * temp;
	next = (struct nodo *)malloc(sizeof(struct nodo));
	temp = (struct nodo *)malloc(sizeof(struct nodo));
	do{
		printf("Ingrese el nombre de la mascota: ");
		scanf("%s",nombre);
		for(o=0;o<32;o++){
			name[o] = toupper(nombre[o]);
		}
		int key = convertir(name);
		int llave = key%1721;
		strcat(name," ");
		if(strcmp(name,"TODOS ")==0){
		    printf("\nPosicion\tCodigo\t\tId\t\tnombre\n");
			for(i=0;i<1721;i++){
				if((h[i].key != 0)){
		            //mostrarHash(i);
					printf("%d\t\t%d\t\t%d\t\t%s\n",i,h[i].key,h[i].id,h[i].nombre);
					if(h[i].next!=NULL){
						next = h[i].next;
						temp = h[i].next;
						printf("%d\t\t%d\t\t%d\t\t%s\n",i,next->key,next->id,next->nombre);
						while(next->next!=NULL){
							next = next->next;
							printf("%d\t\t%d\t\t%d\t\t%s\n",i,next->key,next->id,next->nombre);
						}
					}
				
				}
			}
			m=1;
		}else{
			int i=0;
			while(i<2000){
				if(strcmp(h[llave].nombre,name)==0){
					printf("\nPosicion\tCodigo\t\tId\t\tnombre\n");
					printf("%d\t\t%d\t%d\t%s\n",llave,h[llave].key,h[llave].id,h[llave].nombre);
					if(h[llave].next!=NULL){
						next = h[llave].next;
						printf("%d\t\t%d\t%d\t%s\n",llave,next->key,next->id,next->nombre);
						while(next->next!=NULL){
							next = next->next;
							printf("%d\t\t%d\t%d\t%s\n",llave,next->key,next->id,next->nombre);
						}
					}
		            i = 1;
					m=1;
		            break;
				}else{
					if(llave==1720){
						llave = 0;
					}else{
						llave+=1;
					}
				}
				i++;
			}
		}
	}while(m==0);
}

void borrar(){
    FILE *ap;
    ap = fopen("dataDogs.dat","r");
    if(ap == NULL){
        perror("Couldn't open dataDogs.dat on 'borrar' function");
        exit(-1);
    }
    if (TamanioArchivo(ap) == 0){
        printf("No hay registros\n");
        return;
    }
    printf("El numero de registros presentes es de %d, ingrese el numero del registro a borrar: ",TamanioArchivo(ap));
	int rem;
	int paso=0;
	struct nodo * next;
	struct nodo * temp;
	temp = (struct nodo *)malloc(sizeof(struct nodo));
	next = (struct nodo *)malloc(sizeof(struct nodo));
	do{	
	scanf("%d",&rem);
	int llave = (int)(rem/6000);
	if(h[llave].id == rem){
		temp = h[llave].next;
		if(temp!=NULL){
			strncpy(h[llave].nombre,temp->nombre,32);
			strncpy(h[llave].tipo,temp->tipo,32);
			h[llave].edad = temp->edad;
			strncpy(h[llave].raza,temp->raza,16);
			h[llave].estatura = temp->estatura;
			h[llave].peso = temp->peso;
			h[llave].sexo = temp->sexo;
			h[llave].next = temp->next;
			h[llave].id = temp->id;
			h[llave].key = temp->key;
		}else{
			h[llave].key = 0;
		}
		break;
	}else{
		next = h[llave].next;
		if(next!=NULL){
			if(next->id==rem){
				if(next->next!=NULL){
					h[llave].next = next->next;
				}else{
					h[llave].next = NULL;
				}
				paso=1;
			}else{
				temp = h[llave].next;
				next=next->next;	
				while(next!=NULL){
					if(next->id==rem){
						if(next->next!=NULL){
							temp->next = next->next;
						}else{
							temp->next = NULL;
						}
						paso=1;
						break;
					}else{
						temp = next;
						if(next->next!=NULL){
							next = next->next;
						}else{
							break;
						}						
					}
				}
				}
			}	
		}
		if(paso==0)printf("No existe registro, intenta de nuevo: ");
	}while(paso==0);
}

//FUNCION QUE CONVIERTE EL NOMBRE EN LLAVE
int convertir(char nombre[32]){
    int i;
     int valor=0;
       if((strlen(nombre))<4){
       for(i=0;i<(strlen(nombre)-1);i++){
            valor*=100;
            valor += (int)toupper(nombre[i]);
        }
       }else{
       for(i=0;i<4;i++){
            valor*=100;
            valor += (int)toupper(nombre[i]);
        }
       }
    if(valor < 0){
        return (-1*valor);
    }
    else{
        return valor;
    }
}

void cargarHash(){

    FILE *ap;
    struct nodo *next = malloc(sizeof(struct nodo));
    if(next == NULL){
        perror("Couldn't allocate memory for next in cargarhash");
        exit(-1);
    }
    ap = fopen("dataDogs.dat","r");
    if(ap == NULL){
        perror("Couldn't open datadogs.dat on cargarHash");
        exit(-1);
    }
    if(TamanioArchivo(ap) == 0){
        printf("No hay registros en el archivo");
        return;
    }else{printf("TAMANO: %d",TamanioArchivo(ap));}
    int i,r = 0;
    printf("\nCargando Hash.....");
    for(i=0;i<TamanioArchivo(ap);i++){
        fseek(ap, (i)*sizeof(struct nodo), SEEK_SET);
        r = fread(next,sizeof(struct nodo),1,ap);
        next->key = convertir(next->nombre);
        //printf("\nNOMBRE%s\n",next->nombre);
        //printf("\nKEY%d\n",next->key);
        if (r == -1) { // ERRORS
            perror("No se puede leer el archivo");
            exit(-1);
        }
        calcular(next);
        //free(next);
    }
    printf("\nTerminado\nß");
    fclose(ap);
}

void save(void *ap){

    FILE *apf;
    int r;
    apf = fopen("dataDogs.dat","a+");
    if(apf == NULL){perror("No se puede crear o abrir el archivo"); exit(-1);}
    r = fwrite(ap,sizeof(struct nodo),1,apf);
    if(r != 1){perror("No se puede escribir en el archivo"); exit(-1);}
    r = fclose(apf);
    if (r != 0) {perror("No se puede cerrar el flujo"); exit(-1);}
};

void generar(){

    printf("¿Cuantos registros aleatorios desea Crear?: ");
    int reg;
    scanf("%i",&reg);
    struct nodo *perro;
    perro = malloc(sizeof(struct nodo));
    int count = 0;
    FILE *apf;
    
    apf = fopen("nombresMascotas.txt","r");
    if (apf == NULL) {
        perror("Can't open petnames file stream");
        exit(-1);
    }
    int s,u;
    struct names nombres[1716];
    for(s=0;s<1716;s++){
        fgets(nombres[s].nombre,32,apf);
        for (u=0;u<32;u++){         //Se mira si los nombres tienen saltos de línea
            if(nombres[s].nombre[u] == '\n'){
                nombres[s].nombre[u] = ' ';
            }
        }
    }
    apf = fopen("razas.txt","r");
    if (apf == NULL) {
        perror("Can't open breeds file stream");
        exit(-1);
    }
    struct breeds razas[52];
    for(s=0;s<52;s++){
        fgets(razas[s].breed,16,apf);
        for (u=0;u<16;u++){         //Se mira si las razas tienen saltos de línea
            if(razas[s].breed[u] == '\n'){
                razas[s].breed[u] = ' ';
            }
        }
    }
    struct types tipos[4];
    strncpy(tipos[0].tipo,"Perro",32); for (u=0;u<32;u++){ if(tipos[0].tipo[u] == '\n'){tipos[0].tipo[u] = ' ';}}
    strncpy(tipos[1].tipo,"Gato",32); for (u=0;u<32;u++){ if(tipos[1].tipo[u] == '\n'){tipos[1].tipo[u] = ' ';}}
    strncpy(tipos[2].tipo,"Loro",32); for (u=0;u<32;u++){ if(tipos[2].tipo[u] == '\n'){tipos[2].tipo[u] = ' ';}}
    strncpy(tipos[3].tipo,"Tortuga",32); for (u=0;u<32;u++){ if(tipos[3].tipo[u] == '\n'){tipos[3].tipo[u] = ' ';}}
    fclose(apf);
    
    printf("\nREGISTROS CARGADOS\n");
    
    char raza[16];
    char name[32];
    char tipo[32];
    
    //ITERACION RANDOM
    for(int i=0;i<reg;i++){
        
        //ASIGNACION EN NODO
        strncpy(perro->nombre,nombres[rand() % 1716].nombre,32);
		perro->key = convertir(perro->nombre);
        perro->id = 0;
        //perro->key = convertir(perro->nombre);
        strncpy(perro->tipo,tipos[rand() % 4].tipo,32);
        perro->edad = rand() % 20;
        strncpy(perro->raza,razas[rand() % 52].breed,16);
        perro->estatura = rand() % 50;
        perro->peso = rand() % 60;
        perro->sexo = "HM"[random () % 2];
		calcular(perro);
        save(perro);
    }
}

void borrarArchivo(){
    
    system("clear");
    FILE *tmp, *fp; //Creamos un archivo temporal.
    int i; //Variable para el ciclo
    int aEliminar; //Número del perro a eliminar
    struct nodo *perro = malloc(sizeof(struct nodo));
    fp=fopen("dataDogs.dat","r");
    char nom[32];
    if (fp==NULL){
        printf("No se encontro el archivo dataDogs.dat");
        return;
    }
    int cantidadListas = TamanioArchivo(fp);
    
    if (cantidadListas == 0){
        printf("No hay registros\n");
    }
    else{
        printf("Hay una cantidad de %i registros. Cual desea eliminar?: ", cantidadListas);
        scanf("%i",&aEliminar);
        while(aEliminar>cantidadListas){
            printf("El registro no existe. Por favor, intente otra vez: ");
            scanf("%i", &aEliminar);
            printf("%i",aEliminar);
        }
        aEliminar = aEliminar-1;
        tmp = fopen("dataDogs.tmp", "a+");
        
        for(i = 0 ; i < cantidadListas ; i++){
            fseek(fp, (i)*sizeof(struct nodo),SEEK_SET);//mueve fp hasta el registro i
            if(i!=(aEliminar)){//guarda los registros diferentes al que se va a elminar en el archivo temporal
                fread(perro, sizeof(struct nodo), 1, fp);
                fwrite(perro, sizeof(struct nodo), 1,tmp);
                rewind(fp);
            }else{
                fread(perro, sizeof(struct nodo), 1, fp);
                rewind(fp);
            }
        }
        
        fclose(fp);
        fclose(tmp);
        system("rm dataDogs.dat"); //Borramos el archivo original
        fp=fopen("dataDogs.dat","a+");
        tmp=fopen("dataDogs.tmp","r+");
        //Hacemos el proceso a la inversa, copiamos los elementos de temporal al archivo original.
        for(i = 0 ; i < (cantidadListas-1) ; i++){
            fseek(fp, (i)*sizeof(struct nodo),SEEK_SET);
            fread(perro, sizeof(struct nodo), 1, tmp);
            fwrite(perro, sizeof(struct nodo), 1,fp);
        }
        remove("dataDogs.tmp");
        fclose(fp);
        fclose(tmp);
        printf("El registro ha sido exitosamente eliminado.\n");
    }
   // cargarHash();
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

int TamanioArchivo(FILE *fp){
    fseek(fp, 0, SEEK_END);
    int Total=ftell(fp)/sizeof(struct nodo);
    return Total;
};

