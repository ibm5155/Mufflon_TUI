#pragma once
#ifndef _TEXTO_HEADER
#define _TEXTO_HEADER

#include <stdio.h>
#define bool unsigned char
#define true 1
#define false 0

typedef struct {
	char *Caracteres;
	int QntCaracteres;// 0 = vazio, 1 = 1 caracter,...
}Linha;

typedef struct {
	Linha *Linhas;
	int QntLinhas;// 0 = vazio, 1 = 1 linha,...
	char *Fonte;// local onde será armazenado o texto
	unsigned char Salvo;//SIM/NAO
}Texto;

Texto *CarregaTexto(char *endereco);
void SalvaTexto(Texto *texto);
void AdicionaLinha(Texto * texto, char *adicionar);

void Destroi_Texto(Texto *t);

char  *CarregaPalavra(FILE *arquivo);
char *Carrega_Proxima_Palavra(Linha *linha, int *posicao);

char *InteiroParaCharBinario(int Decimal, bool Tamanho);
int CharBinarioParaInteiro(char *Binario);

char *GeraLixo(int Quantidade);
#endif