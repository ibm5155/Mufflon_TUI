#include <stdlib.h>
#include <string.h>
#include "Text.h"


char *ReadEOFCVAR(Texto *Regra, char *Name) {
	int Offset = 0;
	int newsize = 0;
	char *tmp2 = 0;
	char *tmp = 0;
	int i = 0;
	for (; i < Regra->QntLinhas; i++) {
		tmp = Carrega_Proxima_Palavra(&Regra->Linhas[i], &Offset);
		if (tmp) {
			if (strcmp(tmp, Name) == 0) {
				free(tmp);
				tmp = Carrega_Proxima_Palavra(&Regra->Linhas[i], &Offset);
				if (tmp) {
					if (strcmp(tmp, "=") == 0) {
						free(tmp);
						tmp = (char*)malloc( (Regra->Linhas[i].QntCaracteres - Offset)* sizeof(char));//mem alloc the amount of chars left from the line
						sprintf(tmp, "%s", (char*)&Regra->Linhas[i].Caracteres[Offset+1]);// copy everything after the offset under the start of the tmp
						i = Regra->QntLinhas+1;
					}
				}
			}
			else {
				free(tmp);
				Offset = 0;
			}
		}
	}
	if (i == Regra->QntLinhas)
	{
		tmp = 0;
	}
	return tmp;
}


char *ReadCVAR(Texto *Regra, char *Name) {
	int Offset = 0;
	char *tmp = 0;
	int i = 0;
	for (; i < Regra->QntLinhas; i++) {
		tmp = Carrega_Proxima_Palavra(&Regra->Linhas[i], &Offset);
		if (tmp) {
			if (strcmp(tmp, Name) == 0) {
				free(tmp);
				tmp = Carrega_Proxima_Palavra(&Regra->Linhas[i], &Offset);
				if (tmp) {
					if (strcmp(tmp, "=") == 0) {
						free(tmp);
						tmp = Carrega_Proxima_Palavra(&Regra->Linhas[i], &Offset);
						i = Regra->QntLinhas+1;
					}
				}
			}
			else {
				free(tmp);
				Offset = 0;
			}
		}
	}
	if (i == Regra->QntLinhas)
	{
		tmp = 0;
	}
	return tmp;
}

/**
Retorno padrão se CVAR não encontrado = false
**/
bool ReadBooleanCVAR(Texto *Regra, char *Name) {
	bool Saida = false;
	char *tmp = ReadCVAR(Regra, Name);
	if (tmp) {
		if (strcmp(tmp, "true") == 0) {
			Saida = true;
		}
		free(tmp);
	}
	return Saida;
}
/**
Retorno padrão se CVAR não encontrado = 0
**/
int ReadIntegerCVAR(Texto *Regra, char *Name) {
	int Saida = 0;
	char *tmp = ReadCVAR(Regra, Name);
	if (tmp) {
		Saida = atoi(tmp);
		free(tmp);
	}
	return Saida;
}