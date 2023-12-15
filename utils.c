#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#include "definitions.h"
#include "controllers.h"

TiposDados verificaTipo(char string[], Coluna *coluna, int i) {
    char *pedaco;
    pedaco = strtok(string, "||");
    strcpy(coluna[i].nomeColuna, pedaco);

    pedaco = strtok(NULL, "||");
    int tipo = atoi(pedaco);
    switch (tipo)
    {
    case 0:
        return TIPO_INT;
        break;
    case 1:
        return TIPO_CHAR;
        break;
    case 2:
        return TIPO_FLOAT;
        break;
    case 3:
        return TIPO_DOUBLE;
        break;
    case 4:
        return TIPO_STRING;
        break;
    default:
        printf("Tipo de dado inválido.\n");
        exit(1);
        break;
    }
}

void removerQuebraLinha(char *string) {
    int tamanho = strlen(string);
    if (string[tamanho - 1] == '\n') {
        string[tamanho - 1] = '\0';
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int pegarDados(char string[], Coluna *coluna, int opcao) {
    char *pedacoLinha;
    int i = 0;

    pedacoLinha = strtok(string, "||");
    switch (opcao)
    {
    case 1:
        while (pedacoLinha != NULL)
        {
            int tipo = atoi(pedacoLinha);
            pedacoLinha = strtok(NULL, "||");
            coluna[i].tipoColuna = tipo;
            i++;
        }
        break;
    case 2:
        while (pedacoLinha != NULL)
        {
            strcpy(coluna[i].nomeColuna, pedacoLinha);
            pedacoLinha = strtok(NULL, "||");
            i++;
        }
        break;
    case 3:
        while (pedacoLinha != NULL)
        {
            switch (coluna[i].tipoColuna)
            {
            case 0:
                coluna[i].valorColuna.valorChaveP = atoi(pedacoLinha);
                break;
            case 1:
                coluna[i].valorColuna.valorChar = pedacoLinha[0];
                break;
            case 2:
                coluna[i].valorColuna.valorFloat = atof(pedacoLinha);
                break;
            case 3:
                coluna[i].valorColuna.valorDouble = atof(pedacoLinha);
                break;
            case 4:
                strcpy(coluna[i].valorColuna.valorString, pedacoLinha);
                break;
            default:
                printf("Tipo de dado inválido.\n");
                return 1;
                break;
            }
            pedacoLinha = strtok(NULL, "||");
            i++;
        }
        break;
    default:
        break;
    }
    return 0; 
}

void testaNome(FILE *arquivo, char nomeTabela[]){
    arquivo = fopen(nomeTabela, "r");
        if (arquivo == NULL)
        {
            printf("Nome de tabela invalido ou inexistente.\n");
            printf("As tabelas existentes são: \n");
            listarTabelas();
            exit(1);
        }
        else {
            fclose(arquivo);
        }
}
