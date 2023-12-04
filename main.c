#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAM_NOME 30

typedef enum {
    TIPO_INT,
    TIPO_CHAR,
    TIPO_FLOAT,
    TIPO_DOUBLE,
    TIPO_STRING
}TiposDados;

typedef union {
    unsigned int valorChaveP;
    int valorInt;
    char valorChar;
    float valorFloat;
    double valorDouble;
    char valorString[50];
}Valor;


typedef struct {
    char nomeColuna[MAX_TAM_NOME];
    TiposDados tipoColuna;
    Valor valorColuna;
}Coluna;

int main(int argc, char const *argv[]) 
{
    int escolhaMenu = 0;
    
    switch (escolhaMenu)
    {
    case 1: //criar nova tabelna
        break;
    case 2: //listar tabelas
        break;
    case 3: //criar nova linha na tabelna
        break;
    case 4: //criar nova coluna na tabelna
        break;
    case 5: //listar dados tabela
        break;
    case 6: //pesquisar valor tabela
        break;
    case 7: //apagar tupla
        break;
    case 8: //apagar tabela
        break;
    
    default:
        break;
    }

    return 0;
}

void criarTabela() {
    char nomeTabela[MAX_TAM_NOME];

    printf("Informe o nome da tabela: ");
    scanf("%s", nomeTabela);

    int quantColunas = 0;

    printf("Informe quantas colunas a tabela terá inicialmente: ");
    

    if (scanf("%d", &quantColunas) != 1 || quantColunas <= 0) 
    {
        printf("Número de colunas inválido.\n");
        exit(1);
    }

    Coluna coluna[quantColunas];

    for (int i = 0; i < quantColunas; i++) 
    {
        if (i == 0) 
        {
            printf("Informe o nome da coluna que será a chave primária: ");
        }
        else
        {
            
        }
        
        
        
        
    }
    
    








}

