#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Text.h"

//#include <MT2D/MessageBox/MT2D_MessageBox.h>

void AdicionaLinha(Texto * texto, char * adicionar)
{
	if (texto) {
		int Tamannho = strlen(adicionar);
		if (texto->Linhas) {
			texto->Linhas = (Linha*)realloc(texto->Linhas, (texto->QntLinhas + 1) * sizeof(Linha));
			texto->Linhas[texto->QntLinhas].QntCaracteres = Tamannho;
			texto->Linhas[texto->QntLinhas].Caracteres = (char*)malloc((Tamannho + 1) * sizeof(char));
			strcpy(texto->Linhas[texto->QntLinhas].Caracteres, adicionar);
			texto->QntLinhas++;

		}
		else {
			texto->QntLinhas = 1;
			texto->Linhas = (Linha*)malloc(sizeof(Linha));
			texto->Linhas[0].QntCaracteres = Tamannho;
			texto->Linhas[0].Caracteres = (char*)malloc((Tamannho + 1) * sizeof(char));
			strcpy(texto->Linhas[0].Caracteres, adicionar);
		}
	}
}


Texto * CarregaTexto(char * endereco)
{
	Texto *t = 0;
	FILE *f = 0;
	char *caracteres = 0;
	int qnt_caracteres = 0;
	char buffer = 0;
	if (endereco) {
		f = fopen(endereco, "r");
	}
	if (f) {
		t = (Texto*)malloc(sizeof(Texto));
		t->Fonte = (char*)malloc((strlen(endereco) + 1) * sizeof(char));
		strcpy(t->Fonte, endereco);
		t->Linhas = 0;
		t->QntLinhas = 0;
		t->Salvo = true;
		caracteres = (char*)malloc(sizeof(char));
		while (!feof(f)) {
			buffer = fgetc(f);
			if (buffer == 9) {//transforma o simbolo do tab "o" em espaço
				buffer = ' ';
			}
			if (buffer == '\n') {
				caracteres = (char*)realloc(caracteres, (qnt_caracteres + 2) * sizeof(char));
				caracteres[qnt_caracteres] = '\0';
				qnt_caracteres++;
				AdicionaLinha(t, caracteres);
				free(caracteres);
				qnt_caracteres = 0;
				caracteres = (char*)malloc(sizeof(char));
			}
			else {
				caracteres = (char*)realloc(caracteres, (qnt_caracteres + 2) * sizeof(char));
				caracteres[qnt_caracteres] = buffer;
				qnt_caracteres++;
			}
		}
		if (qnt_caracteres) {//faltou um pedaço a ser carregado...
			caracteres = (char*)realloc(caracteres, (qnt_caracteres + 2) * sizeof(char));
			caracteres[qnt_caracteres - 1] = '\0';
			qnt_caracteres++;
			AdicionaLinha(t, caracteres);
			free(caracteres);
			qnt_caracteres = 0;
		}
		fclose(f);
	}
	else if (endereco) {
		//MT2D_MessageBox("Arquivo nao encontrado ou endereco invalido");
	}
	return t;
}


void Destroi_Texto(Texto *t) {
	int i = 0;
	if (t) {//só apaga se tiver conteudo
			//Etapa1: limpa o endereço de onde contem o texto
		if (t->Fonte) {
			free(t->Fonte);
		}
		//Etapa2: limpa todos os caracters do texto
		for (i = t->QntLinhas - 1; i >= 0; i--) {
			free(t->Linhas[i].Caracteres);
		}
		free(t->Linhas);//pode dar ruim

						//Etapa3: apaga a estrutura do texto
		free(t);
	}
}


