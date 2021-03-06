#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>

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
void escribirHash();
void historiaClinica();

struct names
{
    char nombre[32];
};
struct types
{
    char tipo[32];
};
struct breeds
{
    char breed[16];
};

struct nodo
{
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
    struct nodo *next;
    struct nodo * final;
} nodo;
struct nodo h[2029];

void verRegistro(int sockEntrada, int vere)
{
    int tam, i;
    char tamano[3];
    char num[3];
    int ver = vere;
    int paso = 0;
    struct nodo *temp;
    temp = (struct nodo *)malloc(sizeof(struct nodo));
    int r;
    int id;
    char nombre[32];
    char tipo[32];
    int edad;
    char ed[3];
    char raza[16];
    int estatura;
    char esta[3];
    float peso;
    char pe[3];
    char sexo;
    char texto[120] = "\0";
    do
    {
		char namefile[50], iden[8], salida[50];
		sprintf(iden, "%d", ver);
		strncpy(namefile, "historia", 15);
		strcat(namefile, iden);
		strcat(namefile, ".txt");
		int llave = (int)(ver / 6000);
		if (h[llave].key != 0)
		{
			if (h[llave].id == ver)
			{
				historiaClinica(sockEntrada,h[llave].nombre,h[llave].id);
				break;
			}
			else
			{
				temp = h[llave].next;
				while (temp != NULL)
				{
					if (temp->id == ver)
					{
					historiaClinica(sockEntrada,temp->nombre,temp->id);
					paso = 1;
					break;
					}
					else
					{
					temp = temp->next;
					}
				}
			}
		}
		if (paso == 0)
		//send(sockEntrada, "002", 3, 0);
		send(sockEntrada, "No", 120, 0);
		break;
    } while (paso == 0);
    //free(temp);
}

void historiaClinica(int sockEntrada,char nombre[32],int id){
	char NombreH[40] = "Historia";
	char nombreS[50] = "Historia Clinica: ";
	char texto[200] = "\0";
	char idS[8];
	char hist[1000];
	sprintf(idS,"%d",id);
	strcat(NombreH,idS);
	strcat(NombreH,".txt");
	FILE *ap;
	ap = fopen(NombreH,"r");
	if(ap==NULL){
		printf("No existe");
		strcat(nombreS,nombre);
		ap = fopen(NombreH,"w+");
		fwrite(nombreS,strlen(nombreS),1,ap);
		//fclose(ap);
		send(sockEntrada, NombreH, 120, 0);
		strncpy(hist,nombreS,50);
		send(sockEntrada, hist, 1000, 0);
		recv(sockEntrada,hist,1000,0);
		printf("%s",hist);
	}else{
		
		printf("existe");
		fread(hist,sizeof(hist),1,ap);
		//fclose(ap);
		send(sockEntrada, NombreH, 120, 0);
		send(sockEntrada, hist, 1000, 0);
		recv(sockEntrada,hist,1000,0);
	}
	fclose(ap);
	FILE *apf;
	apf = fopen(NombreH,"w+");
	fwrite(hist,strlen(hist),1, apf);
	fclose(apf);
	int tama = sizeof(apf);
	fflush(stdout);
	}

