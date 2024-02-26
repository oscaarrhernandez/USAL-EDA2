//! TADS ARBOL BINARIO Y GRAFO

#define N 20
typedef tipoNodo *Arbol;
typedef struct tagarco {
	int v;
	struct tagarco *sig;
}arco;
typedef arco *pArco;
typedef struct {
	pArco directorio[N];
	int orden;
} tipoGrafo;


/* 
	a) Definir en C el tipo de datos tipoNodo,utilizado en la linea 2, para que el arbol binario pueda almacenar la información sobre las aristas de un grafo dirigido ponderado. 
*/
typedef struct tagtipoNodo {
	int destino;    // Nodo destino de la arista
	int peso;       // Peso o costo de la arista
	struct tagtipoNodo *izquierdo;
	struct tagtipoNodo *derecho;
} tipoNodo;


/* 
	b) Implementar en C una función recursiva que permita construir un grafo dirigido ponderado a partir de la información que contiene el arbol binario cuyos nodos son del tipo definido en a). 
	*/
void construirGrafoDesdeArbol(tipoNodo *nodo, tipoGrafo *grafo, int padre) {
    if (nodo == NULL) {
        return;
    }
    
    // Agregar una arista desde el nodo padre al nodo actual en el grafo
    pArco nuevaArista = (pArco)malloc(sizeof(arco));
    nuevaArista->v = nodo->destino;
    nuevaArista->sig = grafo->directorio[padre];
    grafo->directorio[padre] = nuevaArista;
    
    // Llamar recursivamente a la función para los hijos izquierdo y derecho
    construirGrafoDesdeArbol(nodo->izquierdo, grafo, nodo->destino);
    construirGrafoDesdeArbol(nodo->derecho, grafo, nodo->destino);
}

/*
	c) Implementar en C una invocación a la función anterior junto con todas las sentencias necesarias para realizarla. 
*/
int main() {
    // Crear el árbol binario con la información de las aristas
    tipoNodo nodo1 = {2, 10, NULL, NULL};
    tipoNodo nodo2 = {3, 5, NULL, NULL};
    tipoNodo nodo3 = {4, 7, NULL, NULL};
    tipoNodo nodo4 = {5, 3, NULL, NULL};
    
    tipoNodo *arbol = &nodo1;
    arbol->izquierdo = &nodo2;
    arbol->derecho = &nodo3;
    arbol->izquierdo->izquierdo = &nodo4;
    
    // Construir el grafo a partir del árbol binario
    tipoGrafo grafo;
    grafo.orden = N;
    
    for (int i = 0; i < N; i++) {
        grafo.directorio[i] = NULL;
    }
    
    construirGrafoDesdeArbol(arbol, &grafo, 0);
    
    // Imprimir el grafo resultante
    for (int i = 0; i < N; i++) {
        pArco arista = grafo.directorio[i];
        printf("Nodo %d:", i);
        
        while (arista != NULL) {
            printf(" -> %d (%d)", arista->v, arista->peso);
            arista = arista->sig;
        }
        
        printf("\n");
    }
    
    return 0;
}

//! TADS MONTICULO BINARIO

#define MAXIMO 100
typedef int tipoClave;
typedef int tipoInfo;
typedef struct {
	tipoClave clave;
	tipoInfo informacion;
} tipoElemento;
typedef struct {
	tipoElemento elemento[MAXIMO];
	int tamanno;
} Monticulo;

/*
	FUNCIONES AUXILIARES QUE SE SUPONEN IMPLEMENTADAS Y QUE HAY QUE UTILIZAR
*/
void filtradoDescendente(Monticulo *m, int i);
void filtradoAscendente(Monticulo *m, int i);

