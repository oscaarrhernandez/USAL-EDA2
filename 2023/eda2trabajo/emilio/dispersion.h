#include <stdio.h>
#define C 5							// capacidad del cubo
#define CUBOS 20				// Número de cubos en el area prima
#define CUBOSDESBORDE 4 // Número de cubos area de desborde

typedef struct
{
	char dni[9];
	char nombre[19];
	char ape1[19];
	char ape2[19];
	char provincia[11];
} tipoAlumno;

typedef struct
{
	tipoAlumno reg[C];
	int cuboDES; // De momento no la vamos a utilizar
	int numRegAsignados;
} tipoCubo;

// funciones proporcionadas
void creaHvacio(char *fichHash);
int leeHash(char *fichHash);
// funciones a codificar
int creaHash(char *fichEntrada, char *fichHash);
int desborde(FILE *fHash, tipoAlumno reg, int cubos, int cubosDesborde); // Auxiliar para creaHash
int buscaReg(FILE *fHash, tipoAlumno *reg, char *dni);
int insertarReg(FILE *f, tipoAlumno *reg);
