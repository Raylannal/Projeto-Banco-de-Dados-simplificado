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

TiposDados verificaTipo(char string[], Coluna *coluna, int i);
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
            printf("Informe o nome da coluna %d, seguido do numero do tipo de dado, separado por '|||': (0 = INT) (1 = CHAR) (2 = FLOAT) (3 = DOUBLE) (4 = STRING) ", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);
            strcpy(nomeColunaImput, nomeColunaImput);
            removerQuebraLinha(nomeColunaImput);
            coluna[i].tipoColuna = verificaTipo(nomeColunaImput, coluna, i);
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
        }
        
    }

    //primeira linha com valores

    unsigned int indexChavePrimaria = 0;
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
            removerQuebraLinha(valorInput);

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
    
    for (int i = 0; i < quantColunas; i++) 
    {
        printf("%s\t", coluna[i].nomeColuna);
    }
    printf("\n");

    // Imprimir dados
    for (int i = 0; i < quantColunas; i++) 
    {
        if (i == 0) 
        {
            printf("%u\t", indexChavePrimaria);
        } 
        else 
        {
            
            switch (coluna[i].tipoColuna) 
            {
                case TIPO_INT:
                    printf("%d\t", coluna[i].valorColuna.valorInt);
                    break;
                case TIPO_CHAR:
                    printf("%c\t", coluna[i].valorColuna.valorChar);
                    break;
                case TIPO_FLOAT:
                    printf("%f\t", coluna[i].valorColuna.valorFloat);
                    break;
                case TIPO_DOUBLE:
                    printf("%lf\t", coluna[i].valorColuna.valorDouble);
                    break;
                case TIPO_STRING:
                    printf("%s\t", coluna[i].valorColuna.valorString);
                    break;
                default:
                    break;
            }
        }
    }
    printf("\n");
    

}

TiposDados verificaTipo(char string[], Coluna *coluna, int i) {
    char *pedaco;
    pedaco = strtok(string, "|||");
    strcpy(coluna[i].nomeColuna, pedaco);

    pedaco = strtok(NULL, "|||");
    int tipo = atoi(pedaco);
    printf("TIPO AQQQQQQ %d\n", tipo);
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
        return -1;
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

  