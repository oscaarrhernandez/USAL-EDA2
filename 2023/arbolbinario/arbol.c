#include <stdlib.h>
#include <stdio.h>
#include "arbol.h"
#include "cola.h"

Arbol creaNodo(tipoInfo info)
{
	tipoNodo *nuevo;

	//   nuevo =(tipoNodo *)calloc(1, sizeof(tipoNodo));
	if ((nuevo = (tipoNodo *)malloc(sizeof(tipoNodo))) == NULL)
		return NULL;
	else
	{
		nuevo->info = info;
		nuevo->izq = NULL;
		nuevo->der = NULL;
		return nuevo;
	}
}
void amplitud(Arbol raiz)
{
	Cola c;
	tipoNodo *nodo;
	colaCreaVacia(&c);
	nodo = raiz;
	if (raiz != NULL)
		colaInserta(&c, nodo);
	while (!colaVacia(&c))
	{
		nodo = (Arbol)colaSuprime(&c);
		printf(" %c ", nodo->info);
		if (nodo->izq != NULL)
			colaInserta(&c, nodo->izq);
		if (nodo->der != NULL)
			colaInserta(&c, nodo->der);
	}
}
int altura(Arbol raiz)
{

	int izq = -1, der = -1;
	if (raiz == NULL)
		return -1;
	else if ((raiz->izq == NULL && raiz->der == NULL))
		return 0;
	else
	{
		izq = altura(raiz->izq);
		if (izq != -1)
			izq++;
		der = altura(raiz->der);
		if (der != -1)
			der++;
		if (der > izq)
			return der;
		else
			return izq;
	}
}
int numNodos(Arbol raiz)
{
	if (raiz == NULL)
		return 0;
	int izq = numNodos(raiz->izq);
	int der = numNodos(raiz->der);
	return izq + der + 1;
}
Arbol anula(Arbol raiz)
{
	if (raiz == NULL)
		return NULL;
	else
	{
		anula(raiz->izq);
		anula(raiz->der);
		free(raiz);
		return NULL;
	}
}
int sustiuye(Arbol raiz, tipoInfo x, tipoInfo y)
{
	if (raiz == NULL)
		return 0;
	else
	{
		int izq = sustiuye(raiz->izq, x, y);
		int der = sustiuye(raiz->der, x, y);
		if (raiz->info == x)
		{
			raiz->info = y;
			return 1 + izq + der;
		}
		return der + izq;
	}
}
int numNodosHoja(Arbol raiz)
{
	if (raiz == NULL)
		return -1;
	else
	{
		int izq = numNodosHoja(raiz->izq);
		int der = numNodosHoja(raiz->der);
		if (izq == -1 && der == -1)
			return 1;
		return izq + der;
	}
}
int numNodosInternos(Arbol raiz)
{
	return numNodos(raiz) - numNodosHoja(raiz);
	if (raiz == NULL)
	{
		return -1;
	}
	else
	{
		int izq = numNodosInternos(raiz->izq);
		int der = numNodosInternos(raiz->der);

		if (izq == -1 && der == -1)
			return 0;
		else if (izq == -1 || der == -1)
		{
			return izq + der + 2;
		}
		else
		{
			return izq + der + 1;
		}
	}
}
int numHijoUnico(Arbol raiz)
{
	if (raiz == NULL)
		return 0;
	else
	{
		int izq = numHijoUnico(raiz->izq);
		int der = numHijoUnico(raiz->der);
		if (izq == 0 && der == 0)
			return 0;
		else if (izq == 0 || der == 0)
			return izq + der + 2;
		else
			return izq + der + 2;
	}
}
Arbol buscarMax(Arbol raiz)
{
	if (raiz == NULL)
		return NULL;
	else
	{
		Arbol izq = buscarMax(raiz->izq);
		Arbol der = buscarMax(raiz->der);

		if (izq == NULL && der == NULL)
		{
			return raiz;
		}
		if (izq == NULL)
		{
			if (der->info > raiz->info)
			{
				return der;
			}
			else
			{
				return raiz;
			}
		}
		else if (der == NULL)
		{
			if (izq->info > raiz->info)
			{
				return izq;
			}
			else
			{
				return raiz;
			}
		}

		if (izq->info > der->info)
		{
			if (izq->info > raiz->info)
			{
				return izq;
			}
			else
			{
				return raiz;
			}
		}
		else if (der->info > raiz->info)
		{ // ¿¿ if(der->info > IZQ->info){  ??
			return der;
		}
		else
		{
			return raiz;
		}
	}
}
Arbol buscarMin(Arbol raiz)
{
	if (raiz == NULL)
		return NULL;
	else
	{
		Arbol izq = buscarMax(raiz->izq);
		Arbol der = buscarMax(raiz->der);

		if (izq == NULL && der == NULL)
		{
			return raiz;
		}
		if (izq == NULL)
		{
			if (der->info < raiz->info)
			{
				return der;
			}
			else
			{
				return raiz;
			}
		}
		else if (der == NULL)
		{
			if (izq->info < raiz->info)
			{
				return izq;
			}
			else
			{
				return raiz;
			}
		}

		if (izq->info < der->info)
		{
			if (izq->info < raiz->info)
			{
				return izq;
			}
			else
			{
				return raiz;
			}
		}
		else if (der->info < raiz->info)
		{
			return der;
		}
		else
		{
			return raiz;
		}
	}
}
int similares(Arbol r1, Arbol r2)
{
	if (r1 == NULL)
	{
		if (r2 == NULL)
			return 1;
		else
			return 0;
	}
	else
	{
		if (r2 == NULL)
			return 0;
		else
		{
			if (similares(r1->izq, r2->izq) == 0)
				return 0;
			if (similares(r1->der, r2->der) == 0)
				return 0;

			return 1;
		}
	}
}
int equivalentes(Arbol r1, Arbol r2)
{
	if (r1 == NULL)
	{
		if (r2 == NULL)
			return 1;
		else
			return 0;
	}
	else
	{
		if (r2 == NULL)
			return 0;
		else
		{
			if (r1->info != r2->info)
				return 0;
			else
			{
				if (equivalentes(r1->izq, r2->izq) == 0)
					return 0;
				if (equivalentes(r1->der, r2->der) == 0)
					return 0;

				return 1;
			}
		}
	}
}
Arbol especular(Arbol raiz)
{
	if (raiz == NULL)
		return NULL;
	else
	{
		Arbol nuevo = creaNodo(raiz->info);
		nuevo->izq = especular(raiz->der);
		nuevo->der = especular(raiz->izq);

		return nuevo;
	}
}