void calcular(struct nodo *next){

    char nombre[32];
    int o;
    //Asignación nombre
    for (o = 0; o < 32; o++)
    {
	nombre[o] = toupper(next->nombre[o]);
    }
    //Asignación tipo
    char tipo[32];
    for (o = 0; o < 32; o++)
    {
	tipo[o] = toupper(next->tipo[o]);
    }
    //Asignación raza
    int edad = next->edad;
    char raza[16];
    for (o = 0; o < 16; o++)
    {
	raza[o] = toupper(next->raza[o]);
    }
    strncpy(raza, next->raza, 16);
    int estatura = next->estatura;
    float peso = next->peso;
    char sexo = next->sexo;
    int key = next->key;
    int llave = key % 2029;
    int i;
    struct nodo *datos;
    datos = (struct nodo *)malloc(sizeof(struct nodo));
    strncpy(datos->nombre, nombre, 32);
    strncpy(datos->tipo, tipo, 32);
    datos->edad = edad;
    strncpy(datos->raza, raza, 16);
    datos->estatura = estatura;
    datos->peso = peso;
    datos->sexo = sexo;
    datos->next = NULL;
    datos->cuantos = 0;
    while (i >= 0)
    {
	if ((h[llave]).key == 0)
	{
	    h[llave].key = key;
	    strncpy(h[llave].nombre, nombre, 32);
	    strncpy(h[llave].tipo, tipo, 32);
	    h[llave].edad = edad;
	    strncpy(h[llave].raza, raza, 16);
	    h[llave].estatura = estatura;
	    h[llave].peso = peso;
	    h[llave].sexo = sexo;
	    h[llave].id = llave * 6000;
	    h[llave].next = NULL;
	    h[llave].cuantos = 0;
	    h[llave].final = NULL;
	    break;
	}
	else
	{
	    if (strcmp(h[llave].nombre, nombre) == 0)
	    {
		if ((h[llave].next) == NULL)
		{
		    datos->id = (llave * 6000) + 1;
		    datos->key = key;
		    h[llave].next = datos;
		    h[llave].final = datos;
		    h[llave].cuantos += 1;
		    break;
		}
		else
		{
		    struct nodo *temp;
		    temp = (struct nodo *)malloc(sizeof(struct nodo));
		    temp = h[llave].final;
		    h[llave].cuantos += 1;
		    h[llave].final = datos;
		    temp->next = datos;
		    datos->id = (llave * 6000) + h[llave].cuantos;
		    datos->key = key;
		    break;
		}
	    }
	    else
	    {
		if (llave == 2029)
		{
		    llave = 0;
		}
		else
		{
		    llave = llave + 1;
		}
	    }
	}
    }
}

void mostrar(int sockEntrada, char perri[32])
{

    char name[32];
    char nombre[32];
    int i = 0;
    int id = 0;
    int o = 0;
    int m = 0;
    struct nodo *next;
    struct nodo *temp;
    next = (struct nodo *)malloc(sizeof(struct nodo));
    temp = (struct nodo *)malloc(sizeof(struct nodo));
    do
    {
		for (o = 0; o < 32; o++)
		{
			name[o] = toupper(perri[o]);
		}
		int key = convertir(name);
		int llave = key % 2029;
		strcat(name, " ");
		char perro[200];
		if (strcmp(name, "TODOS ") == 0)
		{
			printf("\nPosicion\tCodigo\t\tId\t\tnombre\n");
			for (i = 0; i < 2029; i++)
			{
				if ((h[i].key != 0))
				{
					sprintf(perro, "%d\t\t%d\t\t%d\t\t%s\n", i, h[i].key, h[i].id, h[i].nombre);
					send(sockEntrada, perro, 200, 0);
					if (h[i].next != NULL)
					{
						next = h[i].next;
						temp = h[i].next;
						sprintf(perro, "%d\t\t%d\t\t%d\t\t%s\n", i, next->key, next->id, next->nombre);
						send(sockEntrada, perro, 200, 0);
						while (next->next != NULL)
						{
							next = next->next;
							sprintf(perro, "%d\t\t%d\t\t%d\t\t%s\n", i, next->key, next->id, next->nombre);
							send(sockEntrada, perro, 200, 0);
						}
					}
				}
			}
			m = 1;
		}
		else
		{
			int i = 0;
			while (i < 2000)
			{
				if ((strcmp(h[llave].nombre, name) == 0) && (h[llave].key != 0))
				{
					//printf("\nPosicion\tCodigo\t\tId\t\tnombre\n");
					sprintf(perro, "%d\t\t%d\t%d\t%s\n", llave, h[llave].key, h[llave].id, h[llave].nombre);
					send(sockEntrada, perro, 200, 0);
					if (h[llave].next != NULL)
					{
					next = h[llave].next;
					sprintf(perro, "%d\t\t%d\t%d\t%s\n", llave, next->key, next->id, next->nombre);
					send(sockEntrada, perro, 200, 0);
					while (next->next != NULL)
					{
						next = next->next;
						sprintf(perro, "%d\t\t%d\t%d\t%s\n", llave, next->key, next->id, next->nombre);
						send(sockEntrada, perro, 200, 0);
					}
					}
					i = 1;
					m = 1;
					break;
				}
				else
				{
					if (llave == 2029)
					{
						llave = 0;
					}
					else
					{
						llave += 1;
					}
				}
				i++;
			}
		}

		if (m != 1)
		{
			send(sockEntrada, "unable", 200, 0);
			//Servidor(void* arg);
			break;
		}
    } while (m == 0);
    send(sockEntrada, "salir", 200, 0);
}

