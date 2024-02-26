//Jon García 70922400Z PA3
#include "dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lee el contenido del fichero hash organizado mediante el método de DISPERSIÓN según los criterios
// especificados en la práctica. Se leen todos los cubos completos tengan registros asignados o no. La
// salida que produce esta función permite visualizar el método de DISPERSIÓN

int leeHash(char *fichHash)
{ FILE *f;
  tipoCubo cubo;
  regConfig regC;
  int j,nCubo=0,densidadOcupacion;

   f = fopen(fichHash,"rb");
   fread(&regC,sizeof(regConfig),1,f);
   fread(&cubo,sizeof(cubo),1,f); 
   while (!feof(f)){
	for (j=0;j<C;j++) {
        	if (j==0) printf("Cubo %2d (%2d reg. ASIGNADOS)",nCubo,cubo.numRegAsignados);
        	else 	if ((j==1) && cubo.desbordado) printf("DESBORDADO\t\t");
        		else printf("\t\t\t");
		if (j < cubo.numRegAsignados) 
		    printf("\t%s %s %s %s %s\n",cubo.reg[j].dni, cubo.reg[j].nombre,cubo.reg[j].ape1,cubo.reg[j].ape2,
			    		cubo.reg[j].provincia);
	    	else printf ("\n");
        	}
		nCubo++;
       fread(&cubo,sizeof(cubo),1,f);
   }
   fclose(f);
   printf("ORGANIZACIÓN MÉTODO DISPERSIÓN:\n\t %d CUBOS y %d CUBOS DESBORDE con capacidad %d\n",
   		regC.nCubos,regC.nCubosDes, C);   
   printf("\t Contiene %d registros de los cuales se han desbordado %d:\n",regC.numReg,regC.numRegDes);
   densidadOcupacion = 100*regC.numReg/((regC.nCubos+regC.nCubosDes)*C);
   printf("Densidad ACTUAL de ocupación: %d \n\t(MÍNIMA permitida %d  MÁXIMA permitida %d)\n",
   				densidadOcupacion,regC.densidadMin,regC.densidadMax);
   if (densidadOcupacion>regC.densidadMax) {
   	printf("No se respeta la densidad máxima de ocupacion\n");
   	return -3;
   }	
   if (densidadOcupacion<regC.densidadMin) {
   	printf("No se respeta la densidad mínima de ocupacion\n");
   	return -4;
   }	
   if ((densidadOcupacion > regC.densidadMin) && (densidadOcupacion < regC.densidadMax))
      	return 0;
   
   return -1;	
}

void creaHvacio(char *fichHash,regConfig *reg){
  FILE *fHash;
  tipoCubo cubo;
  int j;
  int numCubos = reg->nCubos+reg->nCubosDes;
  memset(&cubo,0,sizeof(cubo)); //Inicializa las variables a su valor nulo, enteros a 0 y cadenas a NULL;
  fHash = fopen(fichHash,"wb");
  

  for (j = 0; j < numCubos; j++) {
        fwrite(&cubo, sizeof(cubo), 1, fHash);
  }
  fclose(fHash);

}

void desborde(FILE *fHash,tipoAlumno *reg, regConfig *regC){

  tipoCubo cubo;
  
  int numCubos = regC->nCubos + regC->nCubosDes;
  int i;

  // Buscar el primer cubo de desbordamiento con espacio
    for (i = 0; i < regC->nCubosDes; i++) {
        fseek(fHash, regC->nCubos * sizeof(tipoCubo) + i * sizeof(tipoCubo), SEEK_SET);
        fread(&cubo, sizeof(cubo), 1, fHash);
        if (cubo.numRegAsignados < C && cubo.desbordado == 1) {
            break;
        }
    }

    // Si no se encontró un cubo de desbordamiento con espacio, crear uno nuevo
    if (i == regC->nCubosDes) {
        fseek(fHash, 0, SEEK_END);
        regC->nCubosDes++;
        regC->nCuboDesAct = numCubos - 1; // Actualizar el número del primer cubo de desbordamiento con espacio
        cubo.desbordado = 1;
        cubo.numRegAsignados = 0;
        fwrite(&cubo, sizeof(cubo), 1, fHash);
    }

    // Posicionar el puntero en el cubo encontrado o creado
    fseek(fHash, regC->nCubos * sizeof(tipoCubo) + i * sizeof(tipoCubo), SEEK_SET);
    fread(&cubo, sizeof(cubo), 1, fHash);

    // Actualizar el cubo con el nuevo registro desbordado
    cubo.reg[cubo.numRegAsignados] = *reg;
    cubo.numRegAsignados++;

    // Guardar el cubo actualizado en el archivo hash
    fseek(fHash, -(sizeof(cubo)), SEEK_CUR);
    fwrite(&cubo, sizeof(cubo), 1, fHash);

    // Actualizar el número de registros desbordados en la estructura regConfig
    regC->numRegDes++;
}

