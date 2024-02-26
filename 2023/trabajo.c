int leeHash(char *fichHash)
{
	FILE *f;
	tipoCubo cubo;
	regConfig regC;
	int j, nCubo = 0, densidadOcupacion;

	f = fopen(fichHash, "rb");
	fread(&regC, sizeof(regConfig), 1, f);
	fread(&cubo, sizeof(cubo), 1, f);
	while (!feof(f))
	{
		for (j = 0; j < C; j++)
		{
			if (j == 0)
				printf("Cubo %2d (%2d reg. ASIGNADOS)", nCubo, cubo.numRegAsignados);
			else if ((j == 1) && cubo.desbordado)
				printf("DESBORDADO\t\t");
			else
				printf("\t\t\t");
			if (j < cubo.numRegAsignados)
				printf("\t%s %s %s %s %s\n", cubo.reg[j].dni, cubo.reg[j].nombre, cubo.reg[j].ape1, cubo.reg[j].ape2,
							 cubo.reg[j].provincia);
			else
				printf("\n");
		}
		nCubo++;
		fread(&cubo, sizeof(cubo), 1, f);
	}
	fclose(f);
	printf("ORGANIZACIÓN MÉTODO DISPERSIÓN:\n\t %d CUBOS y %d CUBOS DESBORDE con capacidad %d\n",
				 regC.nCubos, regC.nCubosDes, C);
	printf("\t Contiene %d registros de los cuales se han desbordado %d:\n", regC.numReg, regC.numRegDes);
	densidadOcupacion = 100 * regC.numReg / ((regC.nCubos + regC.nCubosDes) * C);
	printf("Densidad ACTUAL de ocupación: %d \n\t(MÍNIMA permitida %d  MÁXIMA permitida %d)\n",
				 densidadOcupacion, regC.densidadMin, regC.densidadMax);
	if (densidadOcupacion > regC.densidadMax)
	{
		printf("No se respeta la densidad máxima de ocupacion\n");
		return -3;
	}
	if (densidadOcupacion < regC.densidadMin)
	{
		printf("No se respeta la densidad mínima de ocupacion\n");
		return -4;
	}
	if ((densidadOcupacion > regC.densidadMin) && (densidadOcupacion < regC.densidadMax))
		return 0;

	return -1;
}

void creaHvacio(char *fichHash, regConfig *reg)
{
	FILE *fHash;
	tipoCubo cubo;
	int j;
	int numCubos = reg->nCubos + reg->nCubosDes;
	reg->nCuboDesAct = reg->nCubos;
	reg->numReg = reg->numRegDes = 0;
	memset(&cubo, 0, sizeof(cubo));
	fHash = fopen(fichHash, "wb");

	fwrite(reg, sizeof(regConfig), 1, fHash);
	for (j = 0; j < numCubos; j++)
	{
		fwrite(&cubo, sizeof(tipoCubo), 1, fHash);
	}
	fclose(fHash);
}

void desborde(FILE *fHash, tipoAlumno *reg, regConfig *regC)
{

	tipoCubo cubo;

	int i;

	fseek(fHash, sizeof(regConfig) + sizeof(tipoCubo) * regC->nCuboDesAct, SEEK_SET);
	fread(&cubo, sizeof(tipoCubo), 1, fHash);

	for (i = (regC->nCuboDesAct - regC->nCubos); i < regC->nCubosDes; i++)
	{
		if (cubo.numRegAsignados < C)
		{

			cubo.reg[cubo.numRegAsignados] = *reg;
			cubo.numRegAsignados++;

			fseek(fHash, -sizeof(tipoCubo), SEEK_CUR);
			fwrite(&cubo, sizeof(tipoCubo), 1, fHash);
			return;
		}
		else
		{

			regC->nCuboDesAct++;
			fseek(fHash, sizeof(regConfig) + sizeof(tipoCubo) * regC->nCuboDesAct, SEEK_SET);
			fread(&cubo, sizeof(tipoCubo), 1, fHash);
		}
	}

	tipoCubo newCubo;
	newCubo.desbordado = 0;
	newCubo.numRegAsignados = 1;
	newCubo.reg[0] = *reg;
	regC->nCubosDes++;
	fseek(fHash, 0, SEEK_END);
	fwrite(&newCubo, sizeof(tipoCubo), 1, fHash);
	return;
}

