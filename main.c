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
int pegarDados(char string[], Coluna *coluna, int opcao);
void criarLinha(FILE *arquivo, char nomeTabela[]);


int main() 
{
    FILE *arquivoTabela = NULL;
    char nomeTabela[MAX_TAM_NOME];
    int escolhaMenu = 0;
    scanf("%d", &escolhaMenu);
    limparBuffer();

    switch (escolhaMenu)
    {
    case 1: 
        criarTabela(arquivoTabela);
        break;
    case 2: //listar tabelas
        listarTabelas();
        break;
    case 3: //criar nova linha na tabela
        printf("Informe em qual tabela (nome e extensão) deseja adicionar uma nova linha:  \n");
        fgets(nomeTabela, MAX_TAM_NOME, stdin);
        removerQuebraLinha(nomeTabela);

        arquivoTabela = fopen(nomeTabela, "r");
        if (arquivoTabela == NULL)
        {
            printf("Nome de tabela invalido ou inexistente.\n");
            printf("As tabelas existentes são: \n");
            listarTabelas();
            return 1;
        }
        else {
            fclose(arquivoTabela);
        }
        criarLinha(arquivoTabela, nomeTabela);
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
        }
        else
        {
            printf("Informe o nome da coluna %d, seguido do numero correspondente ao tipo de dado, separado por '||': (0 = INT) (1 = CHAR) (2 = FLOAT) (3 = DOUBLE) (4 = STRING) ", i + 1);
            fgets(nomeColunaImput, MAX_TAM_NOME, stdin);
            removerQuebraLinha(nomeColunaImput);
            coluna[i].tipoColuna = verificaTipo(nomeColunaImput, coluna, i);// Separa o nome da coluna do seu tipo de dado
            strcpy(coluna[i].nomeColuna, nomeColunaImput);
        }
    }

    for (int i = 0; i < quantColunas; i++) // Escreve os tipos de dados na primeira linha
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
    fputc('\n', arquivo);

    for (int i = 0; i < quantColunas; i++)
    {
        if (i == 0)
        {
            fprintf(arquivo,"%s",coluna[i].nomeColuna);// Escreve no arquivo o nome da coluna
        }
        else {
            fprintf(arquivo,"||%s", coluna[i].nomeColuna);// Escreve no arquivo o nome da coluna
        }
        
    }
    fprintf(arquivo,"\n");
    fclose(arquivo);// Fecha o arquivo

    criarLinha(arquivo, nomeTabela);

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
            // Verifica se o nome do arquivo termina com a extensão ".itp"
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

