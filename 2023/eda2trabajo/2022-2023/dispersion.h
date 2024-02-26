#include <stdio.h>
#ifndef __DISPERSION_H
#define __DISPERSION_H

#define C 5			// Capacidad del cubo
typedef struct {
	int nCubos;		// N�mero de cubos en el area prima
	int nCubosDes;		// N�mero de cubos area de desborde
	int nCuboDesAct; 	// N�mero del primer cubo desborde con espacio para m�s registros
	int densidadMax;	// M�xima densidad de ocupaci�n permitida
	int densidadMin;	// M�nima densidad de ocupaci�n permitida
	int numReg;		// N�mero total de registros en el archivo
	int numRegDes;		// N�mero de registros desbordados
} regConfig;

typedef struct {
	char dni[9];
	char nombre[19];
	char ape1[19];
	char ape2[19];
	char provincia[11];
	} tipoAlumno;

typedef struct {
	tipoAlumno reg[C];
	int numRegAsignados;
	int desbordado;	// Este campo indica si el cubo se ha desbordado(1) o no(0)
	}tipoCubo;
	
// Funci�n proporcionada
int leeHash(char *fichHash);
// Funciones a codificar
// Ejercicio 1
int  creaHash(char *fichEntrada,char *fichHash, regConfig *regC);
void creaHvacio(char *fichHash,regConfig *reg);
void desborde(FILE *fHash,tipoAlumno *reg, regConfig *regC);
// Ejercicio 2
tipoAlumno *busquedaHash(FILE *f, char *dni, int *nCubo, int *nCuboDes, int *posReg, int *error);
int modificarReg(char *fichero, char *dni, char *provincia);
#endif

