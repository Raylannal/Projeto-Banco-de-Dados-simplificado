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
void criarTabela(FILE *arquivo);
void removerQuebraLinha(char *string);
void limparBuffer();


int main() 
{
    FILE *arquivoTabela = NULL;
    int escolhaMenu = 0;
    scanf("%d", &escolhaMenu);
    
    switch (escolhaMenu)
    {
    case 1: 
        criarTabela(arquivoTabela);
        break;
    case 2: //listar tabelas
        break;
    case 3: //criar nova linha na tabela
        break;
    case 4: //criar nova coluna na tabela
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

void criarTabela(FILE *arquivo) {
    char nomeTabela[MAX_TAM_NOME];

    printf("Informe o nome da tabela: ");
    limparBuffer();
    fgets(nomeTabela, MAX_TAM_NOME, stdin);
    removerQuebraLinha(nomeTabela);

// Verificando se já existe um arquivo com o mesmo nome
    // Tenta abrir o arquivo em modo leitura
    arquivo = fopen(nomeTabela, "r");
    if (arquivo != NULL)
    {
        // Arquivo com o mesmo nome já existe
        printf("Um arquivo com o nome %s já existe.\n", nomeTabela);
        fclose(arquivo);
        return;
    }
    // Cria o arquivo em modo escrita
    arquivo = fopen(nomeTabela, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    
    int quantColunas = 0;
    printf("Informe quantas colunas a tabela terá inicialmente (mínimo 2): ");
    // Teste para entradas válidas
    if (scanf("%d", &quantColunas) != 1 || quantColunas < 2) 
    {
        printf("Número de colunas inválido.\n");
        return;
    }
    limparBuffer();

    Coluna coluna[quantColunas];
    char nomeColunaImput[MAX_TAM_NOME];
    // Criação da primeira linha da tabela que contém os nomes
    for (int i = 0; i < quantColunas; i++) 
    {
        if (i == 0) 
        {
            printf("Informe o nome da coluna %d que será a chave primária: ", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);// Leitura do nome da coluna
            removerQuebraLinha(nomeColunaImput);
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
            fprintf(arquivo,"%s",coluna[i].nomeColuna);// Escreve no arquivo o nome da coluna
        }
        else
        {
            printf("Informe o nome da coluna %d, seguido do numero correspondente ao tipo de dado, separado por '|': (0 = INT) (1 = CHAR) (2 = FLOAT) (3 = DOUBLE) (4 = STRING) ", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);
            removerQuebraLinha(nomeColunaImput);
            coluna[i].tipoColuna = verificaTipo(nomeColunaImput, coluna, i);// Separa o nome da coluna do seu tipo de dado
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
            fprintf(arquivo,"|-|%s", coluna[i].nomeColuna);// Escreve no arquivo o nome da coluna
        }
        
    }
    fprintf(arquivo,";;\n");// Termina a linha com ";;" e muda o ponteiro para iniciar a proxima linha

    //primeira linha com valores
    unsigned int indexChavePrimaria = 0;
    for (int i = 0; i < quantColunas; i++)
    {
        if (i == 0)
        {
            indexChavePrimaria++;
            fprintf(arquivo,"%d",indexChavePrimaria);// Escreve o valor da coluna chave primaria
        }
        else
        {
            printf("Digite o valor da coluna %s: ", coluna[i].nomeColuna);
            char valorInput[MAX_TAM_VALOR];
            fgets(valorInput, MAX_TAM_VALOR, stdin);
            removerQuebraLinha(valorInput);

            switch (coluna[i].tipoColuna) // Verifica o tipo de dado e escreve no arquivo depois da conversão necessária
            {
            case TIPO_INT:
                coluna[i].valorColuna.valorInt = atoi(valorInput);
                fprintf(arquivo,"|-|%d", coluna[i].valorColuna.valorInt);
                break;
            case TIPO_CHAR:
                coluna[i].valorColuna.valorChar = valorInput[0];
                fprintf(arquivo,"|-|%c", coluna[i].valorColuna.valorChar);
                break;
            case TIPO_FLOAT:
                coluna[i].valorColuna.valorFloat = atof(valorInput);
                fprintf(arquivo,"|-|%f", coluna[i].valorColuna.valorFloat);
                break;
            case TIPO_DOUBLE:
                coluna[i].valorColuna.valorDouble = atof(valorInput);
                fprintf(arquivo,"|-|%lf", coluna[i].valorColuna.valorDouble);
                break;
            case TIPO_STRING:
                strcpy(coluna[i].valorColuna.valorString, valorInput);
                fprintf(arquivo,"|-|%s", coluna[i].valorColuna.valorString);
                break;
            default:
                break;
            }
        }
    }
    fprintf(arquivo,";;\n");// Termina a linha com ";;" e muda o ponteiro para iniciar a proxima linha

    fclose(arquivo);// Fecha o arquivo

}

TiposDados verificaTipo(char string[], Coluna *coluna, int i) {
    char *pedaco;
    pedaco = strtok(string, "|");
    strcpy(coluna[i].nomeColuna, pedaco);

    pedaco = strtok(NULL, "|");
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

  