int  creaHash(char *fichEntrada,char *fichHash, regConfig *regC){
    // Abrir el archivo de entrada
    FILE *fEntrada = fopen(fichEntrada, "r");
    if (fEntrada == NULL) {
        return -1; // Error en el archivo de entrada
    }

    // Crear el archivo hash vacío
    creaHvacio(fichHash, regC);

    // Abrir el archivo hash en modo escritura
    FILE *fHash = fopen(fichHash, "r+b");
    if (fHash == NULL) {
        fclose(fEntrada);
        return -2; // Error en el archivo hash
    }

    // Leer los registros del archivo de entrada y agregarlos al archivo hash
    tipoAlumno alumno;
    while (fread(&alumno, sizeof(tipoAlumno), 1, fEntrada) == 1) {
        int nCubo;
        int posReg;
        int error;
        tipoAlumno *alumnoEncontrado = busquedaHash(fHash, alumno.dni, &nCubo, NULL, &posReg, &error);

        if (error == 1) {
            fclose(fEntrada);
            fclose(fHash);
            return -5; // Error en el proceso de búsqueda
        }

        if (alumnoEncontrado == NULL) {
            // No se encontró el registro, se agrega al cubo correspondiente
            fseek(fHash, nCubo * sizeof(tipoCubo), SEEK_SET);
            tipoCubo cubo;
            fread(&cubo, sizeof(tipoCubo), 1, fHash);
            if (cubo.numRegAsignados < C) {
                cubo.reg[cubo.numRegAsignados] = alumno;
                cubo.numRegAsignados++;
                fseek(fHash, -sizeof(tipoCubo), SEEK_CUR);
                fwrite(&cubo, sizeof(tipoCubo), 1, fHash);
                regC->numReg++;
            } else {
                // El cubo está lleno, se produce desbordamiento
                desborde(fHash, &alumno, regC);
            }
        } else {
            // Se encontró el registro, no se hace nada
            free(alumnoEncontrado);
        }
    }

    // Cerrar archivos
    fclose(fEntrada);
    fclose(fHash);

    // Verificar la densidad de ocupación
    int densidad = (regC->numReg * 100) / (regC->nCubos * C);
    if (densidad > regC->densidadMax) {
        return -3; // Se supera la densidad máxima de ocupación
    }

    if (densidad < regC->densidadMin) {
        return -4; // Se reduce la densidad mínima de ocupación
    }

    return 0; // Proceso finalizado correctamente
}

tipoAlumno *busquedaHash(FILE *f, char *dni, int *nCubo, int *nCuboDes, int *posReg, int *error){

  tipoCubo cubo;
    int cuboActual = 0;
    int cuboDesbordeActual = C;

    // Búsqueda en los cubos principales
    while (fread(&cubo, sizeof(tipoCubo), 1, f) == 1) {
        for (int i = 0; i < cubo.numRegAsignados; i++) {
            if (strcmp(cubo.reg[i].dni, dni) == 0) {
              tipoAlumno *alumnoEncontrado = malloc(sizeof(tipoAlumno));
                if (alumnoEncontrado == NULL) {
                    *error = -5; // Error en la asignación de memoria
                    return NULL;
                }
                *nCubo = cuboActual;
                *nCuboDes = -1;
                *posReg = i;
                *error = 0;
                memcpy(alumnoEncontrado, &(cubo.reg[i]), sizeof(tipoAlumno));
                return alumnoEncontrado; // Se encontró el registro
            }
        }
        cuboActual++;
    }

    // Búsqueda en los cubos de desbordamiento
    fseek(f, cuboDesbordeActual * sizeof(tipoCubo), SEEK_SET);
    while (fread(&cubo, sizeof(tipoCubo), 1, f) == 1) {
        for (int i = 0; i < cubo.numRegAsignados; i++) {
            if (strcmp(cubo.reg[i].dni, dni) == 0) {
              tipoAlumno *alumnoEncontrado = malloc(sizeof(tipoAlumno));
                if (alumnoEncontrado == NULL) {
                    *error = -5; // Error en la asignación de memoria
                    return NULL;
                }
                *nCubo = cuboDesbordeActual;
                *nCuboDes = cuboDesbordeActual - C;
                *posReg = i;
                *error = 0;
                memcpy(alumnoEncontrado, &(cubo.reg[i]), sizeof(tipoAlumno));
                return alumnoEncontrado; // Se encontró el registro en el área de desborde
            }
        }
        cuboDesbordeActual++;
    }

    // No se encontró el registro
    *nCubo = -1;
    *nCuboDes = -1;
    *posReg = -1;
    *error = -1;
    return NULL;
}

int modificarReg(char *fichero, char *dni, char *provincia){
    FILE *f = fopen(fichero, "r+b");
    if (f == NULL) {
        return -2; // Error al abrir el fichero de datos
    }

    int nCubo, nCuboDes, posReg, error;
    tipoAlumno *alumno = busquedaHash(f, dni, &nCubo, &nCuboDes, &posReg, &error);
    if (error != 0) {
        fclose(f);
        free(alumno);
        return -1; // El registro no existe o hubo un error
    }

    // Modificar el campo provincia del registro
    strcpy(alumno->provincia, provincia);

    // Actualizar el registro modificado en el archivo
    fseek(f, sizeof(tipoCubo) * (nCubo + (nCuboDes + 1) * C), SEEK_SET);
    tipoCubo cubo;
    fread(&cubo, sizeof(tipoCubo), 1, f);
    memcpy(&(cubo.reg[posReg]), alumno, sizeof(tipoAlumno));
    fseek(f, -sizeof(tipoCubo), SEEK_CUR);
    fwrite(&cubo, sizeof(tipoCubo), 1, f);

    fclose(f);
    free(alumno);

    return nCubo;


}