void borrar(int sockEntrada, int IdSa)
{
    FILE *ap;
    ap = fopen("dataDogs.dat", "r");
    if (ap == NULL)
    {
	perror("Couldn't open dataDogs.dat on 'borrar' function");
	exit(-1);
    }
    if (TamanioArchivo(ap) == 0)
    {
	printf("No hay registros\n");
	return;
    }
    int rem;
    int paso = 0;
    struct nodo *next;
    struct nodo *temp;
    temp = (struct nodo *)malloc(sizeof(struct nodo));
    next = (struct nodo *)malloc(sizeof(struct nodo));
    int i = 0;
    do
    {
	i = IdSa;
	//printf("i %d \n", i);
	rem = i;
	int llave = (int)(rem / 6000);
	if (h[llave].id == rem)
	{
	    temp = h[llave].next;
	    if (temp != NULL)
	    {
		strncpy(h[llave].nombre, temp->nombre, 32);
		strncpy(h[llave].tipo, temp->tipo, 32);
		h[llave].edad = temp->edad;
		strncpy(h[llave].raza, temp->raza, 16);
		h[llave].estatura = temp->estatura;
		h[llave].peso = temp->peso;
		h[llave].sexo = temp->sexo;
		h[llave].next = temp->next;
		h[llave].id = temp->id;
		h[llave].key = temp->key;
	    }
	    else
	    {
		h[llave].key = 0;
		h[llave].id = 0;
	    }
	    //printf("borrado");
	    borrarArchivo(rem);
	    send(sockEntrada, "si", 2, 0);
	    paso = 1;
	    break;
	}
	else
	{
	    next = h[llave].next;
	    if (next != NULL)
	    {
		if (next->id == rem)
		{
		    if (next->next != NULL)
		    {
			h[llave].next = next->next;
		    }
		    else
		    {
			h[llave].next = NULL;
		    }
		    //printf("borrado");
		    borrarArchivo(rem);
		    send(sockEntrada, "si", 2, 0);
		    paso = 1;
		}
		else
		{
		    temp = h[llave].next;
		    next = next->next;
		    while (next != NULL)
		    {
			if (next->id == rem)
			{
			    if (next->next != NULL)
			    {
				temp->next = next->next;
			    }
			    else
			    {
				temp->next = NULL;
			    }
			    paso = 1;
			    //printf("borrado");
			    borrarArchivo(rem);
			    send(sockEntrada, "si", 2, 0);
			    break;
			}
			else
			{
			    temp = next;
			    if (next->next != NULL)
			    {
				next = next->next;
			    }
			    else
			    {
				break;
			    }
			}
		    }
		}
	    }
	}
	if (paso == 0)
	{
	    send(sockEntrada, "no", 2, 0);
	}
    } while (paso == 0);
    //borrarArchivo(IdSa);
    //printf("Escritura terminada");
}

int convertir(char nombre[32])
{
    int i;
    int valor = 0;
    if ((strlen(nombre)) < 4)
    {
	for (i = 0; i < (strlen(nombre) - 1); i++)
	{
	    valor *= 100;
	    valor += (int)toupper(nombre[i]);
	}
    }
    else
    {
	for (i = 0; i < 4; i++)
	{
	    valor *= 100;
	    valor += (int)toupper(nombre[i]);
	}
    }
    if (valor < 0)
    {
	return (-1 * valor);
    }
    else
    {
	return valor;
    }
}

