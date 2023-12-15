## Descrição

O projeto foi desenvolvido individualmente e contempla todas as funcionalidades solicitadas, seguindo as especificações estabelecidas.

### Funcionalidades Implementadas

- Criar tabela
- Listar todas as tabelas
- Criar nova linha na tabela
- Listar todos dados da tabela
- Pesquisar valor em uma tabela
- Apagar linha
- Apagar tabela

### Funcionalidades Não Implementadas

Não há funcionalidades solicitadas que não tenham sido implementadas neste projeto.

### Reflexão sobre o Processo

Durante o desenvolvimento deste projeto, percebi que a implementação atual, embora funcional para este contexto específico, poderia não ser tão eficiente em cenários que envolvam um grande volume de dados. A coleta de dados individualmente, como implementado aqui, pode tornar-se um gargalo significativo quando lidamos com milhares de registros. Em situações desse tipo, a abordagem de inserção de dados um a um pode ser excessivamente lenta e ineficiente.
Uma melhoria que consideraria para tornar o projeto mais escalável seria implementar métodos que permitam a inserção de dados em massa, talvez utilizando estratégias que possibilitem a inserção de dados em uma única linha ou em lotes. E assim, tornar a aplicação mais eficiente e responsiva.

## Compilação do Projeto

Este projeto foi compilado utilizando o compilador GCC (MinGW.org GCC-6.3.0-1) versão 6.3.0.

Para compilar o projeto, siga os seguintes passos:

1. Abra um terminal na pasta do projeto.
2. Execute os seguintes comandos:

gcc main.c controllers.c utils.c -o output/final -g -W
cd output
./final.exe

### Autor: Raylanna Lara Felix de Araujo








