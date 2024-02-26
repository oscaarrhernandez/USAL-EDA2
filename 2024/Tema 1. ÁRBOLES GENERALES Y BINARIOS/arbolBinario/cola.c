#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cola.h"


int colaCreaVacia(Cola *c) {
	c->frente = NULL;
	c->fondo = NULL;
	return 0;
}

int colaVacia(Cola *c) {
	if(c==NULL)
		return -1;
	if((c->frente==NULL)&&(c->fondo==NULL))
		return 1;
	else
		return 0;
}

int colaInserta(tipoElemento elemento, Cola *c) {
	tipoCelda * temp;
	if(c == NULL)
		return -1;
	if(colaVacia(c) == 1) {
		c->fondo = (tipoCelda *)malloc(sizeof(tipoCelda));
		c->frente = c->fondo;
		c->fondo->elemento = elemento;
		c->fondo->sig = NULL;
		return 0;
	} else {
		temp = c->frente;
		c->frente = (tipoCelda *)malloc(sizeof(tipoCelda));
		c->frente->elemento = elemento;
		c->frente->sig = temp;
		return 0;
	}
}

tipoElemento colaSuprime(Cola *c) {
	tipoElemento valor;
	tipoCelda *temp;
	if(c == NULL)
		//return -2;
		return NULL;
	if(colaVacia(c) == 1)
		//return -1;
		return NULL;
	else {
		if(c->fondo == c->frente) {
			valor = c->fondo->elemento;
			colaCreaVacia(c);
			return valor;
		}
		valor = c->fondo->elemento;
		temp = c->frente;
		while(temp->sig != c->fondo) {
			temp = temp->sig;
		}
		temp->sig = NULL;
		//free(c->fondo);
		c->fondo = temp;
		c->fondo->sig = NULL;
		return valor;
	}
}
