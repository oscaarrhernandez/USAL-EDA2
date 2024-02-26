// dispersion.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dispersion.h"

/*
Lee el contenido del fichero hash organizado mediante el método de DISPERSIÓN según los criterios especificados en
la práctica. Se leen todos los cubos completos tengan registros asignados o no. La salida que produce esta función
permite visualizar el método de DISPERSIÓN.
*/

int leeHash(char *fichHash)
{
	FILE *f;
	tipoCubo cubo;
	regConfig regC;

	int j, densidadOcupacion;
	int nCubo = 0;

	f = fopen(fichHash, "rb");
	fread(&regC, sizeof(regConfig), 1, f);
	fread(&cubo, sizeof(cubo), 1, f);

	while (!feof(f))
	{
		for (j = 0; j < C; j++)
		{
			if (j == 0)
			{
				printf("Cubo %2d (%2d reg. ASIGNADOS)", nCubo, cubo.numRegAsignados);
			} //										  |____| |____________________|
			else if ((j == 1) && cubo.desbordado)
			{
				printf("DESBORDADO\t\t");
			}
			else
			{
				printf("\t\t\t");
			}

			if (j < cubo.numRegAsignados)
			{
				printf("\t%s %s %s %s %s\n", cubo.reg[j].dni, cubo.reg[j].nombre, cubo.reg[j].ape1, cubo.reg[j].ape2, cubo.reg[j].provincia);
			} //							   |______________| |_________________| |_______________| |_______________| |_____________________|
			else
			{
				printf("\n");
			}
		}

		nCubo++;
		fread(&cubo, sizeof(cubo), 1, f);
	}

	fclose(f);

	printf("ORGANIZACION METODO DISPERSION:\n\t %d CUBOS y %d CUBOS DESBORDE con capacidad %d\n", regC.nCubos, regC.nCubosDes, C);
	printf("\t Contiene %d registros de los cuales se han desbordado: %d\n\n", regC.numReg, regC.numRegDes);

	densidadOcupacion = 100 * regC.numReg / ((regC.nCubos + regC.nCubosDes) * C);
	printf("Densidad ACTUAL de ocupación: %d \n\t(MINIMA permitida: %d  MAXIMA permitida: %d)\n", densidadOcupacion, regC.densidadMin, regC.densidadMax);

	if (densidadOcupacion > regC.densidadMax)
	{
		printf("No se respeta la densidad maxima de ocupacion\n");
		return -3;
	}

	if (densidadOcupacion < regC.densidadMin)
	{
		printf("No se respeta la densidad minima de ocupacion.\n");
		return -4;
	}

	if ((densidadOcupacion > regC.densidadMin) && (densidadOcupacion < regC.densidadMax))
	{
		return 0;
	}

	return -1;
}

/*========== PRIMER EJERCICIO ==========*/

int creaHash(char *fichEntrada, char *fichHash, regConfig *regC)
{
	FILE *fEntrada;
	FILE *fHash;

	tipoAlumno alumno;
	tipoCubo cubo;

	fEntrada = fopen(fichEntrada, "rb+");
	if (fEntrada == NULL)
	{
		printf("Error al abrir el archivo de entrada.\n");
		return -1;
	}

	fHash = fopen(fichHash, "wb+");
	if (fHash == NULL)
	{
		printf("Error al abrir el archivo de salida.\n");
		fclose(fEntrada);
		return -2;
	}
	creaHvacio(fichHash, regC);
	// leeHash(fichHash);
	return 0;
	//  Obtener el tamaño del registro y el tamaño del archivo hash
	long tamRegistro = sizeof(tipoAlumno);
	fseek(fHash, 0, SEEK_END);
	long tamHash = ftell(fHash);

	// Calcular la capacidad máxima de registros en el archivo hash
	int capacidadMax = regC->nCubos * C;
	while (fread(&alumno, sizeof(tipoAlumno), 1, fEntrada) == 1)
	{
		int suma = 0;
		int posCubo;
		for (int i = 0; i < 9; i++)
		{
			suma += alumno.dni[i];
		}
		puts("Entra 1.1)");
		posCubo = suma % regC->nCubos;
		puts("Entra 1.2)");
		fseek(fHash, posCubo * sizeof(tipoCubo), SEEK_SET);
		puts("Entra 1.3)");
		fread(&cubo, sizeof(tipoCubo), 1, fHash);
		puts("Entra 1.4)");
		if (cubo.numRegAsignados < C)
		{
			puts("Entra 2)");
			cubo.reg[cubo.numRegAsignados] = alumno;
			cubo.numRegAsignados++;

			fseek(fHash, posCubo * sizeof(tipoCubo), SEEK_SET);
			fwrite(&cubo, sizeof(tipoCubo), 1, fHash);

			regC->numReg++;
		}
		else
		{
			puts("Entra 3)");
			if (cubo.desbordado == 0)
			{
				cubo.desbordado = 1;
				desborde(fHash, &cubo.reg[C - 1], regC);
			}
			desborde(fHash, &alumno, regC);
		}
	}

	fclose(fEntrada);
	fclose(fHash);

	// Verificar densidad de ocupación
	double densidad = (double)regC->numReg / capacidadMax;

	if (densidad > regC->densidadMax)
	{
		return -3;
	}
	else if (densidad < regC->densidadMin)
	{
		return -4;
	}
	else
	{
		fHash = fopen(fichHash, "r+b");
		if (fHash == NULL)
		{
			printf("Error al abrir el archivo de salida para actualizar los campos de configuración.\n");
			return -2;
		}

		fwrite(regC, sizeof(regConfig), 1, fHash);

		fclose(fHash);
		return 0;
	}
}

