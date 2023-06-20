#include <stdio.h>

#include <stdlib.h>

#include <string.h>

// DEFINE TOKENS
#define TKPontoEVirgula 60 // ;
#define TKVoid 61 // void
#define TKChar 62 // char
#define TKShort 63 // short
#define TKInt 64 // int
#define TKLong 65 // long
#define TKFloat 66 // float
#define TKDouble 67 // double
#define TKUnsigned 68 // unsigned
#define TKVirgula 69 // ,
#define TKAtribuicao 70 // =
#define TKMultiplicacao 71 // *
#define TKId 72 // id
#define TKAbreParenteses 73 // (
#define TKFechaParenteses 74 // )
#define TKAbreColchetes 75 // [
#define TKFechaColchetes 76 // ]
#define TKAbreChaves 77 // {
#define TKFechaChaves 78 // }
#define TKIf 79 // if
#define TKElse 80 // else
#define TKWhile 81 // while
#define TKDo 82 // do
#define TKFor 83 // for
#define TKOuLogico 84 // or_op
#define TKELogico 85 // &&
#define TKXorBitwise 86 // ^
#define TKEBitwise 87 // &
#define TKOuBitwise 88 // |
#define TKIgual 89 // ==
#define TKDiferente 90 // !=
#define TKMenor 91 // <
#define TKMaior 92 // >
#define TKMenorOuIgual 93 // <=
#define TKMaiorOuIgual 94 // >=
#define TKEsquerdaBitwise 95 // <<
#define TKDireitaBitwise 96 // >>
#define TKSoma 97 // +
#define TKSubtrai 98 // -
#define TKDivide 99 // /
#define TKResto 100 // %
#define TKIncrementa 101 // ++
#define TKDecrementa 102 // --
#define TKPonto 103 // .
#define TKPontero 104 // ->
#define TKConstante 105 // constant
#define TKString 106 // string_literal
#define TKNaoBitwise 107 // ~
#define TKNegacao 108 // !
#define TKMultiplicacaoAtribuicao 109 // *=
#define TKDivisaoAtribuicao 110 // /=
#define TKRestoAtribuicao 111 // %=
#define TKSomaAtribuicao 112 // +=
#define TKSubtraiAtribuicao 113 // -=
#define TKAtribuicaoEsquerdaBitwise 114 // <<=
#define TKAtribuicaoDireitoBitwise 115 // >>=
#define TKEAtribuicaoBitwise 116 // &=
#define TKXorAtribuicaoBitwise 117 // ^=
#define TKOuAtribuicaoBitwise 118 // or_assign
//#define TKLong 119 // long
#define TKFimDaTabela 1000
#define TKFimArquivo 1001

int linha = 1;
int coluna = 0;
int colunaInicial = 0;
int tk;
char lex[20]; //lexema gerado pela sequencia de leituras de tokens
FILE * arqin;
FILE * filePtr;

char c; //

struct palavraReservada {
    char palavra[20];
    int tk;
};

struct palavraReservada lista_pal[] = {
    {
        "void",
        TKVoid
    },
    {
        "int",
        TKInt
    },
    {
        "float",
        TKFloat
    },
    {
        "char",
        TKChar
    },
    {
        "double",
        TKDouble
    },
    {
        "do",
        TKDo
    },
    {
        "while",
        TKWhile
    },
    {
        "for",
        TKFor
    },
    {
        "if",
        TKIf
    },
    {
        "else",
        TKElse
    },
    {
        "long",
        TKLong
    },
    {
        "unsigned",
        TKUnsigned
    },
    {
        "fimtabela",
        TKFimDaTabela
    }
};

//compara o lexema gerado (token) com as palavras da lista de palavras reservadas
int palavra_reservada(char lex[]) {
    int postab = 0;
    while (strcmp("fimtabela", lista_pal[postab].palavra) != 0) {
        if (strcmp(lex, lista_pal[postab].palavra) == 0) {
            return lista_pal[postab].tk; //se a palavra for igual alguma da lista, � uma palavra reservada e retornar� o token dela
        } else postab++;
    }
    return TKId; //caso n�o seja uma palavra reservada, ser� considerada como um identificador
}

void getToken();
void proxC();

void proxC() {
    fread( & c, 1, 1, arqin);
    if (feof(arqin)) {
        tk = TKFimArquivo;
        c = -1; //se chegou no fim de arquivo
        return;
    }

    if (c == '\n') {
        //printf("CAIU NO IF ==> linha atual: %d ... coluna atual: %d ... char atual %c\n", linha, coluna, c);
        linha++;
        coluna = 0;
        colunaInicial = 1;
    } else {
        // printf("CAIU NO ELSE ==> linha atual: %d ... coluna atual: %d ... char atual %c\n", linha, coluna, c);
        coluna++;
    }
}

