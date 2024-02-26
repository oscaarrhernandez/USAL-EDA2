#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cola.h"

int colaCreaVacia(Cola *c)
{
	if (c != NULL)
	{
		c->fondo = NULL;
		c->frente = NULL;
		return 0;
	}
	else
	{
		printf("La cola ya está creada");
		return 2;
	}
}

int colaVacia(Cola *c)
{
	if (c->frente == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int colaInserta(Cola *c, tipoElemento elemento)
{
	tipoCelda *aux;
	aux = (tipoCelda *)malloc(sizeof(tipoCelda));
	if (c == NULL || aux == NULL)
		return -1;
	if (c->frente == NULL)
	{
		aux->elemento = elemento;
		aux->sig = NULL;
		c->frente = aux;
		c->fondo = aux;
	}
	else
	{
		aux->elemento = elemento;
		aux->sig = NULL;
		c->fondo->sig = aux;
		c->fondo = aux;
	}
}

tipoElemento colaSuprime(Cola *c)
{
	tipoCelda *aux;
	tipoElemento elemento;
	aux = c->frente;
	if (c == NULL || c->frente == NULL)
		return NULL;
	elemento = c->frente->elemento;
	c->frente = c->frente->sig;
	free(aux);
	return elemento;
}
