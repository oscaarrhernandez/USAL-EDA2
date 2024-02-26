// prueba.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dispersion.h"

int main(void)
{
	system("PAUSE");
	system("cls");

	printf("Ejercicio 1\n");
	printf("-----------\n");

	printf("- Se debe obtener el fichero alumnosC.hash\n");

	char fichHash[] = "alumnosC.hash";
	regConfig configuracion;
	configuracion.nCubos = 15;
	configuracion.nCubosDes = 4;
	int res;
	creaHash("alumnos.dat", fichHash, &configuracion);
	/*
		res = creaHash(fichEntrada, fichHash, &configuracion);
		if (res == 0)
		{
			printf("Proceso completado correctamente.\n");
		}
		else if (res == -1)
		{
			printf("Error al abrir el archivo de entrada.\n");
		}
		else if (res == -2)
		{
			printf("Error al abrir/crear el archivo de salida.\n");
		}
		else if (res == -3)
		{
			printf("Se supera la densidad máxima de ocupación.\n");
		}
		else if (res == -4)
		{
			printf("Se reduce la densidad mínima de ocupación.\n");
		}
		else
		{
			printf("Ocurrió un error en el proceso.\n");
		}

		system("PAUSE");

	*/
	leeHash(fichHash);
	return 0;
}
