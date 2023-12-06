#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TAM_NOME 30
#define MAX_TAM_VALOR 50
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

TiposDados verificaTipo(char string[]);
void criarTabela();
void removerQuebraLinha(char *string);
void limparBuffer();


int main() 
{
    int escolhaMenu = 0;
    scanf("%d", &escolhaMenu);
    
    switch (escolhaMenu)
    {
    case 1: 
        criarTabela();
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
    limparBuffer();
    fgets(nomeTabela, MAX_TAM_NOME, stdin);

    int quantColunas = 0;

    printf("Informe quantas colunas a tabela terá inicialmente (mínimo 2): ");
    

    if (scanf("%d", &quantColunas) != 1 || quantColunas < 2) 
    {
        printf("Número de colunas inválido.\n");
        exit(1);
    }
    limparBuffer();

    Coluna coluna[quantColunas];
    char nomeColunaImput[MAX_TAM_NOME];

    for (int i = 0; i < quantColunas; i++) 
    {
        if (i == 0) 
        {
            printf("Informe o nome da coluna %d que será a chave primária: ", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);
            strcpy(nomeColunaImput, nomeColunaImput);
            removerQuebraLinha(nomeColunaImput);
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
        }
        else
        {
            printf("Informe o nome da coluna %d: \n", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);
            strcpy(nomeColunaImput, nomeColunaImput);
            removerQuebraLinha(nomeColunaImput);
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
        }
        
    }

    //primeira linha com valores

    unsigned int indexChavePrimaria = 1;
    for (int i = 0; i < quantColunas; i++)
    {
        if (i == 0)
        {
            printf("%d ", indexChavePrimaria);
            indexChavePrimaria++;
        }
        else
        {
            printf("Digite o valor da coluna %s: ", coluna[i].nomeColuna);
            
            char valorInput[MAX_TAM_VALOR];

            fgets(valorInput, MAX_TAM_VALOR, stdin);

            coluna[i].tipoColuna = verificaTipo(valorInput);

            switch (coluna[i].tipoColuna)
            {
            case TIPO_INT:
                coluna[i].valorColuna.valorInt = atoi(valorInput);
                break;
            case TIPO_CHAR:
                coluna[i].valorColuna.valorChar = valorInput[0];
                break;
            case TIPO_FLOAT:
                coluna[i].valorColuna.valorFloat = atof(valorInput);
                break;
            case TIPO_DOUBLE:
                coluna[i].valorColuna.valorDouble = atof(valorInput);
                break;
            case TIPO_STRING:
                strcpy(coluna[i].valorColuna.valorString, valorInput);
                break;
            default:
                break;
            }
        }
    }
}

TiposDados verificaTipo(char string[]) {
    // Verifica se é um número inteiro
    int valorInt = atoi(string);
    if (valorInt != 0 || string[0] == '0') 
    {
        return TIPO_INT;
    } 
    else 
    {
        // Verifica se é um número de ponto flutuante
        float valorFloat = atof(string);
        if (valorFloat != 0.0 || (valorFloat == 0.0 && string[0] == '0')) 
        {
            return TIPO_FLOAT;
        } 
        else 
        {
            // Tentativa de converter para double
            double valorDouble = atof(string);
            if (valorDouble != 0.0 || (valorDouble == 0.0 && string[0] == '0')) 
            {
                return TIPO_DOUBLE;
            } 
            else 
            {
                // Verifica se é um caractere
                if (isalpha(string[0])) 
                {
                    return TIPO_CHAR;
                } 
                else 
                {
                    return TIPO_STRING;
                }
            }
        }
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

  