void cargarHash()
{

    FILE *ap;
    struct nodo *next = malloc(sizeof(struct nodo));
    if (next == NULL)
    {
	perror("Couldn't allocate memory for next in cargarhash");
	exit(-1);
    }
    ap = fopen("dataDogs.dat", "r");
    if (ap == NULL)
    {
	perror("Couldn't open datadogs.dat on cargarHash");
	exit(-1);
    }
    if (TamanioArchivo(ap) == 0)
    {
	printf("No hay registros en el archivo");
	return;
    }
    else
    {
	printf("TAMANO: %d", TamanioArchivo(ap));
    }
    int i, r = 0;
    printf("\nCargando Hash.....");
    for (i = 0; i < TamanioArchivo(ap); i++)
    {
	fseek(ap, (i) * sizeof(struct nodo), SEEK_SET);
	r = fread(next, sizeof(struct nodo), 1, ap);
	next->key = convertir(next->nombre);
	if (r == -1)
	{ // ERRORS
	    perror("No se puede leer el archivo en cargarhash");
	    exit(-1);
	}
	calcular(next);
    }
    printf("\nTerminado\nß");
    fclose(ap);
}

void save(void *ap)
{

    FILE *apf;
    int r;
    apf = fopen("dataDogs.dat", "a+");
    if (apf == NULL)
    {
	perror("No se puede crear o abrir el archivo");
	exit(-1);
    }
    r = fwrite(ap, sizeof(struct nodo), 1, apf);
    if (r != 1)
    {
	perror("No se puede escribir en el archivo");
	exit(-1);
    }
    r = fclose(apf);
    if (r != 0)
    {
	perror("No se puede cerrar el flujo");
	exit(-1);
    }
};

void generar(int reg)
{
    struct nodo *perro;
    perro = malloc(sizeof(struct nodo));
    int count = 0;
    FILE *apf;

    apf = fopen("nombresMascotas.txt", "r");
    if (apf == NULL)
    {
	perror("Can't open petnames file stream");
	exit(-1);
    }
    int s, u;
    struct names nombres[1716];
    for (s = 0; s < 1716; s++)
    {
	fgets(nombres[s].nombre, 32, apf);
	for (u = 0; u < 32; u++)
	{ //Se mira si los nombres tienen saltos de línea
	    if (nombres[s].nombre[u] == '\n')
	    {
		nombres[s].nombre[u] = ' ';
		break;
	    }
	}
    }
    apf = fopen("razas.txt", "r");
    if (apf == NULL)
    {
	perror("Can't open breeds file stream");
	exit(-1);
    }
    struct breeds razas[52];
    for (s = 0; s < 52; s++)
    {
	fgets(razas[s].breed, 16, apf);
	for (u = 0; u < 16; u++)
	{ //Se mira si las razas tienen saltos de línea
	    if (razas[s].breed[u] == '\n')
	    {
		razas[s].breed[u] = ' ';
		break;
	    }
	}
    }
    struct types tipos[4];
    strncpy(tipos[0].tipo, "Perro", 32);
    strncpy(tipos[1].tipo, "Gato", 32);
    strncpy(tipos[2].tipo, "Loro", 32);
    strncpy(tipos[3].tipo, "Tortuga", 32);

    int r;
    apf = fopen("dataDogs.dat", "a+");
    if (apf == NULL)
    {
	perror("No se puede crear o abrir el archivo");
	exit(-1);
    }

    //ITERACION RANDOM
    for (int i = 0; i < reg; i++)
    {

	//ASIGNACION EN NODO
	strncpy(perro->nombre, nombres[rand() % 1716].nombre, 32);
	perro->key = convertir(perro->nombre);
	perro->id = 0;
	strncpy(perro->tipo, tipos[rand() % 4].tipo, 32);
	perro->edad = rand() % 20;
	strncpy(perro->raza, razas[rand() % 52].breed, 16);
	perro->estatura = rand() % 50;
	perro->peso = rand() % 60;
	perro->sexo = "HM"[random() % 2];
	calcular(perro);
	r = fwrite(perro, sizeof(struct nodo), 1, apf);
	if (r != 1)
	{
	    perror("No se puede escribir en el archivo");
	    exit(-1);
	}
    }
    r = fclose(apf);
    if (r != 0)
    {
	perror("No se puede cerrar el flujo");
	exit(-1);
    }
}