void creaHvacio(char *fichHash, regConfig *regC) // 2ª función a codificar
{

	int i;
	FILE *fHash;
	tipoAlumno nuevo_alumno;
	tipoCubo nuevo_cubo;

	if ((fHash = fopen(fichHash, "rb+")) == NULL)
	{
		printf("No se ha podido abrir el fichero.\n");
		exit(-2);
	}
	int nCubos = (regC->nCubos + regC->nCubosDes);
	regC->nCuboDesAct = 0;

	for (i = 0; i < nCubos; i++)
	{
		fwrite(&nuevo_cubo, sizeof(tipoCubo), 1, fHash);
	}

	fwrite(&regC, sizeof(regConfig), 1, fHash);
	fclose(fHash);
}

void desborde(FILE *fHash, tipoAlumno *reg, regConfig *regC) // 3ª función a codificar
{
	FILE *fDes;
	tipoAlumno nuevo_alumno;
	tipoCubo nuevo_cubo;
	int p_desborde;

	if (regC->nCuboDesAct > regC->nCubosDes)
	{
		printf("No hay mas cubos de desborde disponibles.\n");
		exit(-1);
	}

	p_desborde = regC->nCubos + regC->nCuboDesAct - 1;

	// Abrir el archivo de desbordamiento correspondiente
	fDes = fopen("desbordamiento.dat", "w+b");
	if (fDes == NULL)
	{
		printf("Error al abrir el archivo de desbordamiento.\n");
		exit(-2);
	}

	// Buscar un espacio vacío en el cubo de desbordamiento
	fseek(fDes, 0, SEEK_END);

	while (ftell(fDes) % sizeof(tipoAlumno) != 0)
	{
		fwrite(&nuevo_alumno, sizeof(tipoAlumno), 1, fDes);
	}

	// Escribir el registro desbordado en el cubo de desbordamiento
	fseek(fDes, p_desborde * sizeof(tipoAlumno), SEEK_SET);
	fwrite(reg, sizeof(tipoAlumno), 1, fDes);

	nuevo_cubo.desbordado = 1;
	fseek(fHash, p_desborde * sizeof(tipoCubo), SEEK_SET);
	fwrite(&nuevo_cubo, sizeof(tipoCubo), 1, fHash);

	fclose(fDes);

	// Actualizar el número de registros desbordados y el cubo de desbordamiento actual
	regC->numRegDes++;

	if (regC->numRegDes % regC->nCubosDes == 0)
	{
		regC->nCuboDesAct++;
	}
}

/*========== SEGUNDO EJERCICIO ==========*/ // hecho entero
// tipoAlumno *busquedaHash(FILE *f, char *dni, int *nCubo, int *nCuboDes, int *posReg, int *error);	//1ª función a codificar
// int modificarReg(char *fichero, char *dni, char *provincia);										//2ª función a codificar