int pegarDados(char string[], Coluna *coluna, int opcao) {
    char *pedacoLinha;
    int contColunas = 0, i = 0;

    pedacoLinha = strtok(string, "||");
    switch (opcao)
    {
    case 1:
        while (pedacoLinha != NULL)
        {
            int tipo = atoi(pedacoLinha);
            pedacoLinha = strtok(NULL, "||");
            coluna[i].tipoColuna = tipo;
            contColunas++;
            i++;
        }
        return contColunas;
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
                printf("Tipo de dado inválidohfhfh.\n");
                return -1;
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


void criarLinha(FILE *arquivo, char nomeTabela[]) {
    int quantColunas = 5; // Considerando que a tabela contem inicialmente no máximo 5 colunas
    Coluna linhaNova[quantColunas];
    char linhaDeTipos[MAX_TAM_NOME];
    char linhaAtual[255]; 

    arquivo = fopen(nomeTabela, "r+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo");
        return;
    }

    fscanf(arquivo, "%s", linhaDeTipos); // Lê a primeira linha do arquivo que informa os tipos do dados
    quantColunas = pegarDados(linhaDeTipos, linhaNova, 1);
    
    fscanf(arquivo, "%s", linhaAtual); // Lê a segunda linha do arquivo que informa nos nomes das colunas
    pegarDados(linhaAtual, linhaNova, 2);

    int contadorLinhasRegistro = 0;
    int quantLinhas = 5;
    int listaChavesP[quantLinhas];

    while (fgets(linhaAtual, 255, arquivo) != NULL)
    {
        pegarDados(linhaAtual, linhaNova, 3);
        listaChavesP[contadorLinhasRegistro] = linhaNova[0].valorColuna.valorChaveP;
        contadorLinhasRegistro++;
    }
    

    if (contadorLinhasRegistro == 0) // Escreve o primeiro registro
    {
        for (int i = 0; i < quantColunas; i++)
        {
            if (i == 0)
            {
                fprintf(arquivo,"1");// Inicia a chave primaria do primeiro registro = 1
            }
            else
            {
                printf("Digite o valor da coluna %s: ", linhaNova[i].nomeColuna);
                char valorInput[MAX_TAM_VALOR];
                fgets(valorInput, MAX_TAM_VALOR, stdin);
                removerQuebraLinha(valorInput);

                switch (linhaNova[i].tipoColuna) // Verifica o tipo de dado e escreve no arquivo depois da conversão necessária
                {
                case TIPO_INT:
                    linhaNova[i].valorColuna.valorInt = atoi(valorInput);
                    fprintf(arquivo,"||%d", linhaNova[i].valorColuna.valorInt);
                    break;
                case TIPO_CHAR:
                    linhaNova[i].valorColuna.valorChar = valorInput[0];
                    fprintf(arquivo,"||%c", linhaNova[i].valorColuna.valorChar);
                    break;
                case TIPO_FLOAT:
                    linhaNova[i].valorColuna.valorFloat = atof(valorInput);
                    fprintf(arquivo,"||%f", linhaNova[i].valorColuna.valorFloat);
                    break;
                case TIPO_DOUBLE:
                    linhaNova[i].valorColuna.valorDouble = atof(valorInput);
                    fprintf(arquivo,"||%lf", linhaNova[i].valorColuna.valorDouble);
                    break;
                case TIPO_STRING:
                    strcpy(linhaNova[i].valorColuna.valorString, valorInput);
                    fprintf(arquivo,"||%s", linhaNova[i].valorColuna.valorString);
                    break;
                default:
                    break;
                }
            }
        }
        fputc('\n', arquivo);
    }
    else
    {
        unsigned int indexNovaLinha = 1;
        int jaTemEsse = 0;
        for (int i = indexNovaLinha; i <= contadorLinhasRegistro; i++)
        {
            jaTemEsse = 0;
            for (int j = 0; j < contadorLinhasRegistro; j++)
            {
                if (listaChavesP[j] == i)
                {
                    jaTemEsse = 1;
                    break;
                }
            }
            if (jaTemEsse == 0)
            {
                indexNovaLinha = i;
                break;
            }
        }

        fseek(arquivo, 0, SEEK_END); // Posiciona o ponteiro no final do arquivo para inserir a nova linha

        for (int i = 0; i < quantColunas; i++)
        {
            if (i == 0)
            {
                fprintf(arquivo,"%d", indexNovaLinha);
            }
            else
            {
                printf("Digite o valor da coluna %s: ", linhaNova[i].nomeColuna);
                char valorInput[MAX_TAM_VALOR];
                fgets(valorInput, MAX_TAM_VALOR, stdin);
                removerQuebraLinha(valorInput);

                switch (linhaNova[i].tipoColuna) // Verifica o tipo de dado e escreve no arquivo depois da conversão necessária
                {
                case TIPO_INT:
                    linhaNova[i].valorColuna.valorInt = atoi(valorInput);
                    fprintf(arquivo,"||%d", linhaNova[i].valorColuna.valorInt);
                    break;
                case TIPO_CHAR:
                    linhaNova[i].valorColuna.valorChar = valorInput[0];
                    fprintf(arquivo,"||%c", linhaNova[i].valorColuna.valorChar);
                    break;
                case TIPO_FLOAT:
                    linhaNova[i].valorColuna.valorFloat = atof(valorInput);
                    fprintf(arquivo,"||%f", linhaNova[i].valorColuna.valorFloat);
                    break;
                case TIPO_DOUBLE:
                    linhaNova[i].valorColuna.valorDouble = atof(valorInput);
                    fprintf(arquivo,"||%lf", linhaNova[i].valorColuna.valorDouble);
                    break;
                case TIPO_STRING:
                    strcpy(linhaNova[i].valorColuna.valorString, valorInput);
                    fprintf(arquivo,"||%s", linhaNova[i].valorColuna.valorString);
                    break;
                default:
                    break;
                }
            }
        }
        fputc('\n', arquivo);
    }
    fclose(arquivo);
}


  