void borrarArchivo(int aEliminar)
{

    system("clear");
    FILE *tmp, *fp; //Creamos un archivo temporal.
    int i;	  //Variable para el ciclo
    struct nodo *perro = malloc(sizeof(struct nodo));
    fp = fopen("dataDogs.dat", "r");
    char nom[32];
    if (fp == NULL)
    {
	printf("No se encontro el archivo dataDogs.dat");
	return;
    }
    int cantidadListas = TamanioArchivo(fp);

    if (cantidadListas == 0)
    {
	printf("No hay registros\n");
    }
    else
    {
	aEliminar = aEliminar - 1;
	tmp = fopen("dataDogs.tmp", "a+");

	for (i = 0; i < cantidadListas; i++)
	{
	    fseek(fp, (i) * sizeof(struct nodo), SEEK_SET); //mueve fp hasta el registro i
	    if (i != (aEliminar))
	    { //guarda los registros diferentes al que se va a elminar en el archivo temporal
		fread(perro, sizeof(struct nodo), 1, fp);
		fwrite(perro, sizeof(struct nodo), 1, tmp);
		rewind(fp);
	    }
	    else
	    {
		fread(perro, sizeof(struct nodo), 1, fp);
		rewind(fp);
	    }
	}

	fclose(fp);
	fclose(tmp);
	system("rm dataDogs.dat"); //Borramos el archivo original
	fp = fopen("dataDogs.dat", "a+");
	tmp = fopen("dataDogs.tmp", "r+");
	//Hacemos el proceso a la inversa, copiamos los elementos de temporal al archivo original.
	for (i = 0; i < (cantidadListas - 1); i++)
	{
	    fseek(fp, (i) * sizeof(struct nodo), SEEK_SET);
	    fread(perro, sizeof(struct nodo), 1, tmp);
	    fwrite(perro, sizeof(struct nodo), 1, fp);
	}
	remove("dataDogs.tmp");
	fclose(fp);
	fclose(tmp);
    }
    printf("archivo borrado exitosamente");
}

int TamanioArchivo(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    int Total = ftell(fp) / sizeof(struct nodo);
    return Total;
};

void escribirHash()
{

    system("rm dataDogs.dat");
    struct nodo *next;
    struct nodo *temp;
    next = (struct nodo *)malloc(sizeof(struct nodo));
    temp = (struct nodo *)malloc(sizeof(struct nodo));
    int r, i;
    FILE *ap;
    ap = fopen("dataDogs.dat", "a+");
    if (ap == NULL)
    {
	perror("Couldn't open dataDogs.dat in escribirHash");
    }
    for (i = 0; i < 2029; i++)
    {
	if ((h[i].key != 0))
	{
	    if (h[i].next != NULL)
	    {
		next = h[i].next;
		r = fwrite(next, sizeof(struct nodo), 1, ap);
		while (next->next != NULL)
		{
		    next = next->next;
		    r = fwrite(next, sizeof(struct nodo), 1, ap);
		}
	    }
	}
    }
}

int entero(char num[8])
{
    int tam = strlen(num);
    int i, salida = 0;
    int ayuda = 0;
    for (i = 0; i < tam; i++)
    {
	salida = salida * 10;
	ayuda = num[i] - 48;
	salida += ayuda;
    }
    return salida;
}

