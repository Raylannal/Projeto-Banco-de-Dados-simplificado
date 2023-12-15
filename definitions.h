#ifndef DEF
#define DEF

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

#endif