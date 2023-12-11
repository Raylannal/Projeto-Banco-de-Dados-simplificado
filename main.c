#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

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
void listarTabelas();


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
        listarTabelas();
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

    limparBuffer();
    printf("Informe o nome da tabela: ");
    fgets(nomeTabela, MAX_TAM_NOME, stdin);
    removerQuebraLinha(nomeTabela);
    char extensao[5] = ".itp";

    strcat(nomeTabela, extensao); // Concatena o nome da tabela com a extensão ".itp"

// Verificando se já existe um arquivo com o mesmo nome
    // Tenta abrir o arquivo em modo leitura
    arquivo = fopen(nomeTabela, "r");
    if (arquivo != NULL)
    {
        // Arquivo com o mesmo nome já existe
        printf("Um arquivo com o nome %s já existe.\n", nomeTabela);
        fclose(arquivo);
        return 1;
    }
    // Cria o arquivo em modo escrita
    arquivo = fopen(nomeTabela, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo.\n");
        return 1;
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
            printf("Informe o nome da coluna %d, seguido do numero correspondente ao tipo de dado, separado por '||': (0 = INT) (1 = CHAR) (2 = FLOAT) (3 = DOUBLE) (4 = STRING) ", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);
            removerQuebraLinha(nomeColunaImput);
            coluna[i].tipoColuna = verificaTipo(nomeColunaImput, coluna, i);// Separa o nome da coluna do seu tipo de dado
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
            fprintf(arquivo,"||%s", coluna[i].nomeColuna);// Escreve no arquivo o nome da coluna
        }
        
    }
    fprintf(arquivo,";;\n");// Termina a linha com ";;" e muda o ponteiro para iniciar a proxima linha
    fseek(arquivo, 0, SEEK_SET); // Retorna o ponteiro para inicio do arquivo para escrever os tipos de dados recebidos

    for (int i = 1; i < quantColunas; i++) // Escreve os tipos de dados na primeira linha
    {
        if (i == 0)
        {
            fprintf(arquivo, "0");
        }
        else
        {
            fprintf(arquivo, "||%d", coluna[i].tipoColuna);
        }   
    }
    fprintf(arquivo,"\n");
    
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
                fprintf(arquivo,"||%d", coluna[i].valorColuna.valorInt);
                break;
            case TIPO_CHAR:
                coluna[i].valorColuna.valorChar = valorInput[0];
                fprintf(arquivo,"||%c", coluna[i].valorColuna.valorChar);
                break;
            case TIPO_FLOAT:
                coluna[i].valorColuna.valorFloat = atof(valorInput);
                fprintf(arquivo,"||%f", coluna[i].valorColuna.valorFloat);
                break;
            case TIPO_DOUBLE:
                coluna[i].valorColuna.valorDouble = atof(valorInput);
                fprintf(arquivo,"||%lf", coluna[i].valorColuna.valorDouble);
                break;
            case TIPO_STRING:
                strcpy(coluna[i].valorColuna.valorString, valorInput);
                fprintf(arquivo,"||%s", coluna[i].valorColuna.valorString);
                break;
            default:
                break;
            }
        }
    }
    fprintf(arquivo,"\n");// Termina a linha com ";;" e muda o ponteiro para iniciar a proxima linha

    fclose(arquivo);// Fecha o arquivo

}

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

void listarTabelas() {
    DIR *dir;
    struct dirent *ent;

    char *diretorio = "../output";
    char *extensao = ".itp";

    if ((dir = opendir(diretorio)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            // Verifica se o nome do arquivo termina com a extensão "".itp"
            int tamanhoNome = strlen(ent->d_name);
            int tamanhoExtensao = strlen(extensao);
            if (tamanhoNome >= tamanhoExtensao && strcmp(ent->d_name + tamanhoNome - tamanhoExtensao, extensao) == 0)
            {
                printf("Tabela: %s\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    else {
        perror("Erro ao abrir o diretório");
    }
}

void criarLinha(FILE *arquivo, char nomeTabela[MAX_TAM_NOME]) {
    char linhaDeTipos[MAX_TAM_NOME]; // Considerando que terão no máximo 15 colunas
    char linhaNomesColunas[255]; 
    arquivo = fopen(nomeTabela, "r+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo");
        return 1;
    }
    fscanf(arquivo, "%s", linhaDeTipos); // Lê a primeira linha do arquivo que informa os tipos do dados
    fscanf(arquivo, "%s", linhaNomesColunas); // Lê a segunda linha do arquivo que informa nos nomes das colunas
    
    
}

  