void *Servidor(void *arg)
{
    FILE *ap;
    char IP[30] = "\0";
    char tam[2];
    char opcion[2];
    char nombre[6] = "";
    char tipo[6] = "";
    char edad[2] = "";
    char raza[6] = "";
    char estatura[3] = "";
    char peso[2] = "";
    char paso[2] = "";
    char o;
    char id[8] = "\0";
    char tempo[10] = "\0";
    int r, key;
    int tamano, IdSa;
    int sockEntrada = *(int *)arg;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    printf("Mensage... ");
	struct nodo *temp;
    while (1)
    {
		read(sockEntrada, opcion, 2);
		o = opcion[0];
		if (o == '1')
		{
			//printf("\n1\n");
			temp = malloc(sizeof(struct nodo));
			int ta=sizeof(struct nodo);
			//printf("datos -%d",ta);
			recv(sockEntrada, temp, sizeof(struct nodo), 0);
			//printf("Recibido");
			fflush(stdout);
			key = convertir(temp->nombre);
			temp->key = key;
			temp->id = 0;
			calcular(temp);
			save(temp);
			//printf("guardo");
			fflush(stdout);
			write(sockEntrada, "Registro guardado", 17);
			ap = fopen("serverDogs.log", "a+");
			if (ap == NULL)
			{
			perror("No se pudo crear el serverDogs.log: ");
			exit(-1);
			}
			char output[128];
			strftime(output, 128, "|Fecha 20%y%m%d%H%M%S|cliente|", tlocal);
			sprintf(IP, "%d", sockEntrada);
			strcat(output, IP);
			strcat(output, "|Ingresar|");
			strcat(output, temp->nombre);
			strcat(output, "|");
			//printf("%s\n", output);
			fwrite(output,sizeof(output),1, ap);
			fclose(ap);
		}
		else if (o == '2')
		{

			//printf("\n2\n");
			char ceros[10] = "";
			//ENVIAR TAMANO
			ap = fopen("dataDogs.dat", "r");
			if (ap == NULL)
			{
				perror("Can't open dataDogs.dat in verRegistro");
				exit(-1);
			}
			int size = TamanioArchivo(ap);
			fclose(ap);
			char sizeD[10];
			sprintf(sizeD, "%d", size);
			tamano = strlen(sizeD);
			int p;
			if (tamano < 10)
			{
				for (p = 0; p < (10 - tamano); p++)
				{
					strcat(ceros, "0");
				}
			}
			strcat(ceros, sizeD);
			send(sockEntrada, ceros, 10, 0);
			//printf("\ntamaño %s \n", ceros);
			//FIN ENVIAR TAMANO

			//RECIBIR ID
			do
			{
				char idmas[8] = "";
				int i = recv(sockEntrada, idmas, 8, 0);
				//printf("\n%s\n", idmas);
				char idmas3[8];
				i=0;
				while(idmas[i]=='0'){
					i++;
				}
				int q;
				for (q = i; q < 9; q++){
					idmas3[q-i] = idmas[q];
				}

				sscanf(idmas3, "%d", &i);
				printf("i %d \n", i);
				//FIN RECIBIR ID

			//ENVIAR TEXTO
				memcpy(id, tempo, 8);
				IdSa = i;
				verRegistro(sockEntrada, IdSa);
				recv(sockEntrada, paso, 2, 0);
			} while (strcmp(paso, "No") == 0);
			ap = fopen("serverDogs.log", "a+");
			if (ap == NULL)
			{
				perror("No se pudo crear el serverDogs.log: ");
				exit(-1);
			}
			char output[120];
			char idS[8];
			sprintf(idS, "%d", IdSa);
			strftime(output, 128, "|Fecha 20%y%m%d%H%M%S|cliente|", tlocal);
			sprintf(IP, "%d", sockEntrada);
			strcat(output, IP);
			strcat(output, "|Ver Registro|");
			strcat(output, idS);
			strcat(output, "|");
			//printf("%s\n", output);
			r = fwrite(output,sizeof(output),1, ap);
			fflush(stdout);
			fclose(ap);
			//if(r!=0){perror("No se pudo cargar el registro al serverDogs");}
			//FIN ENVIAR TEXTO
		}
		else if (o == '3')
		{
			char ceros[10] = "";
			//printf("\n3\n");
			ap = fopen("dataDogs.dat", "r");
			if (ap == NULL)
			{
			perror("Can't open dataDogs.dat in verRegistro");
			exit(-1);
			}
			int size = TamanioArchivo(ap);
			fclose(ap);
			char sizeD[10];
			sprintf(sizeD, "%d", size);
			tamano = strlen(sizeD);
			int p;
			if (tamano < 10)
			{
			for (p = 0; p < (10 - tamano); p++)
			{
				strcat(ceros, "0");
			}
			}
			strcat(ceros, sizeD);
			send(sockEntrada, ceros, strlen(ceros), 0);
			//FIN ENVIAR TAMANO

			//RECIBIR ID
			char idmas[8];
			int i = recv(sockEntrada, idmas, 8, 0);
			char idmas3[8];
			for (i = 1; i < 9; i++)
			{
			idmas3[i - 1] = idmas[i];
			}
			i = 0;
			while (idmas3[i] == '0')
			{
			i = i + 1;
			}
			int u = 8 - i;
			char idmas2[u];
			int z = 0;
			for (u = i; u < 8; u++)
			{
			idmas2[z] = idmas3[u];
			z++;
			}
			char ult[sizeof(idmas2)];
			for (i = 0; i < sizeof(idmas2); i++)
			{
			ult[i] = idmas2[i];
			}
			sscanf(ult, "%d", &i);
			//printf("i %d \n",i);
			fflush(stdout);
			//FIN RECIBIR ID

			//ENIVAR TEXTO
			memcpy(id, tempo, 8);
			IdSa = i;
			//printf("%d", IdSa);
			borrar(sockEntrada, i);
			char output[120];
			ap = fopen("serverDogs.log", "a+");
			if (ap == NULL)
			{
				perror("No se pudo crear el serverDogs.log: ");
				exit(-1);
			}
			strftime(output, 128, "|Fecha 20%y%m%d%H%M%S|cliente|", tlocal);
			sprintf(IP, "%d", sockEntrada);
			strcat(output, IP);
			strcat(output, "|Borrar Registro|");
			strcat(output, ult);
			strcat(output, "|");
			//printf("%s\n",output);
			r = fwrite(output, sizeof(output), 1, ap);
			//printf("\nPerro eliminado exitosamente\n");
			fflush(stdout);
			fclose(ap);
		}
		else if (o == '4')
		{
			//printf("\n4\n");
			char perro[32];
			recv(sockEntrada, perro, 32, 0);
			//printf("%s \n", perro);
			mostrar(sockEntrada, perro);
			fflush(stdout);
			char output[120];
			ap = fopen("serverDogs.log", "a+");
			if (ap == NULL)
			{
				perror("No se pudo crear el serverDogs.log: ");
				exit(-1);
			}
			strftime(output, 128, "|Fecha 20%y%m%d%H%M%S|cliente|", tlocal);
			sprintf(IP, "%d", sockEntrada);
			strcat(output, IP);
			strcat(output, "|BUscar Registro|");
			strcat(output, perro);
			strcat(output, "|");
			//printf("%s\n",output);
			r = fwrite(output, sizeof(output), 1, ap);
			fflush(stdout);
			fclose(ap);
		}
		else if (o == '5')
		{
			//printf("\n5\n");
			pthread_exit((void *)0);
		}
		else if (o == '6')
		{
			//printf("\n6\n");
			generar(100000);
		}
	//printf("LLAVE %d", llave);
    }
    fclose(ap);
    close(sockEntrada);
    pthread_exit((void *)0);
}

