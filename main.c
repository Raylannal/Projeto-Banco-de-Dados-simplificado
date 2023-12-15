#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#include "definitions.h"
#include "controllers.h"
#include "utils.h"

int main() 
{
    FILE *arquivoTabela = NULL;
    char nomeTabela[MAX_TAM_NOME];
    int escolhaMenu = 0;
    
    while (escolhaMenu != 8)
    {
        printf("#################################################################\n");
        printf("#                           Menu                                #\n");
        printf("#################################################################\n");
        printf("#                                                               #\n");
        printf("#  1. Criar Tabela.              5. Pesquisar valor na tabela.  #\n");
        printf("#  2. Listar tabelas.            6. Apagar linha.               #\n");
        printf("#  3. Criar nova linha.          7. Apagar tabela.              #\n");
        printf("#  4. Listar dados da tabela     8. Sair                        #\n");
        printf("#                                                               #\n");
        printf("#################################################################\n");

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
                printf("Informe o nome da tabela (nome e extensão) para ser deletada: \n");
                fgets(nomeTabela, MAX_TAM_NOME, stdin);
                removerQuebraLinha(nomeTabela);

                testaNome(arquivoTabela, nomeTabela);

                apagarTabela(nomeTabela);
                break;
            default:
                break;
        }
    }
    return 0;
}
