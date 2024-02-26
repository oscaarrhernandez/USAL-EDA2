#include "dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Crea un fichero hash inicialmente vacio seg�n los criterios especificados en la pr�ctica
// Primera tarea a realizar para  crear un fichero organizado mediante DISPERSI�N
void creaHvacio(char *fichHash)
{
	FILE *fHash;
	tipoCubo cubo;
	int j;
	int numCubos = CUBOS + CUBOSDESBORDE;

	memset(&cubo, 0, sizeof(cubo)); // Inicializa las variables a su valor nulo, enteros a 0 y cadenas a NULL;

	fHash = fopen(fichHash, "wb");
	for (j = 0; j < numCubos; j++)
		fwrite(&cubo, sizeof(cubo), 1, fHash);
	fclose(fHash);
}
// Lee el contenido del fichero hash organizado mediante el m�todo de DISPERSI�N seg�n los criterios
// especificados en la pr�ctica. Se leen todos los cubos completos tengan registros asignados o no. La
// salida que produce esta funci�n permite visualizar el m�todo de DISPERSI�N

int leeHash(char *fichHash)
{
	FILE *f;
	tipoCubo cubo;
	int j, i = 0;
	size_t numLee;

	f = fopen(fichHash, "rb");
	rewind(f);
	fread(&cubo, sizeof(cubo), 1, f);
	while (!feof(f))
	{
		for (j = 0; j < C; j++)
		{
			if (j == 0)
				printf("Cubo %2d (%2d reg. ASIGNADOS)", i, cubo.numRegAsignados);
			else
				printf("\t\t\t");
			if (j < cubo.numRegAsignados)
				printf("\t%s %s %s %s %s\n",
							 cubo.reg[j].dni,
							 cubo.reg[j].nombre,
							 cubo.reg[j].ape1,
							 cubo.reg[j].ape2,
							 cubo.reg[j].provincia);
			else
				printf("\n");
		}
		i++;
		fread(&cubo, sizeof(cubo), 1, f);
	}
	fclose(f);
	return i;
}

///////
int creaHash(char *fichEntrada, char *fichHash)
{
	FILE *fEntrada, *fHash;
	tipoAlumno reg;
	tipoCubo cubo;
	fpos_t pos;
	int i, numDesb = 0;

	creaHvacio(fichHash);

	fEntrada = fopen(fichEntrada, "rb");
	fHash = fopen(fichHash, "r+b");
	fread(&reg, sizeof(reg), 1, fEntrada);

	while (!feof(fEntrada))
	{
		// Procesamiento, la clave es fseek
		// Calcular la funcion para asignar el cubo
		int numCubo = atoi(reg.dni) % CUBOS;
		// printf("%d->%d\n",an, numCubo);
		fseek(fHash, numCubo * sizeof(cubo), SEEK_SET);
		fgetpos(fHash, &pos);
		fread(&cubo, sizeof(cubo), 1, fHash);
		i = cubo.numRegAsignados;
		if (i >= C)
		{
			numDesb++;
			cubo.numRegAsignados++;
			// Tratar desbordes
			if (!desborde(fHash, reg, CUBOS, CUBOSDESBORDE))
				printf("registro no almacenado");
		}
		else
		{
			cubo.reg[i] = reg;
			cubo.numRegAsignados++;
		}
		fsetpos(fHash, &pos); // Me vuelvo a situal en el cubo en el que estaba
		// fseek(fHash,numcubo*sizeof(cubo), SEEK_SET); tambien se puede hacer asi
		fwrite(&cubo, sizeof(cubo), 1, fHash); // Tengo que hacer esto para modificarlo en memoria secundaria

		fread(&reg, sizeof(reg), 1, fEntrada);
	}
	fclose(fEntrada);
	fclose(fHash);
	return numDesb;
}

int desborde(FILE *fHash, tipoAlumno reg, int cubos, int cubosDesborde)
{
	int i, nCubos;
	fpos_t pos;
	tipoCubo cubo;
	memset(&cubo, 0, sizeof(cubo));
	nCubos = cubos;
	while (nCubos < cubos + cubosDesborde)
	{
		fseek(fHash, nCubos * sizeof(cubo), SEEK_SET);
		fgetpos(fHash, &pos);
		fread(&cubo, sizeof(cubo), 1, fHash);
		i = cubo.numRegAsignados;
		if (i < C)
		{
			cubo.reg[i] = reg;
			cubo.numRegAsignados++;
			fsetpos(fHash, &pos);
			fwrite(&cubo, sizeof(cubo), 1, fHash);
			return 1;
		}
		else
		{
			nCubos++;
		}
	}
	return 0;
}
int buscaReg(FILE *fHash, tipoAlumno *reg, char *dni)
{
	int numCubo, i = 0;
	numCubo = atoi(dni) % CUBOS;
	tipoCubo cubo;
	tipoAlumno lista;
	fseek(fHash, 0, SEEK_SET);
	fseek(fHash, numCubo * sizeof(cubo), SEEK_SET);
	while (i < C)
	{
		fread(&lista, sizeof(lista), 1, fHash);
		if (strcmp(lista.dni, dni) == 0)
		{
			*reg = lista;
			return numCubo;
		}
		else
		{
			i++;
		}
	} // Busco en el cubo referenciado
	// Si no está ahí busco en los de desborde
	int cont = 0;
	numCubo = CUBOS;
	while (!feof(fHash))
	{
		i = 0;
		fseek(fHash, numCubo * sizeof(cubo), SEEK_SET);
		while (i < C)
		{
			fread(&lista, sizeof(lista), 1, fHash);
			int comp = strcmp(lista.dni, dni);
			if (comp == 0)
			{
				*reg = lista;
				return numCubo; // CUBOS+(cont%5);
			}
			i++;
		}
		numCubo++;
	}
	return -1;
}
int insertarReg(FILE *f, tipoAlumno *reg)
{
	tipoCubo cubo;
	fpos_t pos;
	int numCubo = atoi(reg->dni) % CUBOS;
	fseek(f, numCubo * sizeof(cubo), SEEK_SET);
	fgetpos(f, &pos);
	fread(&cubo, sizeof(cubo), 1, f);
	int i = cubo.numRegAsignados;
	if (cubo.numRegAsignados < C)
	{
		cubo.reg[i] = *reg;
		cubo.numRegAsignados++;
		printf("ENTRA el:%d\n", cubo.numRegAsignados);
		fsetpos(f, &pos);
		fwrite(&cubo, sizeof(cubo), 1, f);
	}
	else
	{
		// Si al cubo que corresponde ya esta ocupado se coloca en los de desborde
		printf("Desborde\n");
	}
}
