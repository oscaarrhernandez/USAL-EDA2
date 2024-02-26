/* EJERCICIO 1. TADS Árbol Binario */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 20

typedef struct tipoNodo
{
	int info;
	struct tipoNodo *izq, *der;
} tipoNodo;
typedef tipoNodo *Arbol;

Arbol generaAB(int nivel, int nivelMax, int rango)
{

	Arbol a;
	int numhijos;
	if (nivel > nivelMax)
		return NULL;
	a = (tipoNodo *)calloc(nivelMax, sizeof(Arbol));
	a->info = rand() % rango;
	numhijos = rand() % 4;
	if ((numhijos == 1) || (numhijos == 2))
	{
		a->izq = generaAB(nivel + 1, nivelMax, rango);
	}
	if ((numhijos == 3) || (numhijos == 2))
	{
		a->der = generaAB(nivel + 1, nivelMax, rango);
	}

	return a;
}

void ejercicio1b(void)
{
	Arbol a1, a2;
	a1 = generaAB(0, 5, 100);
	a2 = generaAB(0, -1, 100);
	a2 = generaAB(5, 2, 100);
}
