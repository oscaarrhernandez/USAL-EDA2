#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "pila.h"



int pilaCreaVacia(Pila *p) {
	*p = NULL;
	return 1;
}

int pilaVacia(Pila *p) {
	if(*p == NULL)
		return -1;
	else
		return 0;
}

int pilaInserta(tipoElemento elemento, Pila *p) {
	Pila nuevo;
	if (NULL==(nuevo = malloc(sizeof(tipoCelda)))){
		return 0;
	} else {
		nuevo->elemento = elemento;
		nuevo->sig = NULL;
	}
	nuevo->sig = *p;
	*p = nuevo;

	return 1;
}

tipoElemento pilaSuprime(Pila *p) {
	tipoCelda *aBorrar;
	tipoElemento valor;

	if(p == NULL || *p == NULL){
		return 0;
	} else {
		valor = (*p)->elemento;
		aBorrar = *p;
		*p = (*p)->sig;
		free(aBorrar);
		return valor;
	}
}