/* 
	Implementar una función que modifique la clave del elemento, en una posición cualquiera del montículo, una cantidad que puede ser positiva o negativa, de forma que después de la modificación se siga cumpliendo la propiedad de orden. No olvides el prototipo de la función. 
*/
void modificarClave(Monticulo *m, int posicion, int cantidad) {
    // Verificar que la posición sea válida
    if (posicion < 0 || posicion >= m->tamanno) {
        printf("Posición inválida\n");
        return;
    }
  
    // Modificar la clave del elemento
    m->elemento[posicion].clave += cantidad;

    // Reestablecer la propiedad de orden descendente
    filtradoDescendente(m, posicion);
    // También puedes usar filtradoAscendente para reestablecer la propiedad de orden ascendente
}


//! TAD CONJUNTOS DISJUNTOS
/* 
	a) Escribe en lenguaje C las declaraciones necesarias para definir el tipo de datos conjunto si se representa mediante puntero a nodo padre. 
	*/
typedef struct NodoConjunto {
    int elemento;
    int rango;
    struct NodoConjunto *padre;
} Conjunto;


/* 
	b) Justifica brevemente porque se plantea la mejora de union por tamaño/altura. 
*/
	La mejora de "unión por tamaño/altura" se plantea para evitar árboles desbalanceados en la estructura de conjuntos disjuntos. Si se utiliza solo la unión por rango, puede ocurrir que en ciertos casos se generen árboles largos y delgados, lo que aumenta la complejidad temporal de las operaciones de búsqueda y unión. La mejora de "unión por tamaño/altura" busca equilibrar la altura de los árboles, lo que reduce la complejidad de las operaciones y mejora el rendimiento en general.
	La unión por tamaño se basa en mantener un contador de elementos en cada conjunto y, al unir dos conjuntos, se elige como raíz del árbol el conjunto de mayor tamaño. De esta manera, se garantiza que el árbol resultante sea más balanceado.

	La unión por altura se basa en mantener un valor de altura (o profundidad) para cada conjunto y, al unir dos conjuntos, se conecta el conjunto de menor altura al de mayor altura. De esta manera, se evita que los árboles crezcan excesivamente y se mantiene la estructura más equilibrada.
/* 
	c) Justifica brevemente la siguiente afirmación "La compresión de caminos es compatible con la unión por tamaño, pero no del todo con la unión por altura". 
*/
	La compresión de caminos es una técnica utilizada para optimizar la operación de búsqueda en la estructura de conjuntos disjuntos. Consiste en actualizar los punteros de los nodos en el camino de búsqueda para que apunten directamente al nodo raíz del conjunto, reduciendo así la longitud del camino de búsqueda en futuras operaciones.
	La compresión de caminos es compatible con la unión por tamaño porque no afecta la información del tamaño de los conjuntos. Al comprimir los caminos, se actualizan los punteros de los nodos, pero los tamaños de los conjuntos se mantienen intactos. Esto permite mantener la propiedad de tamaño y seguir realizando las uniones basadas en esta información.
	Sin embargo, la compresión de caminos no es totalmente compatible con la unión por altura. Al comprimir los caminos, se pueden alterar las alturas de los nodos y romper la información de altura precisa que se utiliza en la unión por altura. Esto puede afectar la decisión de unir conjuntos basada en la altura, lo que puede conducir a una estructura de conjuntos disjuntos menos balanceada y afectar el rendimiento general de las operaciones.

//! EJERCICIO OPCIONAL
/* 
	Proponer un algoritmo en seudocódigo que permita identificar si un grafo es conexo mediante clases de equivalencia.
*/
	Función esConexo(grafo):
    n <- número de nodos en el grafo
    Crear una matriz de clases de equivalencia de tamaño n x n
    Inicializar la matriz de clases de equivalencia con valores distintos para cada nodo

    Para cada arista (u, v) en el grafo:
        Si las clases de equivalencia de u y v son distintas:
            Unir las clases de equivalencia de u y v
            Actualizar la matriz de clases de equivalencia con el nuevo valor

    clase <- clase de equivalencia del primer nodo
    Para cada nodo en el grafo:
        Si la clase de equivalencia del nodo es distinta a la clase:
            Devolver falso

    Devolver verdadero