//int LinhaCarregaPalavra = 1;
/**
retorno:
0 : arquivo não encontrado
? : ponteiro da frase
*/
/*
char  *CarregaPalavra(FILE *arquivo)
{
//carrega palavra vai ser chamado pelas funções: carregainstrucao e carregaregistrador.
char  *Palavra = 0;
int TamanhoPalavra = 0;
char buffer = 0;

if (arquivo) {
//primeiro loop é sempre verdadeiro, a menos que já esteja no final do arquivo
while (!feof(arquivo) && buffer != '\n' && buffer != ' '){
buffer = fgetc(arquivo);
if (buffer != '\n' && buffer != ' ' && !feof(arquivo)) {
if (buffer == INICIO_COMENTARIO) {//então pule a linha
while (!feof(arquivo) && buffer != '\n') {//precisa percorrer a linha inteira senão será retornado alguma palavra dentro do comentário
buffer = fgetc(arquivo);
}if (TamanhoPalavra == 0) {//não capturou nenhum dado então pule para a próxima linha (provavelmente a linha lida não contém nenhuma palavra válida)
buffer = 0; //engana o loop inicial para não cair na condição de parada
LinhaCarregaPalavra++;
}

}
else {
if (TamanhoPalavra == 0) {
Palavra = (char*)malloc((TamanhoPalavra + 2) * sizeof(char));
Palavra[TamanhoPalavra] = buffer;
Palavra[TamanhoPalavra+1] = '\0';
TamanhoPalavra++;
}
else {
Palavra = (char*)realloc(Palavra, (TamanhoPalavra + 2) * sizeof(char));
Palavra[TamanhoPalavra] = buffer;
Palavra[TamanhoPalavra + 1] = '\0';
TamanhoPalavra++;
}
}
}else if (buffer == ' ' && TamanhoPalavra == 0) {//caso caia aqui ele vai pular até o inicio do próximo dado (pulando todos os espaços)
while (!feof(arquivo) && (buffer == '\n' || buffer == ' ') ) {
buffer = fgetc(arquivo);
}
if (buffer == '\n') {
LinhaCarregaPalavra++;
}
fseek(arquivo, -1, SEEK_CUR);//pega o ponteiro do cursor e move um offset para traz, pois  só
//sairá do loop acima caso ele ache alguma palavra/comentário (senão este caracter lido será pulado...) (caso seja um comentário ele será tratado acima)
buffer = 0; //engana o loop inicial para não cair na condição de parada
}
}
if (buffer == '\n') {
LinhaCarregaPalavra++;
}
}
return Palavra;
}
*/
/**
Carrega uma palavra de uma linha a partir da posição "posicao".
A palavra carregada não conterá espaços, somente a palavra isolada.
RETORNO:
0 : linha em branco ou fim da linha
? : ponteiro da palavra
----------
O ponteiro da posição será alterado para a última posição carregada
pela função.

*/
char *Carrega_Proxima_Palavra(Linha *linha, int *posicao) {
	char *Palavra = 0;
	int pos = 0;//posiçao do inicio da palavra
	int pos2 = 0;//posição do final da palvra
	int c = 0;
	if (linha) {
		if (posicao) {
			pos = posicao[0];
			while (pos < linha->QntCaracteres && linha->Caracteres[pos] == ' ') {
				pos++;
			}if (pos < linha->QntCaracteres) {// ainda não chegamos ao final da linha, então encontramos algo.
				pos2 = pos;
				while (pos2 < linha->QntCaracteres && linha->Caracteres[pos2] != ' ') {
					pos2++;
				}
				//sempre é garantido que ao final de cada linha não terá espaços depois da última palavra
				Palavra = (char*)malloc((pos2 - pos + 1) * sizeof(char));
				while (pos < pos2) {
					Palavra[c] = linha->Caracteres[pos];
					pos++;
					c++;
				}
				Palavra[c] = '\0';
				posicao[0] = pos2;//retorna a posição atualizada
			}
		}
	}
	return Palavra;
}

void SalvaTexto(Texto *texto) {
	int i = 0;
	FILE *OUT_TEXT;
	if (texto)
	{
		OUT_TEXT = fopen(texto->Fonte, "wb");
		if (OUT_TEXT)
		{
			while (i < texto->QntLinhas)
			{
				fprintf(OUT_TEXT, "%s\n", texto->Linhas[i].Caracteres);
				i++;
			}
			texto->Salvo = true;
			fclose(OUT_TEXT);
		}
	}
}