void getToken() {
    int estado = 0, //monitora se o char � uma letra, se for, joga pra 1 para verificar se � uma palavra reservada
        fim = 0,
        posl = 0;
    colunaInicial = coluna;

    while (!fim) {
        lex[posl++] = c;
        //printf("char atual: %c\n", c);
        switch (estado) {
        case 0:
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_') {
                proxC();
                estado = 1;
                break;
            }

            if (c >= '0' && c <= '9') {
                posl--;
                while (c >= '0' && c <= '9') {
                    lex[posl++] = c;
                    proxC();
                }
                lex[posl] = '\0';
                tk = TKInt;
                printf("Token Selecionado %d\n", tk);
                return;
            }

            if (c == '=') {
                proxC();

                if (c == '=') {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKIgual;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    proxC();
                    tk = TKAtribuicao;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '+') {
                proxC();
                if (c == '+') {
                    lex[posl++] = '+';
                    lex[posl] = '\0';
                    tk = TKIncrementa;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else if (c == '=') {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    tk = TKSomaAtribuicao;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    tk = TKSoma;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '-') {
                proxC();
                if (c == '>') {
                    lex[posl++] = '>';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKPontero;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    tk = TKDecrementa;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '>') {
                proxC();

                if (c == '>') {
                    lex[posl++] = '>';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKDireitaBitwise;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else if (c == '=') {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKMaiorOuIgual;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    tk = TKMaior;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '<') {
                proxC();

                if (c == '<') {
                    lex[posl++] = '<';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKEsquerdaBitwise;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else if (c == '=') {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKMenorOuIgual;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    tk = TKMenor;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '!') {
                proxC();
                if (c == '=') {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKDiferente;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    tk = TKNegacao;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '|') {
                proxC();

                if (c == '|') {
                    lex[posl++] = '|';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKOuLogico;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    proxC();
                    tk = TKOuBitwise;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '&') {
                proxC();
                if (c == '&') {
                    lex[posl++] = '&';
                    lex[posl] = '\0';
                    proxC();
                    tk = TKELogico;
                    printf("Token Selecionado %d\n", tk);
                    return;
                } else {
                    lex[posl] = '\0';
                    proxC();
                    tk = TKEBitwise;
                    printf("Token Selecionado %d\n", tk);
                    return;
                }
            }

            if (c == '/') {
                lex[posl] = '\0';
                proxC();
                tk = TKDivide;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '%') {
                lex[posl] = '\0';
                proxC();
                tk = TKResto;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '^') {
                lex[posl] = '\0';
                proxC();
                tk = TKXorBitwise;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '~') {
                lex[posl] = '\0';
                proxC();
                tk = TKNaoBitwise;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '*') {
                lex[posl] = '\0';
                proxC();
                tk = TKMultiplicacao;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '[') {
                lex[posl] = '\0';
                proxC();
                tk = TKAbreColchetes;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == ']') {
                lex[posl] = '\0';
                proxC();
                tk = TKFechaColchetes;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '(') {
                lex[posl] = '\0';
                proxC();
                tk = TKAbreParenteses;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == ')') {
                lex[posl] = '\0';
                proxC();
                tk = TKFechaParenteses;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '{') {
                lex[posl] = '\0';
                proxC();
                tk = TKAbreChaves;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '}') {
                lex[posl] = '\0';
                proxC();
                tk = TKFechaChaves;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == ',') {
                lex[posl] = '\0';
                proxC();
                tk = TKVirgula;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == ';') {
                lex[posl] = '\0';
                proxC();
                tk = TKPontoEVirgula;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == '.') {
                lex[posl] = '\0';
                proxC();
                tk = TKPonto;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == -1) {
                lex[posl] = '\0';
                proxC();
                tk = TKFimArquivo;
                printf("Token Selecionado %d\n", tk);
                return;
            }
            if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                proxC();
                posl--;
                colunaInicial = coluna;
                break;
            }
            if (c == '\0') {
                tk = -1;
                printf("Token Selecionado %d\n", tk);
                return;
            }

            printf("Erro lexico: encontrou o caracter %c (%d)", c, c);

            while (c != '\n') proxC();
            break;

        case 1:
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9') {
                proxC();
                break;
            }
            lex[--posl] = '\0';
            tk = palavra_reservada(lex); //joga para fun��o palavra reservada para verificar se � uma ou n�o
            printf("Token Selecionado %d\n", tk);
            return;
        } //switch
    } // while
} // funcao

int ProgramaC();

int RestoProgramaC();

int Declaracao();

int Declaracao1Linha();

int DeclaracaoSpec();

int DeclaracaoSpec1Linha();

int Tipo();

int InitDeclaracaoLista();

int InitDeclaracaoLista1Hash();

int InitDeclaracao();

int InitDeclaracao1Linha();

int Pointer();

int DeclaracaoDireto();

int RestoDeclaracaoDireto();

int RestoDeclaracaoDireto2();

int RestoDeclaracaoDireto3();

int ListaParametro();

int ListaParametro1Hash();

int DeclaracaoParametro();

int DeclaracaoParametro1Linha();

int ListaIdentificadores();

int RestoListaIdentificadores();

int Funcao();

int Funcao2Linha();

int Funcao1Linha();

int ListaDeclaracao();

int ListaDeclaracao1Hash();

int ComandoComposto();

int ComandoComposto2();

int ComandoComposto21Linha();

int ListaComando();

int ListaComando1Hash();

int Comando();

int ComandoExpressao();

int ComandoSelecao();

int ComandoSelecao1Linha();

int ComandoIterativo();

int ComandoIterativo1Linha();

int Init();

int Init1Linha();

int InitList();

int RestoInitList();

int Atribuicao();

int ExpressaoOr();

int RestoExpressaoOr();

int ExpressaoAnd();

int RestoExpressaoAnd();

int OuInclusivo();

int RestoOuInclusivo();

int OuExclusivo();

int RestoOuExclusivo();

int AndBitwise();

int RestoAndBitwise();

int ExpressaoIgual();

int RestoExpressaoIgual();

int ExpressaoRelac();

int RestoExpressaoRelac();

int ExpressaoDesl();

int RestoExpressaoDesl();

int ExpressaoAd();

int RestoExpressaoAd();

int ExpressaoMult();

int RestoExpressaoMult();

int ExpressaoUnaria();

int ExpressaoPosfix();

int RestoExpressaoPosfix();

int RestoExpressaoPosfix2();

int ExpressaoPrim();

int ArgumentList();

int RestoArgumentList();

int OperadorUnario();

int OperadorAtribuicao();

int Expressao();

int RestoExpressao();

// <*********** INICIO DO ANALISADOR SINT�TICO DESCENDENTE RECURSIVO SEM RETROCESSO ***********>

//ProgramaC -> Funcao RestoProgramaC | Declaracao RestoProgramaC
int ProgramaC() {
    printf("ENTROU ProgramaC \n");
    if (Funcao()) {
        if (RestoProgramaC()) {
            return 1;
        } else {
            return 0;
        }
    } else if (Declaracao()) {
        if (RestoProgramaC()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoProgramaC -> Funcao RestoProgramaC | Declaracao RestoProgramaC | ?
int RestoProgramaC() {
    printf("ENTROU RestoProgramaC \n");
    if (Funcao()) {
        if (RestoProgramaC()) {
            return 1;
        } else {
            return 0;
        }
    } else if (Declaracao()) {
        if (RestoProgramaC()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//Declaracao -> DeclaracaoSpec Declaracao1Linha | DeclaracaoDireto
int Declaracao() {
    printf("ENTROU Declaracao \n");
    if (DeclaracaoSpec()) {
        if (Declaracao1Linha()) {
            return 1;
        } else {
            return 0;
        }
    } else if (DeclaracaoDireto()) {
        return 1;
    } else {
        return 0;
    }
}

//Declaracao1Linha -> ; | InitDeclaracaoLista ;
int Declaracao1Linha() {
    printf("ENTROU Declaracao1Linha \n");
    if (tk == TKPontoEVirgula) { // ;
        printf("CONSUMIU TOKEN ;\n");
        getToken();
        return 1;
    } else if (InitDeclaracaoLista()) {
        if (tk == TKPontoEVirgula) { // ;
            printf("CONSUMIU TOKEN ;\n");
            getToken();
            return 1;
        }
        else {
            printf("Erro, esperava token ';'\n");
            printf("ERRO NA FUNCAO 'Declaracao1Linha'\n");
            return 0;
        }
    }
    else {
        printf("Erro, esperava token ';'\n");
        printf("ERRO NA FUNCAO 'Declaracao1Linha'\n");
        return 0;
    }
}

//DeclaracaoSpec -> Tipo DeclaracaoSpec1Linha
int DeclaracaoSpec() {
    printf("ENTROU DeclaracaoSpec \n");
    if (Tipo()) {
        return 1;
    } else {
        return 0;
    }
}

//Tipo -> void | char | short | int | long | float | double | unsigned
int Tipo() {
    printf("ENTROU Tipo \n");
    if (tk == TKVoid) { // void
        printf("CONSUMIU TOKEN void\n");
        getToken();
        return 1;
    } else if (tk == TKChar) { // char
        printf("CONSUMIU TOKEN char\n");
        getToken();
        return 1;
    } else if (tk == TKShort) { // short
        printf("CONSUMIU TOKEN short\n");
        getToken();
        return 1;
    } else if (tk == TKInt) { // int
        printf("CONSUMIU TOKEN int\n");
        getToken();
        return 1;
    } else if (tk == TKLong) { // long
        printf("CONSUMIU TOKEN long\n");
        getToken();
        return 1;
    } else if (tk == TKFloat) { // float
        printf("CONSUMIU TOKEN float\n");
        getToken();
        return 1;
    } else if (tk == TKDouble) { // double
        printf("CONSUMIU TOKEN double\n");
        getToken();
        return 1;
    } else if (tk == TKUnsigned) { // unsigned
        printf("CONSUMIU TOKEN unsigned\n");
        getToken();
        return 1;
    } else {
        printf("Erro, esperava token de inicializacao (void, char, short, int, long, float, double, unsigned...\n");
        printf("ERRO NA FUNCAO 'TIPO'\n");
        return 0;
    }
}

//InitDeclaracaoLista -> InitDeclaracao InitDeclaracaoLista1Hash
int InitDeclaracaoLista() {
    printf("ENTROU InitDeclaracaoLista \n");
    if (InitDeclaracao()) {
        if (InitDeclaracaoLista1Hash()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//InitDeclaracaoLista1Hash -> , InitDeclaracao InitDeclaracaoLista1Hash | ?
int InitDeclaracaoLista1Hash() {
    printf("ENTROU InitDeclaracaoLista1Hash \n");
    if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (InitDeclaracao()) {
            if (InitDeclaracaoLista1Hash()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO InitDeclaracaoLista1Hash esperava ',' e derivou vazio\n");
        return 1;
    }
}

//InitDeclaracao -> Declaracao InitDeclaracao1Linha
int InitDeclaracao() {
    printf("ENTROU InitDeclaracao \n");
    if (Declaracao()) {
        if (InitDeclaracao1Linha()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//InitDeclaracao1Linha -> = Init | ?
int InitDeclaracao1Linha() {
    printf("ENTROU InitDeclaracao1Linha \n");
    if (tk == TKAtribuicao) { // =
        printf("CONSUMIU TOKEN =\n");
        getToken();
        if (Init()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO InitDeclaracao1Linha esperava '=' e derivou vazio\n");
        return 1;
    }
}

//Pointer -> * Pointer | ?
int Pointer() {
    printf("ENTROU Pointer \n");
    if (tk == TKMultiplicacao) { // *
        printf("CONSUMIU TOKEN *\n");
        getToken();
        if (Pointer()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO Pointer esperava '*' e derivou vazio\n");
        return 1;
    }
}

//DeclaracaoDireto -> id RestoDeclaracaoDireto | ( Declaracao ) RestoDeclaracaoDireto
int DeclaracaoDireto() {
    printf("ENTROU DeclaracaoDireto \n");
    if (tk == TKId) { // id
        printf("CONSUMIU TOKEN id\n");
        getToken();
        if (RestoDeclaracaoDireto()) {
            return 1;
        } else {
            return 0;
        }
    } else if (tk == TKAbreParenteses) { // (
        printf("CONSUMIU TOKEN (\n");
        getToken();
        if (Declaracao()) {
            if (tk == TKFechaParenteses) { // )
                printf("CONSUMIU TOKEN )\n");
                getToken();
                if (RestoDeclaracaoDireto()) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                printf("ERRO NA FUNCAO 'DeclaracaoDireto'\n");
                printf("Erro, esperava token ')'\n");
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'DeclaracaoDireto'\n");
        printf("Erro, esperava token 'TKId ou ('\n");
        return 0;
    }
}

//RestoDeclaracaoDireto -> [ RestoDeclaracaoDireto2 | ( RestoDeclaracaoDireto3
int RestoDeclaracaoDireto() {
    printf("ENTROU RestoDeclaracaoDireto \n");
    if (tk == TKAbreColchetes) { // [
        printf("CONSUMIU TOKEN [\n");
        getToken();
        if (RestoDeclaracaoDireto2()) {
            return 1;
        } else {
            return 0;
        }
    } else if (tk == TKAbreParenteses) { // (
        printf("CONSUMIU TOKEN (\n");
        getToken();
        if (RestoDeclaracaoDireto3()) {
            return 1;
        } else {
            return 0;
        }
    } else if (tk == TKPontoEVirgula) { // ;
        printf("CONSUMIU TOKEN ;\n");
        getToken();
        return 1;
    }
	else {
        printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto'\n");
        printf("Erro, esperava token '[ ou ('\n");
        return 0;
    }
}

//RestoDeclaracaoDireto2 -> ExpressaoOr ] RestoDeclaracaoDireto | ] RestoDeclaracaoDireto
int RestoDeclaracaoDireto2() {
    printf("ENTROU RestoDeclaracaoDireto2 \n");
    if (ExpressaoOr()) {
        if (tk == TKFechaColchetes) { // ]
            printf("CONSUMIU TOKEN ]\n");
            getToken();
            if (RestoDeclaracaoDireto()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto2'\n");
            printf("Erro, esperava token ']'\n");
            return 0;
        }
    } else if (tk == TKFechaColchetes) { // ]
        printf("CONSUMIU TOKEN ]\n");
        getToken();
        if (RestoDeclaracaoDireto()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto2'\n");
        printf("Erro, esperava token ']'\n");
        return 0;
    }
}

//RestoDeclaracaoDireto3 -> ListaParametro ) RestoDeclaracaoDireto | ListaIdentificadores ) RestoDeclaracaoDireto | ) RestoDeclaracaoDireto
int RestoDeclaracaoDireto3() {
    printf("ENTROU RestoDeclaracaoDireto3 \n");
    if (ListaParametro()) {
        if (tk == TKFechaParenteses) { // )
            printf("CONSUMIU TOKEN )\n");
            getToken();
            if (RestoDeclaracaoDireto()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto3'\n");
            printf("Erro, esperava token ')'\n");
            return 0;
        }
    } else if (ListaIdentificadores()) {
        if (tk == TKFechaParenteses) { // )
            printf("CONSUMIU TOKEN )\n");
            getToken();
            if (RestoDeclaracaoDireto()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto3'\n");
            printf("Erro, esperava token ')'\n");
            return 0;
        }
    } else if (tk == TKFechaParenteses) { // )
        printf("CONSUMIU TOKEN )\n");
        getToken();
        return 1;
    } else {
        printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto3'\n");
        printf("Erro, esperava token ')'\n");
        return 0;
    }
}

//ListaParametro -> DeclaracaoParametro ListaParametro1Hash
int ListaParametro() {
    printf("ENTROU ListaParametro \n");
    if (DeclaracaoParametro()) {
        if (ListaParametro1Hash()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ListaParametro1Hash -> , DeclaracaoParametro ListaParametro1Hash | ?
int ListaParametro1Hash() {
    printf("ENTROU ListaParametro1Hash \n");
    if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (DeclaracaoParametro()) {
            if (ListaParametro1Hash()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO ListaParametro1Hash esperava ',' e derivou vazio\n");
        return 1;
    }
}

//DeclaracaoParametro -> DeclaracaoSpec DeclaracaoParametro1Linha
int DeclaracaoParametro() {
    printf("ENTROU DeclaracaoParametro \n");
    if (DeclaracaoSpec()) {
        if (DeclaracaoParametro1Linha()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//DeclaracaoParametro1Linha -> Declaracao | ?
int DeclaracaoParametro1Linha() {
    printf("ENTROU DeclaracaoParametro1Linha \n");
    if (Declaracao()) {
        return 1;
    } else {
        return 1;
    }
}

//ListaIdentificadores -> id RestoListaIdentificadores
int ListaIdentificadores() {
    printf("ENTROU ListaIdentificadores \n");
    if (tk == TKId) { // id
        printf("CONSUMIU TOKEN id\n");
        getToken();
        if (RestoListaIdentificadores()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'ListaIdentificadores'\n");
        printf("Erro, esperava token 'TKid'\n");
        return 0;
    }
}

//RestoListaIdentificadores -> , id RestoListaIdentificadores | ?
int RestoListaIdentificadores() {
    printf("ENTROU RestoListaIdentificadores \n");
    if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (tk == TKId) { // id
            printf("CONSUMIU TOKEN id\n");
            getToken();
            if (RestoListaIdentificadores()) {
                return 1;
            } else {
                printf("ERRO NA FUNCAO 'RestoListaIdentificadores'\n");
                printf("Erro, esperava token 'TKid'\n");
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoListaIdentificadores esperava ',' e derivou vazio\n");
        return 1;
    }
}

//Funcao -> DeclaracaoSpec Declaracao Funcao1Linha | Declaracao Funcao2Linha
int Funcao() {
    printf("ENTROU Funcao \n");
    if (DeclaracaoSpec()) {
        if (Declaracao()) {
            if (Funcao1Linha()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (Declaracao()) {
        if (Funcao2Linha()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//Funcao2Linha -> ListaDeclaracao ComandoComposto | ComandoComposto
int Funcao2Linha() {
    printf("ENTROU Funcao2Linha \n");
    if (ListaDeclaracao()) {
        if (ComandoComposto()) {
            return 1;
        } else {
            return 0;
        }
    } else if (ComandoComposto()) {
        return 1;
    } else {
        return 0;
    }
}

//Funcao1Linha -> ListaDeclaracao ComandoComposto | ComandoComposto
int Funcao1Linha() {
    printf("ENTROU Funcao1Linha \n");
    if (ListaDeclaracao()) {
        if (ComandoComposto()) {
            return 1;
        } else {
            return 0;
        }
    } else if (ComandoComposto()) {
        return 1;
    } else {
        return 0;
    }
}

//ListaDeclaracao -> Declaracao ListaDeclaracao1Hash
int ListaDeclaracao() {
    printf("ENTROU ListaDeclaracao \n");
    if (Declaracao()) {
        if (ListaDeclaracao1Hash()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ListaDeclaracao1Hash -> Declaracao ListaDeclaracao1Hash | ?
int ListaDeclaracao1Hash() {
    printf("ENTROU ListaDeclaracao1Hash \n");
    if (Declaracao()) {
        if (ListaDeclaracao1Hash()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//ComandoComposto -> { ComandoComposto2
int ComandoComposto() {
    printf("ENTROU ComandoComposto \n");
    if (tk == TKAbreChaves) { // {
        printf("CONSUMIU TOKEN {\n");
        getToken();
        if (ComandoComposto2()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'ComandoComposto'\n");
        printf("Erro, esperava token '{'\n");
        return 0;
    }
}

//ComandoComposto2 -> } | ListaComando } | ListaDeclaracao ComandoComposto21Linha
int ComandoComposto2() {
    printf("ENTROU ComandoComposto2 \n");
    if (tk == TKFechaChaves) { // }
        printf("CONSUMIU TOKEN }\n");
        getToken();
        return 1;
    } else if (ListaComando()) {
        if (tk == TKFechaChaves) { // }
            printf("CONSUMIU TOKEN }\n");
            getToken();
            return 1;
        }
        else {
            printf("ERRO NA FUNCAO 'ComandoComposto2'\n");
            printf("Erro, esperava token '}'\n");
            return 0;
        }
    }
    else if (ListaDeclaracao()) {
        if (ComandoComposto21Linha()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'ComandoComposto2'\n");
        printf("Erro, esperava token '}'\n");
        return 0;
    }
}

//ComandoComposto21Linha -> } | ListaComando }
int ComandoComposto21Linha() {
    printf("ENTROU ComandoComposto21Linha \n");
    if (tk == TKFechaChaves) { // }
        printf("CONSUMIU TOKEN }\n");
        getToken();
        return 1;
    } else if (ListaComando()) {
        if (tk == TKFechaChaves) { // }
            printf("CONSUMIU TOKEN }\n");
            getToken();
            return 1;
        }
        else {
            printf("ERRO NA FUNCAO 'ComandoComposto21Linha'\n");
            printf("Erro, esperava token '}'\n");
            return 0;
        }
    }
    else {
        printf("ERRO NA FUNCAO 'ComandoComposto21Linha'\n");
        printf("Erro, esperava token '}'\n");
        return 0;
    }
}

//ListaComando -> Comando ListaComando1Hash
int ListaComando() {
    printf("ENTROU ListaComando \n");
    if (Comando()) {
        if (ListaComando1Hash()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ListaComando1Hash -> Comando ListaComando1Hash | ?
int ListaComando1Hash() {
    printf("ENTROU ListaComando1Hash \n");
    if (Comando()) {
        if (ListaComando1Hash()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//Comando -> ComandoComposto | ComandoExpressao | ComandoSelecao | ComandoIterativo
int Comando() {
    printf("ENTROU Comando \n");
    if (ComandoComposto()) {
        return 1;
    } else if (ComandoExpressao()) {
        return 1;
    } else if (ComandoSelecao()) {
        return 1;
    } else if (ComandoIterativo()) {
        return 1;
    } else {
        return 0;
    }
}

//ComandoExpressao -> ; | Expressao ;
int ComandoExpressao() {
    printf("ENTROU ComandoExpressao \n");
    if (tk == TKPontoEVirgula) { // ;
        printf("CONSUMIU TOKEN ;\n");
        getToken();
        return 1;
    } else if (Expressao()) {
        if (tk == TKPontoEVirgula) { // ;
            printf("CONSUMIU TOKEN ;\n");
            getToken();
            return 1;
        }
        else {
            printf("ERRO NA FUNCAO 'ComandoExpressao'\n");
            printf("Erro, esperava token ';'\n");
            return 0;
        }
    }
    else {
        printf("ERRO NA FUNCAO 'ComandoExpressao'\n");
        printf("Erro, esperava token ';'\n");
        return 0;
    }
}

//ComandoSelecao -> if ( Expressao ) Comando ComandoSelecao1Linha
int ComandoSelecao() {
    printf("ENTROU ComandoSelecao \n");
    if (tk == TKIf) { // if
        printf("CONSUMIU TOKEN if\n");
        getToken();
        if (tk == TKAbreParenteses) { // (
            printf("CONSUMIU TOKEN (\n");
            getToken();
            if (Expressao()) {
                if (tk == TKFechaParenteses) { // )
                    printf("CONSUMIU TOKEN )\n");
                    getToken();
                    if (Comando()) {
                        if (ComandoSelecao1Linha()) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        return 0;
                    }
                } else {
                    printf("ERRO NA FUNCAO 'ComandoSelecao'\n");
                    printf("Erro, esperava token ')'\n");
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            printf("ERRO NA FUNCAO 'ComandoSelecao'\n");
            printf("Erro, esperava token '('\n");
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'ComandoSelecao'\n");
        printf("Erro, esperava token 'TKif'\n");
        return 0;
    }
}

//ComandoSelecao1Linha -> else Comando | ?
int ComandoSelecao1Linha() {
    printf("ENTROU ComandoSelecao1Linha \n");
    if (tk == TKElse) { // else
        printf("CONSUMIU TOKEN else\n");
        getToken();
        if (Comando()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO ComandoSelecao1Linha esperava 'else' e derivou vazio\n");
        return 1;
    }
}

//ComandoIterativo -> while ( Expressao ) Comando | do Comando while ( Expressao ) ; | for ( ComandoExpressao ComandoExpressao ComandoIterativo1Linha
int ComandoIterativo() {
    printf("ENTROU ComandoIterativo \n");
    if (tk == TKWhile) { // while
        printf("CONSUMIU TOKEN while\n");
        getToken();
        if (tk == TKAbreParenteses) { // (
            printf("CONSUMIU TOKEN (\n");
            getToken();
            if (Expressao()) {
                if (tk == TKFechaParenteses) { // )
                    printf("CONSUMIU TOKEN )\n");
                    getToken();
                    if (Comando()) {
                        return 1;
                    } else {
                        return 0;
                    }
                } else {
                    printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                    printf("Erro, esperava token ')'\n");
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
            printf("Erro, esperava token ')'\n");
            return 0;
        }
    } else if (tk == TKDo) { // do
        printf("CONSUMIU TOKEN do\n");
        getToken();
        if (Comando()) {
            if (tk == TKWhile) { // while
                printf("CONSUMIU TOKEN while\n");
                getToken();
                if (tk == TKAbreParenteses) { // (
                    printf("CONSUMIU TOKEN (\n");
                    getToken();
                    if (Expressao()) {
                        if (tk == TKFechaParenteses) { // )
                            printf("CONSUMIU TOKEN )\n");
                            getToken();
                            if (tk == TKPontoEVirgula) { // ;
                                printf("CONSUMIU TOKEN ;\n");
                                getToken();
                                return 1;
                            }
                            else {
                                printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                                printf("Erro, esperava token ';'\n");
                                return 0;
                            }
                        }
                        else {
                            printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                            printf("Erro, esperava token ')'\n");
                            return 0;
                        }
                    }
                    else {
                        return 0;
                    }
                }
                else {
                    printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                    printf("Erro, esperava token '('\n");
                    return 0;
                }
            }
            else {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    else if (tk == TKFor) { // for
        printf("CONSUMIU TOKEN for\n");
        getToken();
        if (tk == TKAbreParenteses) { // (
            printf("CONSUMIU TOKEN (\n");
            getToken();
            if (ComandoExpressao()) {
                if (ComandoExpressao()) {
                    if (ComandoIterativo1Linha()) {
                        return 1;
                    } else {
                        return 0;
                    }
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
            printf("Erro, esperava token ')'\n");
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
        printf("Erro, esperava token 'do' ou 'while'\n");
        return 0;
    }
}

//ComandoIterativo1Linha -> ) Comando | Expressao ) Comando
int ComandoIterativo1Linha() {
    printf("ENTROU ComandoIterativo1Linha \n");
    if (tk == TKFechaParenteses) { // )
        printf("CONSUMIU TOKEN )\n");
        getToken();
        if (Comando()) {
            return 1;
        } else {
            return 0;
        }
    } else if (Expressao()) {
        if (tk == TKFechaParenteses) { // )
            printf("CONSUMIU TOKEN )\n");
            getToken();
            if (Comando()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'ComandoIterativo1Linha'\n");
        printf("Erro, esperava token ')'\n");
        return 0;
    }
}

//Init -> Atribuicao | { InitList Init1Linha
int Init() {
    printf("ENTROU Init \n");
    if (Atribuicao()) {
        return 1;
    } else if (tk == TKAbreChaves) { // {
        printf("CONSUMIU TOKEN {\n");
        getToken();
        if (InitList()) {
            if (Init1Linha()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("ERRO NA FUNCAO 'Init'\n");
        printf("Erro, esperava token '{'\n");
        return 0;
    }
}

//Init1Linha -> } | , }
int Init1Linha() {
    printf("ENTROU Init1Linha \n");
    if (tk == TKFechaChaves) { // }
        printf("CONSUMIU TOKEN }\n");
        getToken();
        return 1;
    } else if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (tk == TKFechaChaves) { // }
            printf("CONSUMIU TOKEN }\n");
            getToken();
            return 1;
        }
        else {
            printf("ERRO NA FUNCAO 'Init1Linha'\n");
            printf("Erro, esperava token '}'\n");
            return 0;
        }
    }
    else {
        printf("ERRO NA FUNCAO 'Init1Linha'\n");
        printf("Erro, esperava token '}' ou ','\n");
        return 0;
    }
}

//InitList -> Init | RestoInitList
int InitList() {
    printf("ENTROU InitList \n");
    if (Init()) {
        return 1;
    } else if (RestoInitList()) {
        return 1;
    } else {
        return 0;
    }
}

//RestoInitList -> , Init RestoInitList | ?
int RestoInitList() {
    printf("ENTROU RestoInitList \n");
    if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (Init()) {
            if (RestoInitList()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoInitList esperava ',' e derivou vazio\n");
        return 1;
    }
}

//Atribuicao -> ExpressaoOr | ExpressaoUnaria OperadorAtribuicao Atribuicao
int Atribuicao() {
    printf("ENTROU Atribuicao \n");
    if (ExpressaoOr()) {
        return 1;
    } else if (ExpressaoUnaria()) {
        if (OperadorAtribuicao()) {
            if (Atribuicao()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ExpressaoOr -> ExpressaoAnd RestoExpressaoOr
int ExpressaoOr() {
    printf("ENTROU ExpressaoOr \n");
    if (ExpressaoAnd()) {
        if (RestoExpressaoOr()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoOr -> or_op ExpressaoAnd RestoExpressaoOr | ?
int RestoExpressaoOr() {
    printf("ENTROU RestoExpressaoOr \n");
    if (tk == TKOuLogico) { // or_op
        printf("CONSUMIU TOKEN or_op\n");
        getToken();
        if (ExpressaoAnd()) {
            if (RestoExpressaoOr()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoExpressaoOr esperava 'TKOuLogico' e derivou vazio\n");
        return 1;
    }
}

//ExpressaoAnd -> OuInclusivo RestoExpressaoAnd
int ExpressaoAnd() {
    printf("ENTROU ExpressaoAnd \n");
    if (OuInclusivo()) {
        if (RestoExpressaoAnd()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoAnd -> && OuInclusivo RestoExpressaoAnd | ?
int RestoExpressaoAnd() {
    printf("ENTROU RestoExpressaoAnd \n");
    if (tk == TKELogico) { // &&
        printf("CONSUMIU TOKEN &&\n");
        getToken();
        if (OuInclusivo()) {
            if (RestoExpressaoAnd()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoExpressaoAnd esperava 'TKOuLogico' e derivou vazio\n");
        return 1;
    }
}

//OuInclusivo -> OuExclusivo RestoOuInclusivo
int OuInclusivo() {
    printf("ENTROU OuInclusivo \n");
    if (OuExclusivo()) {
        if (RestoOuInclusivo()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoOuInclusivo -> PIPE OuExclusivo RestoOuInclusivo | ?
int RestoOuInclusivo() {
    printf("ENTROU RestoOuInclusivo \n");
    if (tk == TKOuBitwise) {
        printf("CONSUMIU TOKEN ouBitWise\n");
        getToken();
        if (OuExclusivo()) {
            if (RestoOuInclusivo()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoOuInclusivo esperava 'TKOuBitwise' e derivou vazio\n");
        return 1;
    }
}

//OuExclusivo -> AndBitwise RestoOuExclusivo
int OuExclusivo() {
    printf("ENTROU OuExclusivo \n");
    if (AndBitwise()) {
        if (RestoOuExclusivo()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoOuExclusivo -> ^ AndBitwise RestoOuExclusivo | ?
int RestoOuExclusivo() {
    printf("ENTROU RestoOuExclusivo \n");
    if (tk == TKXorBitwise) { // ^
        printf("CONSUMIU TOKEN ^\n");
        getToken();
        if (AndBitwise()) {
            if (RestoOuExclusivo()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoOuExclusivo esperava 'TKXorBitwise' e derivou vazio\n");
        return 1;
    }
}

//AndBitwise -> ExpressaoIgual RestoAndBitwise
int AndBitwise() {
    printf("ENTROU AndBitwise \n");
    if (ExpressaoIgual()) {
        if (RestoAndBitwise()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoAndBitwise -> & ExpressaoIgual RestoAndBitwise | ?
int RestoAndBitwise() {
    printf("ENTROU RestoAndBitwise \n");
    if (tk == TKEBitwise) { // &
        printf("CONSUMIU TOKEN &\n");
        getToken();
        if (ExpressaoIgual()) {
            if (RestoAndBitwise()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//ExpressaoIgual -> ExpressaoRelac RestoExpressaoIgual
int ExpressaoIgual() {
    printf("ENTROU ExpressaoIgual \n");
    if (ExpressaoRelac()) {
        if (RestoExpressaoIgual()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoIgual -> == ExpressaoRelac RestoExpressaoIgual | != ExpressaoRelac RestoExpressaoIgual | ?
int RestoExpressaoIgual() {
    printf("ENTROU RestoExpressaoIgual \n");
    if (tk == TKIgual) { // ==
        printf("CONSUMIU TOKEN ==\n");
        getToken();
        if (ExpressaoRelac()) {
            if (RestoExpressaoIgual()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKDiferente) { // !=
        printf("CONSUMIU TOKEN !=\n");
        getToken();
        if (ExpressaoRelac()) {
            if (RestoExpressaoIgual()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        printf("WARNING NA FUNCAO RestoExpressaoIgual esperava '= ou ==' e derivou vazio");
        return 1;
    }
}

//ExpressaoRelac -> ExpressaoDesl RestoExpressaoRelac
int ExpressaoRelac() {
    printf("ENTROU ExpressaoRelac \n");
    if (ExpressaoDesl()) {
        if (RestoExpressaoRelac()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoRelac -> < ExpressaoDesl RestoExpressaoRelac | > ExpressaoDesl RestoExpressaoRelac | <= ExpressaoDesl RestoExpressaoRelac | >= ExpressaoDesl RestoExpressaoRelac | ?
int RestoExpressaoRelac() {
    printf("ENTROU RestoExpressaoRelac \n");
    if (tk == TKMenor) { // <
        printf("CONSUMIU TOKEN <\n");
        getToken();
        if (ExpressaoDesl()) {
            if (RestoExpressaoRelac()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKMaior) { // >
        printf("CONSUMIU TOKEN >\n");
        getToken();
        if (ExpressaoDesl()) {
            if (RestoExpressaoRelac()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKMenorOuIgual) { // <=
        printf("CONSUMIU TOKEN <=\n");
        getToken();
        if (ExpressaoDesl()) {
            if (RestoExpressaoRelac()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKMaiorOuIgual) { // >=
        printf("CONSUMIU TOKEN >=\n");
        getToken();
        if (ExpressaoDesl()) {
            if (RestoExpressaoRelac()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//ExpressaoDesl -> ExpressaoAd RestoExpressaoDesl
int ExpressaoDesl() {
    printf("ENTROU ExpressaoDesl \n");
    if (ExpressaoAd()) {
        if (RestoExpressaoDesl()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoDesl -> << ExpressaoAd RestoExpressaoDesl | >> ExpressaoAd RestoExpressaoDesl
int RestoExpressaoDesl() {
    printf("ENTROU RestoExpressaoDesl \n");
    if (tk == TKEsquerdaBitwise) { // <<
        printf("CONSUMIU TOKEN <<\n");
        getToken();
        if (ExpressaoAd()) {
            if (RestoExpressaoDesl()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKDireitaBitwise) { // >>
        printf("CONSUMIU TOKEN >>\n");
        getToken();
        if (ExpressaoAd()) {
            if (RestoExpressaoDesl()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ExpressaoAd -> ExpressaoMult RestoExpressaoAd
int ExpressaoAd() {
    printf("ENTROU ExpressaoAd \n");
    if (ExpressaoMult()) {
        if (RestoExpressaoAd()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoAd -> + ExpressaoMult RestoExpressaoAd | - ExpressaoMult RestoExpressaoAd | ?
int RestoExpressaoAd() {
    printf("ENTROU RestoExpressaoAd \n");
    if (tk == TKSoma) { // +
        printf("CONSUMIU TOKEN +\n");
        getToken();
        if (ExpressaoMult()) {
            if (RestoExpressaoAd()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKSubtrai) { // -
        printf("CONSUMIU TOKEN -\n");
        getToken();
        if (ExpressaoMult()) {
            if (RestoExpressaoAd()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//ExpressaoMult -> ExpressaoUnaria RestoExpressaoMult
int ExpressaoMult() {
    printf("ENTROU ExpressaoMult \n");
    if (ExpressaoUnaria()) {
        if (RestoExpressaoMult()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoMult -> * ExpressaoUnaria RestoExpressaoMult | / ExpressaoUnaria RestoExpressaoMult | % ExpressaoUnaria RestoExpressaoMult | ?
int RestoExpressaoMult() {
    printf("ENTROU RestoExpressaoMult \n");
    if (tk == TKMultiplicacao) { // *
        printf("CONSUMIU TOKEN *\n");
        getToken();
        if (ExpressaoUnaria()) {
            if (RestoExpressaoMult()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKDivide) { // /
        printf("CONSUMIU TOKEN /\n");
        getToken();
        if (ExpressaoUnaria()) {
            if (RestoExpressaoMult()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKResto) { // %
        printf("CONSUMIU TOKEN %\n");
        getToken();
        if (ExpressaoUnaria()) {
            if (RestoExpressaoMult()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//ExpressaoUnaria -> ExpressaoPosfix | ++ ExpressaoUnaria | -- ExpressaoUnaria | OperadorUnario ExpressaoUnaria
int ExpressaoUnaria() {
    printf("ENTROU ExpressaoUnaria \n");
    if (ExpressaoPosfix()) {
        return 1;
    } else if (tk == TKIncrementa) { // ++
        printf("CONSUMIU TOKEN ++\n");
        getToken();
        if (ExpressaoUnaria()) {
            return 1;
        } else {
            return 0;
        }
    } else if (tk == TKDecrementa) { // --
        printf("CONSUMIU TOKEN --\n");
        getToken();
        if (ExpressaoUnaria()) {
            return 1;
        } else {
            return 0;
        }
    } else if (OperadorUnario()) {
        if (ExpressaoUnaria()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ExpressaoPosfix -> ExpressaoPrim RestoExpressaoPosfix
int ExpressaoPosfix() {
    printf("ENTROU ExpressaoPosfix \n");
    if (ExpressaoPrim()) {
        if (RestoExpressaoPosfix()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressaoPosfix -> [ Expressao ] RestoExpressaoPosfix | ( RestoExpressaoPosfix2 | . id RestoExpressaoPosfix | -> id RestoExpressaoPosfix | ++ RestoExpressaoPosfix | -- RestoExpressaoPosfix | ?
int RestoExpressaoPosfix() {
    printf("ENTROU RestoExpressaoPosfix \n");
    if (tk == TKAbreColchetes) { // [
        printf("CONSUMIU TOKEN [\n");
        getToken();
        if (Expressao()) {
            if (tk == TKFechaColchetes) { // ]
                printf("CONSUMIU TOKEN ]\n");
                getToken();
                if (RestoExpressaoPosfix()) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKAbreParenteses) { // (
        printf("CONSUMIU TOKEN (\n");
        getToken();
        if (RestoExpressaoPosfix2()) {
            return 1;
        } else {
            return 0;
        }
    } else if (tk == TKPonto) { // .
        printf("CONSUMIU TOKEN .\n");
        getToken();
        if (tk == TKId) { // id
            printf("CONSUMIU TOKEN id\n");
            getToken();
            if (RestoExpressaoPosfix()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKPontero) { // ->
        printf("CONSUMIU TOKEN ->\n");
        getToken();
        if (tk == TKId) { // id
            getToken();
            if (RestoExpressaoPosfix()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKIncrementa) { // ++
        printf("CONSUMIU TOKEN ++\n");
        getToken();
        if (RestoExpressaoPosfix()) {
            return 1;
        } else {
            return 0;
        }
    } else if (tk == TKDecrementa) { // --
        printf("CONSUMIU TOKEN --\n");
        getToken();
        if (RestoExpressaoPosfix()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

//RestoExpressaoPosfix2 -> ArgumentList ) RestoExpressaoPosfix | ) RestoExpressaoPosfix
int RestoExpressaoPosfix2() {
    printf("ENTROU RestoExpressaoPosfix2 \n");
    if (ArgumentList()) {
        if (tk == TKFechaParenteses) { // )
            printf("CONSUMIU TOKEN )\n");
            getToken();
            if (RestoExpressaoPosfix()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (tk == TKFechaParenteses) { // )
        printf("CONSUMIU TOKEN )\n");
        getToken();
        if (RestoExpressaoPosfix()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//ExpressaoPrim -> id | constant | string_literal | ( Expressao )
int ExpressaoPrim() {
    printf("ENTROU ExpressaoPrim \n");
    if (tk == TKId) { // id
        printf("CONSUMIU TOKEN id\n");
        getToken();
        return 1;
    } else if (tk == TKConstante) { // constant
        printf("CONSUMIU TOKEN constant\n");
        getToken();
        return 1;
    } else if (tk == TKString) { // string_literal
        printf("CONSUMIU TOKEN string_literal\n");
        getToken();
        return 1;
    } else if (tk == TKAbreParenteses) { // (
        printf("CONSUMIU TOKEN (\n");
        getToken();
        if (Expressao()) {
            if (tk == TKFechaParenteses) { // )
                printf("CONSUMIU TOKEN )\n");
                getToken();
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

//ArgumentList -> Atribuicao RestoArgumentList
int ArgumentList() {
    printf("ENTROU ArgumentList \n");
    if (Atribuicao()) {
        if (RestoArgumentList()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoArgumentList -> , Atribuicao RestoArgumentList
int RestoArgumentList() {
    printf("ENTROU RestoArgumentList \n");
    if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (Atribuicao()) {
            if (RestoArgumentList()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//OperadorUnario -> & | * | + | - | ~ | !
int OperadorUnario() {
    printf("ENTROU OperadorUnario \n");
    if (tk == TKEBitwise) { // &
        printf("CONSUMIU TOKEN &\n");
        getToken();
        return 1;
    } else if (tk == TKMultiplicacao) { // *
        printf("CONSUMIU TOKEN *\n");
        getToken();
        return 1;
    } else if (tk == TKSoma) { // +
        printf("CONSUMIU TOKEN +\n");
        getToken();
        return 1;
    } else if (tk == TKSubtrai) { // -
        printf("CONSUMIU TOKEN -\n");
        getToken();
        return 1;
    } else if (tk == TKNaoBitwise) { // ~
        printf("CONSUMIU TOKEN ~\n");
        getToken();
        return 1;
    } else if (tk == TKNegacao) { // !
        printf("CONSUMIU TOKEN !\n");
        getToken();
        return 1;
    } else {
        return 0;
    }
}

//OperadorAtribuicao -> = | *= | /= | %= | += | -= | <<= | >>= | &= | ^= | or_assign
int OperadorAtribuicao() {
    printf("ENTROU OperadorAtribuicao \n");
    if (tk == TKAtribuicao) { // =
        printf("CONSUMIU TOKEN =\n");
        getToken();
        return 1;
    } else if (tk == TKMultiplicacaoAtribuicao) { // *=
        printf("CONSUMIU TOKEN *=\n");
        getToken();
        return 1;
    } else if (tk == TKDivisaoAtribuicao) { // /=
        printf("CONSUMIU TOKEN /=\n");
        getToken();
        return 1;
    } else if (tk == TKRestoAtribuicao) { // %=
        printf("CONSUMIU TOKEN %=\n");
        getToken();
        return 1;
    } else if (tk == TKSomaAtribuicao) { // +=
        printf("CONSUMIU TOKEN +=\n");
        getToken();
        return 1;
    } else if (tk == TKSubtraiAtribuicao) { // -=
        printf("CONSUMIU TOKEN -=\n");
        getToken();
        return 1;
    } else if (tk == TKAtribuicaoEsquerdaBitwise) { // <<=
        printf("CONSUMIU TOKEN <<=\n");
        getToken();
        return 1;
    } else if (tk == TKAtribuicaoDireitoBitwise) { // >>=
        printf("CONSUMIU TOKEN >>=\n");
        getToken();
        return 1;
    } else if (tk == TKEAtribuicaoBitwise) { // &=
        printf("CONSUMIU TOKEN &=\n");
        getToken();
        return 1;
    } else if (tk == TKXorAtribuicaoBitwise) { // ^=
        printf("CONSUMIU TOKEN ^=\n");
        getToken();
        return 1;
    } else if (tk == TKOuAtribuicaoBitwise) { // or_assign
        printf("CONSUMIU TOKEN or_assign\n");
        getToken();
        return 1;
    } else {
        return 0;
    }
}

//Expressao -> Atribuicao RestoExpressao
int Expressao() {
    printf("ENTROU Expressao \n");
    if (Atribuicao()) {
        if (RestoExpressao()) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//RestoExpressao -> , Atribuicao RestoExpressao | ?
int RestoExpressao() {
    printf("ENTROU RestoExpressao \n");
    if (tk == TKVirgula) { // ,
        printf("CONSUMIU TOKEN ,\n");
        getToken();
        if (Atribuicao()) {
            if (RestoExpressao()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

int main() {
    arqin = fopen("C:\\Users\\filip\\Documents\\Formais\\TDE4\\codigoTeste.txt", "rb");

    if (!arqin) {
        printf("Erro na abertura do fonte.\n");
        return 0;
    }

    proxC(); // le primeiro caracter do arquivo
    getToken(); // le primeiro token

    while (tk != TKFimArquivo) {
        //struct palavraReservada token_encontrado = buscar_token(tk);
        printf("token encontrado: %s\n", lex);
        getToken();

    }

    printf("Passou no lexico!!!\nAgora Vamos testar o sintatico\n");
    printf("*********************************\n"
        "*********************************\n"
        "*********************************\n"
        "*********************************\n\n");

    arqin = fopen("C:\\Users\\filip\\Documents\\Formais\\TDE4\\codigoTeste.txt", "rb");

    proxC(); // le primeiro caracter do arquivo
    getToken(); // le primeiro token
    if (ProgramaC()) {
        printf("Passou no sintatico!\n");
    } else printf("Erro sintatico!\n");
    return 1;
}
