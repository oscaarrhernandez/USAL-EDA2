#include <stdio.h>
#include <stdlib.h>
#include "dispersion.h"

int main()
{
	// Configuración inicial
	regConfig config;
	config.nCubos = 15;
	config.nCubosDes = 4;
	// config.densidadMax = 75;
	// config.densidadMin = 40;

	// Crear el archivo hash vacío
	char entrada[] = "alumnos.dat";
	char hash[] = "alumnosC.hash";
	int result = creaHash(entrada, hash, &config);
	if (result != 0)
	{
		printf("Error al crear el archivo hash. Código de error: %d\n", result);
		return 1;
	}
	leeHash("alumnosC.hash");
	/*
	// Abrir el archivo hash para lectura/escritura
	FILE *fHash = fopen(hash, "r+b");
	if (fHash == NULL)
	{
		printf("Error al abrir el archivo hash.\n");
		return 1;
	}

	// Simular registros desbordados
	tipoAlumno alumno;
	for (int i = 0; i < 100; i++)
	{
		// Rellenar el registro con datos de prueba
		sprintf(alumno.dni, "DNI-%d", i);
		sprintf(alumno.nombre, "Nombre-%d", i);
		sprintf(alumno.ape1, "Apellido1-%d", i);
		sprintf(alumno.ape2, "Apellido2-%d", i);
		sprintf(alumno.provincia, "Provincia-%d", i);

		// Asignar el registro al cubo
		desborde(fHash, &alumno, &config);
	}

	// Cerrar el archivo hash
	fclose(fHash);
	printf("\nHola\n");*/
	return 0;
}