int creaHash(char *fichEntrada, char *fichHash, regConfig *regC)
{
	tipoAlumno alumno;
	tipoCubo cubo;
	FILE *fichE, *fichH;
	int nCubo;

	creaHvacio(fichHash, regC);

	fichE = fopen(fichEntrada, "rb");
	if (fichE == NULL)
	{
		printf("Error al abrir el fichero alumno.dat\n");
		return -1;
	}
	fichH = fopen(fichHash, "r+b");
	if (fichH == NULL)
	{
		printf("Error al abrir el fichero alumnoC.hash\n");
		return -2;
	}

	while (fread(&alumno, sizeof(tipoAlumno), 1, fichE))
	{
		nCubo = (atoi(alumno.dni)) % (regC->nCubos);

		fseek(fichH, sizeof(regConfig) + nCubo * sizeof(tipoCubo), SEEK_SET);
		fread(&cubo, sizeof(tipoCubo), 1, fichH);

		if (cubo.numRegAsignados < C)
		{
			cubo.reg[cubo.numRegAsignados] = alumno;
			cubo.numRegAsignados++;
			regC->numReg++;

			fseek(fichH, sizeof(regConfig) + nCubo * sizeof(tipoCubo), SEEK_SET);
			fwrite(&cubo, sizeof(tipoCubo), 1, fichH);
		}
		else
		{
			cubo.desbordado = 1;
			regC->numReg++;
			regC->numRegDes++;
			fseek(fichH, sizeof(regConfig) + nCubo * sizeof(tipoCubo), SEEK_SET);
			fwrite(&cubo, sizeof(tipoCubo), 1, fichH);
			desborde(fichH, &alumno, regC);
		}
	}

	fseek(fichH, 0, SEEK_SET);
	fwrite(regC, sizeof(regConfig), 1, fichH);
	fclose(fichE);
	fclose(fichH);
	int densidadOcupacion = 100 * regC->numReg / ((regC->nCubos + regC->nCubosDes) * C);
	if (densidadOcupacion > regC->densidadMax)
		return -3;
	if (densidadOcupacion < regC->densidadMin)
		return -4;
	return 0;
}

tipoAlumno *busquedaHash(FILE *f, char *dni, int *nCubo, int *nCuboDes, int *posReg, int *error)
{
	tipoCubo cubo;
	int i, j;
	regConfig regC;
	fseek(f, 0, SEEK_SET);
	fread(&regC, sizeof(regConfig), 1, f);

	*nCubo = atoi(dni) % regC.nCubos;
	*posReg = 0;

	tipoAlumno *alumno = malloc(sizeof(tipoAlumno));

	fseek(f, sizeof(regConfig) + sizeof(tipoCubo) * (*nCubo), SEEK_SET);

	fread(&cubo, sizeof(tipoCubo), 1, f);
	for (i = 0; i < cubo.numRegAsignados; i++)
	{
		if (strcmp(cubo.reg[i].dni, dni) == 0)
		{
			*error = 0;
			*nCuboDes = -1;
			*posReg = i;
			*alumno = cubo.reg[i];
			return alumno;
		}
	}

	for (i = 0; i < regC.nCubosDes; i++)
	{
		fseek(f, sizeof(regConfig) + sizeof(tipoCubo) * (regC.nCubos + i), SEEK_SET);
		fread(&cubo, sizeof(tipoCubo), 1, f);

		for (j = 0; j < cubo.numRegAsignados; j++)
		{
			if (strcmp(cubo.reg[j].dni, dni) == 0)
			{
				*error = 0;
				*nCuboDes = i;
				*posReg = j;
				*alumno = cubo.reg[j];
				return alumno;
			}
		}
	}
	*error = -1;
	return NULL;
}

int modificarReg(char *fichero, char *dni, char *provincia)
{
	FILE *f = fopen(fichero, "r+b");
	if (f == NULL)
	{
		return -2;
	}
	regConfig regC;
	fseek(f, 0, SEEK_SET);
	fread(&regC, sizeof(regConfig), 1, f);

	int nCubo, nCuboDes, posReg, error;
	tipoAlumno *alumno = busquedaHash(f, dni, &nCubo, &nCuboDes, &posReg, &error);
	if (error != 0)
	{
		fclose(f);
		free(alumno);
		return -1;
	}

	strcpy(alumno->provincia, provincia);

	if (nCuboDes == -1)
	{
		fseek(f, sizeof(regConfig) + sizeof(tipoCubo) * (nCubo), SEEK_SET);
	}
	else
	{
		fseek(f, sizeof(regConfig) + sizeof(tipoCubo) * (regC.nCubos + nCuboDes), SEEK_SET);
	}

	tipoCubo cubo;
	fread(&cubo, sizeof(tipoCubo), 1, f);
	memcpy(&(cubo.reg[posReg]), alumno, sizeof(tipoAlumno));
	fseek(f, -sizeof(tipoCubo), SEEK_CUR);
	fwrite(&cubo, sizeof(tipoCubo), 1, f);

	fclose(f);
	free(alumno);

	return nCubo;
}
