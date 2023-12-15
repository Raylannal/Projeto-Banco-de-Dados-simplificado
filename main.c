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

typedef struct {
    char valorIgual[MAX_TAM_VALOR];
    char valorMaior[MAX_TAM_VALOR];
    char valorMenor[MAX_TAM_VALOR];
}Lista;


TiposDados verificaTipo(char string[], Coluna *coluna, int i);
void criarTabela(FILE *arquivo);
void removerQuebraLinha(char *string);
void limparBuffer();
void listarTabelas();
int pegarDados(char string[], Coluna *coluna, int opcao);
void criarLinha(FILE *arquivo, char nomeTabela[]);
void testaNome(FILE *arquivo, char nomeTabela[]);
void listarDadosTabela(FILE *arquivo, char nomeTabela[]);
void pesquisaValor(FILE *arquivo, char nomeTabela[]);
void apagarLinha(FILE *arquivo, char nomeTabela[]);


int main() 
{
    FILE *arquivoTabela = NULL;
    char nomeTabela[MAX_TAM_NOME];
    int escolhaMenu = 8;
    
    while (escolhaMenu != 0)
    {
        printf("menu \n");
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

                testaNome(arquivoTabela, nomeTabela); // Testa se o nome da tabela é válido

                criarLinha(arquivoTabela, nomeTabela);
                break;
            case 4: //listar dados tabela
                printf("Qual é o nome da tabela que você deseja listar? (Inclua a extensão) \n");
                fgets(nomeTabela, MAX_TAM_NOME, stdin);
                removerQuebraLinha(nomeTabela);

                testaNome(arquivoTabela, nomeTabela);

                listarDadosTabela(arquivoTabela, nomeTabela);
                break;
            case 5: //pesquisar valor tabela
                printf("Informe o nome da tabela (nome e extensão) para realizar a pesquisa: \n");
                fgets(nomeTabela, MAX_TAM_NOME, stdin);
                removerQuebraLinha(nomeTabela);

                testaNome(arquivoTabela, nomeTabela);

                pesquisaValor(arquivoTabela, nomeTabela);
                break;
            case 6: //apagar linha
                printf("Informe o nome da tabela (nome e extensão) para deletar uma linha: \n");
                fgets(nomeTabela, MAX_TAM_NOME, stdin);
                removerQuebraLinha(nomeTabela);

                testaNome(arquivoTabela, nomeTabela);

                apagarLinha(arquivoTabela, nomeTabela);
                break;
            case 7: //apagar tabela
                break;
            default:
                break;
        }
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

    fprintf(arquivo, "%d\n", quantColunas); // Escreve a quantidade de colunas na primeira linha

    Coluna coluna[quantColunas];
    char nomeColunaImput[MAX_TAM_NOME];

    // Criação da linha da tabela que contém os nomes
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

    for (int i = 0; i < quantColunas; i++) // Escreve os tipos de dados na segunda linha
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


void criarLinha(FILE *arquivo, char nomeTabela[]) {
    arquivo = fopen(nomeTabela, "r+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo. \n");
        return;
    }

    int quantColunas = 0;
    fscanf(arquivo, "%d\n", &quantColunas);
    Coluna linhaNova[quantColunas];
    int maxTamLinha = (quantColunas + 1) * MAX_TAM_VALOR; // Como cada nome da coluna tem o seu tamanho de até MAX_TAM_VALOR, então a linha contendo todos os nomes terá no máximo = quantintade de colunas * MAX_TAM_NOME + caracteres separadores. Então a quantidade de colunas + 1 comporta toda a linha

    char linhaAtual[maxTamLinha];
    
    fgets(linhaAtual, maxTamLinha, arquivo);// Lê a segunda linha do arquivo que informa os tipos do dados
    pegarDados(linhaAtual, linhaNova, 1); // Separa os tipos de dados

    fgets(linhaAtual, maxTamLinha, arquivo); // Lê a terceira linha do arquivo que informa nos nomes das colunas
    removerQuebraLinha(linhaAtual);
    pegarDados(linhaAtual, linhaNova, 2);

    int contadorLinhasRegistro = 0;
    int quantLinhas = 5;
    int *listaChavesP = malloc(sizeof(int) * quantLinhas);
    if (listaChavesP == NULL)
    {
        printf("Falha na alocação de memória.\n");
        fclose(arquivo);
        return;
    }
    

    // Verifica se já existe alguma linha de registro
    while (fgets(linhaAtual, maxTamLinha, arquivo) != NULL)
    {
        if (contadorLinhasRegistro > quantLinhas)
        {
            quantLinhas += 10;
            int *listaChaveMaior = realloc(listaChavesP, sizeof(int) * quantLinhas);
            if (listaChaveMaior == NULL)
            {
                printf("Falha na realocação de memória para .\n");
                free(listaChavesP);
                fclose(arquivo);
                return;
            }
            listaChavesP = listaChaveMaior;
        }
        pegarDados(linhaAtual, linhaNova, 3);
        listaChavesP[contadorLinhasRegistro] = linhaNova[0].valorColuna.valorChaveP;
        contadorLinhasRegistro++;
    }
    

    if (contadorLinhasRegistro == 0) // Se não houver algum registro, então escreve o primeiro registro
    {
        for (int i = 0; i < quantColunas; i++)
        {
            if (i == 0)
            {
                fprintf(arquivo,"1");// Inicia a chave primaria do primeiro registro = 1
            }
            else
            {
                printf("Digite o valor da coluna %s (máximo 50 caracteres): ", linhaNova[i].nomeColuna);
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
        // Se já existem outras linhas então irá definir o índice da nova linha verificando se o posível candidado ja pertence a outro registro
        for (int i = indexNovaLinha; i <= contadorLinhasRegistro + 1; i++)// O loop inicia em 1 e vai até uma unidade a mais do número de linhas de registro. Dessa forma, o novo índice sempre será o menor valor possível
        {
            jaTemEsse = 0;
            for (int j = 0; j < contadorLinhasRegistro; j++)// Percorre todo o vetor que contém as chaves primárias da tabela
            {
                if (listaChavesP[j] == i)
                {
                    jaTemEsse = 1; // Se o candidato ao novo índice for igual a algum já existente, então interrompe o loop interno e atribui 1 a JaTemEsse
                    break;
                }
            }
            if (jaTemEsse == 0)// Caso o possível índice seja diferente dos já existentes, então esse é válido para ser o nomo índice
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
                printf("Digite o valor da coluna %s (máximo 50 caracteres): ", linhaNova[i].nomeColuna);
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

    free(listaChavesP);
    fclose(arquivo);
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


void listarDadosTabela(FILE *arquivo, char nomeTabela[]) {
    char linhaAtual[255];

    arquivo = fopen(nomeTabela, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    fgets(linhaAtual, 255, arquivo); // Pega a primeira linha que contém os tipos para que ela não seja impressa

    while (fgets(linhaAtual, 255, arquivo) != NULL) // Imprime as linhas do arquivo até chegar ao final
    {
        printf("%s", linhaAtual);
    }
    fclose(arquivo);
}

void pesquisaValor(FILE *arquivo, char nomeTabela[]) {
    arquivo = fopen(nomeTabela, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int quantColunas = 0;
    fscanf(arquivo, "%d\n", &quantColunas);
    Coluna colunaTabela[quantColunas];
    int maxTamLinha = (quantColunas + 1) * MAX_TAM_VALOR; // Como cada nome da coluna tem o seu tamanho de até MAX_TAM_VALOR, então a linha contendo todos os nomes terá no máximo = quantintade de colunas * MAX_TAM_NOME + caracteres separadores. Então a quantidade de colunas + 1 comporta toda a linha

    char linhaAtual[maxTamLinha];
    
    fgets(linhaAtual, maxTamLinha, arquivo);// Lê a segunda linha do arquivo que informa os tipos do dados
    pegarDados(linhaAtual, colunaTabela, 1); // Separa os tipos de dados

    fgets(linhaAtual, maxTamLinha, arquivo); // Lê a terceira linha do arquivo que informa nos nomes das colunas
    removerQuebraLinha(linhaAtual);
    pegarDados(linhaAtual, colunaTabela, 2);

    printf("Digite o número correspondente à coluna em que deseja realizar a pesquisa: \n");
    printf("As colunas disponíveis são: \n");
    for (int i = 0; i < quantColunas; i++)
    {
        printf("%d - %s\n", i + 1, colunaTabela[i].nomeColuna);
    }

    int escolha = -1;
    scanf("%d", &escolha);
    limparBuffer();
    escolha -= 1;
    if (escolha < 0 || escolha >= quantColunas)
    {
        printf("Coluna inválida.\n");
        fclose(arquivo);
        return;
    }
    
    char valorPesquisado[MAX_TAM_VALOR];
    printf("Informe o valor a ser pesquisado na coluna %s: ", colunaTabela[escolha].nomeColuna);
    fgets(valorPesquisado, MAX_TAM_VALOR, stdin);
    removerQuebraLinha(valorPesquisado);
    
    int indexValorIgual = 0, indexValorMaior = 0, indexValorMenor = 0;
    int maiorQuantLinhas = 1;

    Lista *listaDeComparacao = malloc(sizeof(Lista) * maiorQuantLinhas);

    while (fgets(linhaAtual, maxTamLinha, arquivo) != NULL)
    {
        char linhaCopiaAtual[maxTamLinha];
        strcpy(linhaCopiaAtual, linhaAtual);
        removerQuebraLinha(linhaCopiaAtual);

        if (indexValorIgual + 1 > maiorQuantLinhas)
        {
            maiorQuantLinhas += 10;
            Lista *listaGrande = realloc(listaDeComparacao, sizeof(Lista) * maiorQuantLinhas);
            if (listaGrande == NULL)
            {
                printf("Falha na realocação de memória.\n");
                free(listaDeComparacao);
                fclose(arquivo);
                return;
            }
            listaDeComparacao = listaGrande;
        }
        if (indexValorMaior + 1> maiorQuantLinhas)
        {
            maiorQuantLinhas += 10;
            Lista *listaGrande = realloc(listaDeComparacao, sizeof(Lista) * maiorQuantLinhas);
            if (listaGrande == NULL)
            {
                printf("Falha na realocação de memória.\n");
                free(listaDeComparacao);
                fclose(arquivo);
                return;
            }
            listaDeComparacao = listaGrande;
        }
        if (indexValorMenor + 1 > maiorQuantLinhas)
        {
            maiorQuantLinhas += 10;
            Lista *listaGrande = realloc(listaDeComparacao, sizeof(Lista) * maiorQuantLinhas);
            if (listaGrande == NULL)
            {
                printf("Falha na realocação de memória.\n");
                free(listaDeComparacao);
                fclose(arquivo);
                return;
            }
            listaDeComparacao = listaGrande;
        }

        pegarDados(linhaAtual, colunaTabela, 3);

        int valorIntInput;
        float valorFloatInput;
        double valorDoubletInput;
        switch (colunaTabela[escolha].tipoColuna)
        {
            case TIPO_INT:
                valorIntInput = atoi(valorPesquisado);
                if (colunaTabela[escolha].valorColuna.valorInt == valorIntInput)
                {
                    strcpy(listaDeComparacao[indexValorIgual].valorIgual, linhaCopiaAtual);
                    indexValorIgual++;
                }
                else if (colunaTabela[escolha].valorColuna.valorInt < valorIntInput)
                {
                    strcpy(listaDeComparacao[indexValorMenor].valorMenor, linhaCopiaAtual);
                    indexValorMenor++;
                }
                else
                {
                    strcpy(listaDeComparacao[indexValorMaior].valorMaior, linhaCopiaAtual);
                    indexValorMaior++;
                }
                break;
            case TIPO_CHAR:
                if (colunaTabela[escolha].valorColuna.valorChar == valorPesquisado[0])
                {
                    strcpy(listaDeComparacao[indexValorIgual].valorIgual, linhaCopiaAtual);
                    indexValorIgual++;
                }
                else if (colunaTabela[escolha].valorColuna.valorChar < valorPesquisado[0])
                {
                    strcpy(listaDeComparacao[indexValorMenor].valorMenor, linhaCopiaAtual);
                    indexValorMenor++;
                }
                else
                {
                    strcpy(listaDeComparacao[indexValorMaior].valorMaior, linhaCopiaAtual);
                    indexValorMaior++;
                }
                break;
            case TIPO_FLOAT:
                valorFloatInput = atof(valorPesquisado);
                if (colunaTabela[escolha].valorColuna.valorFloat == valorFloatInput)
                {
                    strcpy(listaDeComparacao[indexValorIgual].valorIgual, linhaCopiaAtual);
                    indexValorIgual++;
                }
                else if (colunaTabela[escolha].valorColuna.valorFloat < valorFloatInput)
                {
                    strcpy(listaDeComparacao[indexValorMenor].valorMenor, linhaCopiaAtual);
                    indexValorMenor++;
                }
                else
                {
                    strcpy(listaDeComparacao[indexValorMaior].valorMaior, linhaCopiaAtual);
                    indexValorMaior++;
                }
                break;
            case TIPO_DOUBLE:
                valorDoubletInput = atof(valorPesquisado);
                if (colunaTabela[escolha].valorColuna.valorDouble == valorDoubletInput)
                {
                    strcpy(listaDeComparacao[indexValorIgual].valorIgual, linhaCopiaAtual);
                    indexValorIgual++;
                }
                else if (colunaTabela[escolha].valorColuna.valorDouble < valorDoubletInput)
                {
                    strcpy(listaDeComparacao[indexValorMenor].valorMenor, linhaCopiaAtual);
                    indexValorMenor++;
                }
                else
                {
                    strcpy(listaDeComparacao[indexValorMaior].valorMaior, linhaCopiaAtual);
                    indexValorMaior++;
                }
                break;
            case TIPO_STRING:
                if (strcmp(colunaTabela[escolha].valorColuna.valorString, valorPesquisado) == 0)
                {
                    strcpy(listaDeComparacao[indexValorIgual].valorIgual, linhaCopiaAtual);
                    indexValorIgual++;
                    break;
                }
                else if (colunaTabela[escolha].valorColuna.valorString < valorPesquisado)
                {
                    strcpy(listaDeComparacao[indexValorMenor].valorMenor, linhaCopiaAtual);
                    indexValorMenor++;
                    break;
                }
                else
                {
                    strcpy(listaDeComparacao[indexValorMaior].valorMaior, linhaCopiaAtual);
                    indexValorMaior++;
                    break;
                }
                break;
            default:
                break;
        }
    }

    printf("\n");
    printf("Os valores menores que o informado são:\n");
    if (indexValorMenor == 0)
    {
        printf("Não há valores menores do que o informado.\n");
    }
    else
    {
        for (int i = 0; i < indexValorMenor; i++)
        {
            printf("%s\n", listaDeComparacao[i].valorMenor);
        }
    }
    printf("\n");
    printf("Os valores iguais ao informado são:\n");
    if (indexValorIgual == 0)
    {
        printf("Não há valores iguais ao informado.\n");
    }
    else
    {
        for (int i = 0; i < indexValorIgual; i++)
        {
            printf("%s\n", listaDeComparacao[i].valorIgual);
        }
    }
    printf("\n");
    printf("Os valores maiores que o informado são:\n");
    if (indexValorMaior == 0)
    {
        printf("Não há valores maiores do que o informado.\n");
    }
    else
    {
        for (int i = 0; i < indexValorMaior; i++)
        {
            printf("%s\n", listaDeComparacao[i].valorMaior);
        }
    }
    
    free(listaDeComparacao);
    fclose(arquivo);
}

void apagarLinha(FILE *arquivo, char nomeTabela[]) {
    FILE *arquivoDestino; // Cria um novo arquivo temporario para reescrever o arquivo original sem a linha deletada

    arquivo = fopen(nomeTabela, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    arquivoDestino = fopen("temp.itp", "w");
    if (arquivoDestino == NULL)
    {
        fclose(arquivo);
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    unsigned int escolha = 0;
    printf("Informe o valor da chave primária a ser excluído: \n");
    scanf("%u", &escolha);
    limparBuffer();
    if (escolha <= 0)
    {
        printf("Valor inválido.\n");
        fclose(arquivo);
        fclose(arquivoDestino);
        return;
    }
    
    int quantColunas = 0;
    fscanf(arquivo, "%d\n", &quantColunas);
    fprintf(arquivoDestino, "%d\n", quantColunas);

    Coluna colunaTabela[quantColunas];
    int maxTamLinha = (quantColunas + 1) * MAX_TAM_VALOR; // Como cada nome da coluna tem o seu tamanho de até MAX_TAM_VALOR, então a linha contendo todos os nomes terá no máximo = quantintade de colunas * MAX_TAM_NOME + caracteres separadores. Então a quantidade de colunas + 1 comporta toda a linha

    char linhaAtual[maxTamLinha];

    fgets(linhaAtual, maxTamLinha, arquivo);// Lê a segunda linha do arquivo que informa os tipos do dados
    removerQuebraLinha(linhaAtual);
    fprintf(arquivoDestino, "%s\n", linhaAtual);
    pegarDados(linhaAtual, colunaTabela, 1); // Separa os tipos de dados

    fgets(linhaAtual, maxTamLinha, arquivo); // Lê a terceira linha do arquivo que informa nos nomes das colunas
    removerQuebraLinha(linhaAtual);
    fprintf(arquivoDestino, "%s\n", linhaAtual);

    char linhaCopiaAtual[maxTamLinha];
    int apagouLinha = 0;

    while (fgets(linhaAtual, maxTamLinha, arquivo) != NULL) 
    {
        strcpy(linhaCopiaAtual, linhaAtual);
        removerQuebraLinha(linhaCopiaAtual);
        pegarDados(linhaAtual, colunaTabela, 3);

        if (colunaTabela[0].valorColuna.valorChaveP == escolha)
        {
            apagouLinha = 1;
        }
        else
        {
            fprintf(arquivoDestino, "%s\n", linhaCopiaAtual);
        }
    }
    if (apagouLinha == 0)
    {
        printf("Valor de chave primária inválido, nenhuma linha foi deletada.\n");
    }
    fclose(arquivo);
    fclose(arquivoDestino);

    remove(nomeTabela);
    rename("temp.itp", nomeTabela);
}
  