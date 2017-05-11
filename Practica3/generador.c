#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

	strncpy(perro->nombre, nombres[rand() % 1716].nombre, 32);
	perro->key = convertir(perro->nombre);
	perro->id = 0;
	strncpy(perro->tipo, tipos[rand() % 4].tipo, 32);
	perro->edad = rand() % 20;
	strncpy(perro->raza, razas[rand() % 52].breed, 16);
	perro->estatura = rand() % 50;
	perro->peso = rand() % 60;
	perro->sexo = "HM"[random() % 2];
	save(perro);
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

int main(){
	printf("Ingrese el numero de registros a crear\n");
	int num;
	scanf("%d", &num);
	generar(num);
}