int main()
{

    int i;
    for (i = 0; i < 2029; i++)
    {
	h[i].key = 0;
    }
    FILE *ap;
    ap = fopen("dataDogs.dat", "r");
    if (ap == NULL)
    {
    }
    else
    {
	cargarHash();
    }

    int clientefd, r, serverfd;
    unsigned int size;
    struct sockaddr_in server;
    struct sockaddr_in cliente;
    serverfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverfd < 0)
    {
	printf("Error en el Socket\n");
	exit(1);
    }
    //generar();
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(6881);
    bzero(server.sin_zero, 8);
    int one = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    r = bind(serverfd, (struct sockaddr *)&server, sizeof(server));
    if (r < 0)
    {
	printf("Error en bind\n");
	exit(1);
    }
    r = listen(serverfd, 5);
    if (r < 0)
    {
	printf("Error en el listen\n");
	exit(1);
    }
	int NumCli = 0;
    while (1){
		if(NumCli<33){
			pthread_t thread;
		size = sizeof(cliente);
		clientefd = accept(serverfd, (struct sockaddr *)&cliente, &size);
		if (clientefd < 0)
		{
			printf("Error en el accept\n");
			exit(1);
		}
		r = pthread_create(&thread, NULL, Servidor, &clientefd);
		if (r != 0)
		{
			printf("Erro na Thread\n");
			exit(1);
		}

		pthread_detach(thread);
		NumCli+=1;
		}
    }
    exit(0);
}
