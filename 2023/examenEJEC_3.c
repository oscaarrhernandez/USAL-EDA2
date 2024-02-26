/* EJERCICIO 3. PASAR GRAFO DIRIGIDO A NO DIRIGIDO*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20
#define INF 999999

typedef struct tagarco
{
	int v;
	int peso;
	struct tagarco *sig;
} arco;

typedef arco *pArco;

typedef struct
{
	int alcanzado;
	int gradoEntrada;
	int ordenTop;
	int distancia;
	int peso;
	int anterior;
	pArco lista;
} vertices;

typedef struct
{
	vertices directorio[N];
	int orden;
} tipoGrafo;

void convertirGrafoNoDirigido(tipoGrafo *grafoDirigido)
{
	for (int i = 0; i < grafoDirigido->orden; i++)
	{
		for (pArco arcoActual = grafoDirigido->directorio[i].lista; arcoActual != NULL; arcoActual = arcoActual->sig)
		{
			// Agregar un arco en dirección opuesta al grafo no dirigido
			pArco nuevoArco = (pArco)malloc(sizeof(arco));
			nuevoArco->v = i;
			nuevoArco->peso = arcoActual->peso;
			nuevoArco->sig = grafoDirigido->directorio[arcoActual->v].lista;
			grafoDirigido->directorio[arcoActual->v].lista = nuevoArco;

			// Actualizar el grado de entrada del vértice destino
			grafoDirigido->directorio[arcoActual->v].gradoEntrada++;
		}
	}
}
