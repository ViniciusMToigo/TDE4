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
#define TKLong 119 // long
#define TKFimDaTabela 1000
#define TKFimArquivo 1001


int linha = 1;
int coluna = 0;
int colunaInicial = 0;
int tk;
char lex[20]; //lexema gerado pela sequencia de leituras de tokens
FILE *arqin;
FILE *filePtr;

char c; //

struct palavraReservada{char palavra[20]; int tk;};

struct palavraReservada lista_pal[]={{"void",TKVoid},
                                     {"int",TKInt},
                                     {"float",TKFloat},
                                     {"char",TKChar},
                                     {"double",TKDouble},
                                     {"do",TKDo},
                                     {"while",TKWhile},
                                     {"for",TKFor},
                                     {"if",TKIf},
                                     {"else",TKElse},
                                     {"long",TKLong},
                                     {"unsigned",TKUnsigned},
                                     {"fimtabela", TKFimDaTabela}};

//compara o lexema gerado (token) com as palavras da lista de palavras reservadas
int palavra_reservada(char lex[])
{
    int postab=0;
    while (strcmp("fimtabela",lista_pal[postab].palavra)!=0)
    {
        if (strcmp(lex,lista_pal[postab].palavra)==0){
            return lista_pal[postab].tk; //se a palavra for igual alguma da lista, � uma palavra reservada e retornar� o token dela
        }
        else postab++;
    }
    return TKId; //caso n�o seja uma palavra reservada, ser� considerada como um identificador
}



void getToken();
void proxC();

void proxC()
{
    if (feof(arqin)) {
        c=-1; //se chegou no fim de arquivo
        return;
    }
    fread(&c,1,1,arqin);

    if (c=='\n'){
        //printf("CAIU NO IF ==> linha atual: %d ... coluna atual: %d ... char atual %c\n", linha, coluna, c);
        linha++;
        coluna = 0;
        colunaInicial = 1;
    }
    else{
        // printf("CAIU NO ELSE ==> linha atual: %d ... coluna atual: %d ... char atual %c\n", linha, coluna, c);
        coluna++;
    }
}

void getToken()
{
    int estado=0, //monitora se o char � uma letra, se for, joga pra 1 para verificar se � uma palavra reservada
    fim=0,
            posl=0;
    colunaInicial = coluna;

    while (!fim)
    {
        lex[posl++]=c;
        //printf("char atual: %c\n", c);
        switch(estado){
            case 0:
                if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_'){proxC();estado=1;break;}

                if (c>='0' && c<='9')
                {
                    posl--;
                    while (c>='0' && c<='9'){
                        lex[posl++] = c;
                        proxC();
                    }
                    lex[posl]='\0';
                    tk=TKInt;
                    return;
                }

                if (c=='=')
                {
                    proxC();

                    if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        proxC();
                        tk=TKIgual;/*printf("Reconheceu token TKIgual\n");*/return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        proxC();
                        tk=TKAtribuicao;/*printf("Reconheceu token TKAtrib\n");*/return;
                    }
                }

                if (c=='+')
                {
                    proxC();
                    if (c=='+')
                    {
                        lex[posl++]='+';
                        lex[posl]='\0';
                        //proxC();
                        tk=TKIncrementa;/*printf("Reconheceu token TKDuploMais\n");*/return;
                    }
                    else if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        // proxC();
                        tk=TKSomaAtribuicao;/*printf("Reconheceu token TKAtribMais\n");*/return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        tk=TKSoma;/*printf("Reconheceu token TKSoma\n");*/return;
                    }
                }

                if (c=='-')
                {
                    proxC();
                    if (c=='>')
                    {
                        lex[posl++]='>';
                        lex[posl]='\0';
                        proxC();
                        tk=TKPontero;/*printf("Reconheceu token TKAcessoPointer\n");*/return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        tk=TKDecrementa;/*printf("Reconheceu token TKSub\n");*/return;
                    }
                }

                if (c=='>')
                {
                    proxC();

                    if (c=='>')
                    {
                        lex[posl++]='>';
                        lex[posl]='\0';
                        proxC();
                        tk=TKDireitaBitwise;
                        /*printf("Reconheceu token TKBWRightShift\n");*/
                        return;
                    }
                    else if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        proxC();
                        tk=TKMaiorOuIgual;
                        /*printf("Reconheceu token TKMaiorIgual\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        tk=TKMaior;
                        /*printf("Reconheceu token TKMaior\n");*/
                        return;
                    }
                }

                if (c=='<')
                {
                    proxC();

                    if (c=='<')
                    {
                        lex[posl++]='<';
                        lex[posl]='\0';
                        proxC();
                        tk=TKEsquerdaBitwise;
                        /*printf("Reconheceu token TKBWLeftShift\n");*/
                        return;
                    }
                    else if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        proxC();
                        tk=TKMenorOuIgual;
                        /*printf("Reconheceu token TKMenorIgual\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        tk=TKMenor;
                        /*printf("Reconheceu token TKMenor\n");*/
                        return;
                    }
                }

                if (c=='!')
                {
                    proxC();
                    if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        proxC();
                        tk=TKDiferente;
                        /*printf("Reconheceu token TKDiferente\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        //proxC();
                        tk=TKNegacao;
                        /*printf("Reconheceu token TKDiferente\n");*/
                        return;
                    }
                }

                if (c=='|')
                {
                    proxC();

                    if (c=='|')
                    {
                        lex[posl++]='|';
                        lex[posl]='\0';
                        proxC();
                        tk=TKOuLogico;
                        /*printf("Reconheceu token TKDiferente\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        proxC();
                        tk=TKOuBitwise;/*printf("Reconheceu token TKBWOr\n");*/
                        return;
                    }
                }

                if (c=='&')
                {
                    proxC();
                    if (c=='&')
                    {
                        lex[posl++]='&';
                        lex[posl]='\0';
                        proxC();
                        tk=TKELogico ;
                        /*printf("Reconheceu token TKDiferente\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        proxC();
                        tk=TKEBitwise;/*printf("Reconheceu token TKBWAnd\n");*/
                        return;
                    }
                }

                if (c=='/'){lex[posl]='\0';proxC();tk=TKDivide;        return;}
                if (c=='%'){lex[posl]='\0';proxC();tk=TKResto;       return;}
                if (c=='^'){lex[posl]='\0';proxC();tk=TKXorBitwise;          return;}
                if (c=='~'){lex[posl]='\0';proxC();tk=TKNaoBitwise;          return;}
                if (c=='*'){lex[posl]='\0';proxC();tk=TKMultiplicacao;           return;}
                if (c=='['){lex[posl]='\0';proxC();tk=TKAbreColchetes;   return;}
                if (c==']'){lex[posl]='\0';proxC();tk=TKFechaColchetes;  return;}
                if (c=='('){lex[posl]='\0';proxC();tk=TKAbreParenteses; return;}
                if (c==')'){lex[posl]='\0';proxC();tk=TKFechaParenteses;return;}
                if (c=='{'){lex[posl]='\0';proxC();tk=TKAbreChaves;     return;}
                if (c=='}'){lex[posl]='\0';proxC();tk=TKFechaChaves;    return;}
                if (c==','){lex[posl]='\0';proxC();tk=TKVirgula;        return;}
                if (c==';'){lex[posl]='\0';proxC();tk=TKPontoEVirgula;  return;}
                if (c=='.'){lex[posl]='\0';proxC();tk=TKPonto;          return;}
                if (c==-1 ){lex[posl]='\0';proxC();tk=TKFimArquivo;     return;}
                if (c==' ' || c=='\n' || c=='\t' || c=='\r') {proxC();posl--;colunaInicial = coluna;break;}
                if (c=='\0') {tk=-1;return;}

                printf("Erro lexico: encontrou o caracter %c (%d)",c,c);

                while (c!='\n') proxC();
                break;

            case 1:
                if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9') {proxC();break; }
                lex[--posl]='\0';
                //  printf("lexema atual: %s\n", lex);
                tk=palavra_reservada(lex); //joga para fun��o palavra reservada para verificar se � uma ou n�o
                return;
        } //switch
    }// while
}// funcao


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
int ProgramaC(){
    if (Funcao()){
        if (RestoProgramaC()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else 	if (Declaracao()){
        if (RestoProgramaC()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoProgramaC -> Funcao RestoProgramaC | Declaracao RestoProgramaC | ?
int RestoProgramaC(){
    if (Funcao()){
        if (RestoProgramaC()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else 	if (Declaracao()){
        if (RestoProgramaC()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//Declaracao -> DeclaracaoSpec Declaracao1Linha | DeclaracaoDireto
int Declaracao(){
    if (DeclaracaoSpec()){
        if (Declaracao1Linha()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if (DeclaracaoDireto()){
        return 1;
    }//fecha21
    else{return 0;}
}//fecha25

//Declaracao1Linha -> ; | InitDeclaracaoLista ;
int Declaracao1Linha(){
    if(tk == TKPontoEVirgula){// ;
        getToken();
        return 1;
    }//fecha23
    else 	if (InitDeclaracaoLista()){
        if(tk == TKPontoEVirgula){// ;
            getToken();
            return 1;
        }//fecha24
        else{
            printf("Erro, esperava token ';'\n");
            printf("ERRO NA FUNCAO 'Declaracao1Linha'\n");
            return 0;}
    }//fecha24
    else{
        printf("Erro, esperava token ';'\n");
        printf("ERRO NA FUNCAO 'Declaracao1Linha'\n");
        return 0;}
}//fecha25

//DeclaracaoSpec -> Tipo DeclaracaoSpec1Linha
int DeclaracaoSpec(){
    if (Tipo()){
        return 1;
    }
    else{
        return 0;
    }
}//fecha25



//Tipo -> void | char | short | int | long | float | double | unsigned
int Tipo(){
    if(tk == TKVoid){// void
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKChar){// char
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKShort){// short
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKInt){// int
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKLong){// long
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKFloat){// float
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKDouble){// double
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKUnsigned){// unsigned
        getToken();
        return 1;
    }//fecha23
    else{
        printf("Erro, esperava token de inicializacao (void, char, short, int, long, float, double, unsigned...\n");
        printf("ERRO NA FUNCAO 'TIPO'\n");
        return 0;}
}//fecha25

//InitDeclaracaoLista -> InitDeclaracao InitDeclaracaoLista1Hash
int InitDeclaracaoLista(){
    if (InitDeclaracao()){
        if (InitDeclaracaoLista1Hash()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//InitDeclaracaoLista1Hash -> , InitDeclaracao InitDeclaracaoLista1Hash | ?
int InitDeclaracaoLista1Hash(){
    if(tk == TKVirgula){// ,
        getToken();
        if (InitDeclaracao()){
            if (InitDeclaracaoLista1Hash()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO InitDeclaracaoLista1Hash esperava ',' e derivou vazio\n");
        return 1;}
}//fecha25

//InitDeclaracao -> Declaracao InitDeclaracao1Linha
int InitDeclaracao(){
    if (Declaracao()){
        if (InitDeclaracao1Linha()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//InitDeclaracao1Linha -> = Init | ?
int InitDeclaracao1Linha(){
    if(tk == TKAtribuicao){// =
        getToken();
        if (Init()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO InitDeclaracao1Linha esperava '=' e derivou vazio\n");
        return 1;}
}//fecha25

//Pointer -> * Pointer | ?
int Pointer(){
    if(tk == TKMultiplicacao){// *
        getToken();
        if (Pointer()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO Pointer esperava '*' e derivou vazio\n");
        return 1;}
}//fecha25

//DeclaracaoDireto -> id RestoDeclaracaoDireto | ( Declaracao ) RestoDeclaracaoDireto
int DeclaracaoDireto(){
    if(tk == TKId){// id
        getToken();
        if (RestoDeclaracaoDireto()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKAbreParenteses){// (
        getToken();
        if (Declaracao()){
            if(tk == TKFechaParenteses){// )
                getToken();
                if (RestoDeclaracaoDireto()){
                    return 1;
                }//fecha22
                else{return 0;}
            }//fecha22
            else{
                printf("ERRO NA FUNCAO 'DeclaracaoDireto'\n");
                printf("Erro, esperava token ')'\n");
                return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'DeclaracaoDireto'\n");
        printf("Erro, esperava token 'TKId ou ('\n");
        return 0;}
}//fecha25

//RestoDeclaracaoDireto -> [ RestoDeclaracaoDireto2 | ( RestoDeclaracaoDireto3
int RestoDeclaracaoDireto(){
    if(tk == TKAbreColchetes){// [
        getToken();
        if (RestoDeclaracaoDireto2()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKAbreParenteses){// (
        getToken();
        if (RestoDeclaracaoDireto3()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto'\n");
        printf("Erro, esperava token '[ ou ('\n");
        return 0;}
}//fecha25

//RestoDeclaracaoDireto2 -> ExpressaoOr ] RestoDeclaracaoDireto | ] RestoDeclaracaoDireto
int RestoDeclaracaoDireto2(){
    if (ExpressaoOr()){
        if(tk == TKFechaColchetes){// ]
            getToken();
            if (RestoDeclaracaoDireto()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{
            printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto2'\n");
            printf("Erro, esperava token ']'\n");
            return 0;}
    }//fecha22
    else if(tk == TKFechaColchetes){// ]
        getToken();
        if (RestoDeclaracaoDireto()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto2'\n");
        printf("Erro, esperava token ']'\n");
        return 0;}
}//fecha25

//RestoDeclaracaoDireto3 -> ListaParametro ) RestoDeclaracaoDireto | ListaIdentificadores ) RestoDeclaracaoDireto | ) RestoDeclaracaoDireto
int RestoDeclaracaoDireto3(){
    if (ListaParametro()){
        if(tk == TKFechaParenteses){// )
            getToken();
            if (RestoDeclaracaoDireto()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{
            printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto3'\n");
            printf("Erro, esperava token ')'\n");
            return 0;}
    }//fecha22
    else 	if (ListaIdentificadores()){
        if(tk == TKFechaParenteses){// )
            getToken();
            if (RestoDeclaracaoDireto()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{
            printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto3'\n");
            printf("Erro, esperava token ')'\n");
            return 0;}
    }//fecha22
    else if(tk == TKFechaParenteses){// )
        getToken();
        if (RestoDeclaracaoDireto()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'RestoDeclaracaoDireto3'\n");
        printf("Erro, esperava token ')'\n");
        return 0;}
}//fecha25

//ListaParametro -> DeclaracaoParametro ListaParametro1Hash
int ListaParametro(){
    if (DeclaracaoParametro()){
        if (ListaParametro1Hash()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ListaParametro1Hash -> , DeclaracaoParametro ListaParametro1Hash | ?
int ListaParametro1Hash(){
    if(tk == TKVirgula){// ,
        getToken();
        if (DeclaracaoParametro()){
            if (ListaParametro1Hash()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO ListaParametro1Hash esperava ',' e derivou vazio\n");
        return 1;}
}//fecha25

//DeclaracaoParametro -> DeclaracaoSpec DeclaracaoParametro1Linha
int DeclaracaoParametro(){
    if (DeclaracaoSpec()){
        if (DeclaracaoParametro1Linha()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//DeclaracaoParametro1Linha -> Declaracao | ?
int DeclaracaoParametro1Linha(){
    if (Declaracao()){
        return 1;
    }//fecha21
    else {return 1;}
}//fecha25

//ListaIdentificadores -> id RestoListaIdentificadores
int ListaIdentificadores(){
    if(tk == TKId){// id
        getToken();
        if (RestoListaIdentificadores()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'ListaIdentificadores'\n");
        printf("Erro, esperava token 'TKid'\n");
        return 0;}
}//fecha25

//RestoListaIdentificadores -> , id RestoListaIdentificadores | ?
int RestoListaIdentificadores(){
    if(tk == TKVirgula){// ,
        getToken();
        if(tk == TKId){// id
            getToken();
            if (RestoListaIdentificadores()){
                return 1;
            }//fecha22
            else{
                printf("ERRO NA FUNCAO 'RestoListaIdentificadores'\n");
                printf("Erro, esperava token 'TKid'\n");
                return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoListaIdentificadores esperava ',' e derivou vazio\n");
        return 1;}
}//fecha25

//Funcao -> DeclaracaoSpec Declaracao Funcao1Linha | Declaracao Funcao2Linha
int Funcao(){
    if (DeclaracaoSpec()){
        if (Declaracao()){
            if (Funcao1Linha()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else 	if (Declaracao()){
        if (Funcao2Linha()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//Funcao2Linha -> ListaDeclaracao ComandoComposto | ComandoComposto
int Funcao2Linha(){
    if (ListaDeclaracao()){
        if (ComandoComposto()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if (ComandoComposto()){
        return 1;
    }//fecha21
    else{return 0;}
}//fecha25

//Funcao1Linha -> ListaDeclaracao ComandoComposto | ComandoComposto
int Funcao1Linha(){
    if (ListaDeclaracao()){
        if (ComandoComposto()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if (ComandoComposto()){
        return 1;
    }//fecha21
    else{return 0;}
}//fecha25

//ListaDeclaracao -> Declaracao ListaDeclaracao1Hash
int ListaDeclaracao(){
    if (Declaracao()){
        if (ListaDeclaracao1Hash()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ListaDeclaracao1Hash -> Declaracao ListaDeclaracao1Hash | ?
int ListaDeclaracao1Hash(){
    if (Declaracao()){
        if (ListaDeclaracao1Hash()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//ComandoComposto -> { ComandoComposto2
int ComandoComposto(){
    if(tk == TKAbreChaves){// {
        getToken();
        if (ComandoComposto2()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'ComandoComposto'\n");
        printf("Erro, esperava token '{'\n");
        return 0;}
}//fecha25

//ComandoComposto2 -> } | ListaComando } | ListaDeclaracao ComandoComposto21Linha
int ComandoComposto2(){
    if(tk == TKFechaChaves){// }
        getToken();
        return 1;
    }//fecha23
    else 	if (ListaComando()){
        if(tk == TKFechaChaves){// }
            getToken();
            return 1;
        }//fecha24
        else{
            printf("ERRO NA FUNCAO 'ComandoComposto2'\n");
            printf("Erro, esperava token '}'\n");
            return 0;}
    }//fecha24
    else 	if (ListaDeclaracao()){
        if (ComandoComposto21Linha()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'ComandoComposto2'\n");
        printf("Erro, esperava token '}'\n");
        return 0;}
}//fecha25

//ComandoComposto21Linha -> } | ListaComando }
int ComandoComposto21Linha(){
    if(tk == TKFechaChaves){// }
        getToken();
        return 1;
    }//fecha23
    else 	if (ListaComando()){
        if(tk == TKFechaChaves){// }
            getToken();
            return 1;
        }//fecha24
        else{
            printf("ERRO NA FUNCAO 'ComandoComposto21Linha'\n");
            printf("Erro, esperava token '}'\n");
            return 0;}
    }//fecha24
    else{
        printf("ERRO NA FUNCAO 'ComandoComposto21Linha'\n");
        printf("Erro, esperava token '}'\n");
        return 0;}
}//fecha25

//ListaComando -> Comando ListaComando1Hash
int ListaComando(){
    if (Comando()){
        if (ListaComando1Hash()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ListaComando1Hash -> Comando ListaComando1Hash | ?
int ListaComando1Hash(){
    if (Comando()){
        if (ListaComando1Hash()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//Comando -> ComandoComposto | ComandoExpressao | ComandoSelecao | ComandoIterativo
int Comando(){
    if (ComandoComposto()){
        return 1;
    }//fecha21
    else if (ComandoExpressao()){
        return 1;
    }//fecha21
    else if (ComandoSelecao()){
        return 1;
    }//fecha21
    else if (ComandoIterativo()){
        return 1;
    }//fecha21
    else{return 0;}
}//fecha25

//ComandoExpressao -> ; | Expressao ;
int ComandoExpressao(){
    if(tk == TKPontoEVirgula){// ;
        getToken();
        return 1;
    }//fecha23
    else 	if (Expressao()){
        if(tk == TKPontoEVirgula){// ;
            getToken();
            return 1;
        }//fecha24
        else{
            printf("ERRO NA FUNCAO 'ComandoExpressao'\n");
            printf("Erro, esperava token ';'\n");
            return 0;}
    }//fecha24
    else{
        printf("ERRO NA FUNCAO 'ComandoExpressao'\n");
        printf("Erro, esperava token ';'\n");
        return 0;}
}//fecha25

//ComandoSelecao -> if ( Expressao ) Comando ComandoSelecao1Linha
int ComandoSelecao(){
    if(tk == TKIf){// if
        getToken();
        if(tk == TKAbreParenteses){// (
            getToken();
            if (Expressao()){
                if(tk == TKFechaParenteses){// )
                    getToken();
                    if (Comando()){
                        if (ComandoSelecao1Linha()){
                            return 1;
                        }//fecha22
                        else{return 0;}
                    }//fecha22
                    else{return 0;}
                }//fecha22
                else{
                    printf("ERRO NA FUNCAO 'ComandoSelecao'\n");
                    printf("Erro, esperava token ')'\n");
                    return 0;}
            }//fecha22
            else{return 0;}
        }//fecha22
        else{
            printf("ERRO NA FUNCAO 'ComandoSelecao'\n");
            printf("Erro, esperava token '('\n");
            return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'ComandoSelecao'\n");
        printf("Erro, esperava token 'TKif'\n");
        return 0;}
}//fecha25

//ComandoSelecao1Linha -> else Comando | ?
int ComandoSelecao1Linha(){
    if(tk == TKElse){// else
        getToken();
        if (Comando()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO ComandoSelecao1Linha esperava 'else' e derivou vazio\n");
        return 1;}
}//fecha25

//ComandoIterativo -> while ( Expressao ) Comando | do Comando while ( Expressao ) ; | for ( ComandoExpressao ComandoExpressao ComandoIterativo1Linha
int ComandoIterativo(){
    if(tk == TKWhile){// while
        getToken();
        if(tk == TKAbreParenteses){// (
            getToken();
            if (Expressao()){
                if(tk == TKFechaParenteses){// )
                    getToken();
                    if (Comando()){
                        return 1;
                    }//fecha22
                    else{return 0;}
                }//fecha22
                else{
                    printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                    printf("Erro, esperava token ')'\n");
                    return 0;}
            }//fecha22
            else{return 0;}
        }//fecha22
        else{
            printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
            printf("Erro, esperava token ')'\n");
            return 0;}
    }//fecha22
    else if(tk == TKDo){// do
        getToken();
        if (Comando()){
            if(tk == TKWhile){// while
                getToken();
                if(tk == TKAbreParenteses){// (
                    getToken();
                    if (Expressao()){
                        if(tk == TKFechaParenteses){// )
                            getToken();
                            if(tk == TKPontoEVirgula){// ;
                                getToken();
                                return 1;
                            }//fecha24
                            else{
                                printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                                printf("Erro, esperava token ';'\n");
                                return 0;}
                        }//fecha24
                        else{
                            printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                            printf("Erro, esperava token ')'\n");
                            return 0;}
                    }//fecha24
                    else{return 0;}
                }//fecha24
                else{
                    printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
                    printf("Erro, esperava token '('\n");
                    return 0;}
            }//fecha24
            else{return 0;}
        }//fecha24
        else{return 0;}
    }//fecha24
    else if(tk == TKFor){// for
        getToken();
        if(tk == TKAbreParenteses){// (
            getToken();
            if (ComandoExpressao()){
                if (ComandoExpressao()){
                    if (ComandoIterativo1Linha()){
                        return 1;
                    }//fecha22
                    else{return 0;}
                }//fecha22
                else{return 0;}
            }//fecha22
            else{return 0;}
        }//fecha22
        else{
            printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
            printf("Erro, esperava token ')'\n");
            return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'ComandoIterativo'\n");
        printf("Erro, esperava token 'do' ou 'while'\n");
        return 0;}
}//fecha25

//ComandoIterativo1Linha -> ) Comando | Expressao ) Comando
int ComandoIterativo1Linha(){
    if(tk == TKFechaParenteses){// )
        getToken();
        if (Comando()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else 	if (Expressao()){
        if(tk == TKFechaParenteses){// )
            getToken();
            if (Comando()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'ComandoIterativo1Linha'\n");
        printf("Erro, esperava token ')'\n");
        return 0;}
}//fecha25

//Init -> Atribuicao | { InitList Init1Linha
int Init(){
    if (Atribuicao()){
        return 1;
    }//fecha21
    else if(tk == TKAbreChaves){// {
        getToken();
        if (InitList()){
            if (Init1Linha()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else{
        printf("ERRO NA FUNCAO 'Init'\n");
        printf("Erro, esperava token '{'\n");
        return 0;}
}//fecha25

//Init1Linha -> } | , }
int Init1Linha(){
    if(tk == TKFechaChaves){// }
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKVirgula){// ,
        getToken();
        if(tk == TKFechaChaves){// }
            getToken();
            return 1;
        }//fecha24
        else{
            printf("ERRO NA FUNCAO 'Init1Linha'\n");
            printf("Erro, esperava token '}'\n");
            return 0;}
    }//fecha24
    else{
        printf("ERRO NA FUNCAO 'Init1Linha'\n");
        printf("Erro, esperava token '}' ou ','\n");
        return 0;}
}//fecha25

//InitList -> Init | RestoInitList
int InitList(){
    if (Init()){
        return 1;
    }//fecha21
    else if (RestoInitList()){
        return 1;
    }//fecha21
    else{return 0;}
}//fecha25

//RestoInitList -> , Init RestoInitList | ?
int RestoInitList(){
    if(tk == TKVirgula){// ,
        getToken();
        if (Init()){
            if (RestoInitList()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoInitList esperava ',' e derivou vazio\n");
        return 1;}
}//fecha25

//Atribuicao -> ExpressaoOr | ExpressaoUnaria OperadorAtribuicao Atribuicao
int Atribuicao(){
    if (ExpressaoOr()){
        return 1;
    }//fecha21
    else 	if (ExpressaoUnaria()){
        if (OperadorAtribuicao()){
            if (Atribuicao()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ExpressaoOr -> ExpressaoAnd RestoExpressaoOr
int ExpressaoOr(){
    if (ExpressaoAnd()){
        if (RestoExpressaoOr()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoOr -> or_op ExpressaoAnd RestoExpressaoOr | ?
int RestoExpressaoOr(){
    if(tk == TKOuLogico){// or_op
        getToken();
        if (ExpressaoAnd()){
            if (RestoExpressaoOr()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoExpressaoOr esperava 'TKOuLogico' e derivou vazio\n");
        return 1;}
}//fecha25

//ExpressaoAnd -> OuInclusivo RestoExpressaoAnd
int ExpressaoAnd(){
    if (OuInclusivo()){
        if (RestoExpressaoAnd()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoAnd -> && OuInclusivo RestoExpressaoAnd | ?
int RestoExpressaoAnd(){
    if(tk == TKELogico){// &&
        getToken();
        if (OuInclusivo()){
            if (RestoExpressaoAnd()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoExpressaoAnd esperava 'TKOuLogico' e derivou vazio\n");
        return 1;}
}//fecha25

//OuInclusivo -> OuExclusivo RestoOuInclusivo
int OuInclusivo(){
    if (OuExclusivo()){
        if (RestoOuInclusivo()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoOuInclusivo -> PIPE OuExclusivo RestoOuInclusivo | ?
int RestoOuInclusivo(){
    if (tk == TKOuBitwise){
        getToken();
        if (OuExclusivo()){
            if (RestoOuInclusivo()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoOuInclusivo esperava 'TKOuBitwise' e derivou vazio\n");
        return 1;}
}//fecha25

//OuExclusivo -> AndBitwise RestoOuExclusivo
int OuExclusivo(){
    if (AndBitwise()){
        if (RestoOuExclusivo()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoOuExclusivo -> ^ AndBitwise RestoOuExclusivo | ?
int RestoOuExclusivo(){
    if(tk == TKXorBitwise){// ^
        getToken();
        if (AndBitwise()){
            if (RestoOuExclusivo()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoOuExclusivo esperava 'TKXorBitwise' e derivou vazio\n");
        return 1;}
}//fecha25

//AndBitwise -> ExpressaoIgual RestoAndBitwise
int AndBitwise(){
    if (ExpressaoIgual()){
        if (RestoAndBitwise()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoAndBitwise -> & ExpressaoIgual RestoAndBitwise | ?
int RestoAndBitwise(){
    if(tk == TKEBitwise){// &
        getToken();
        if (ExpressaoIgual()){
            if (RestoAndBitwise()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//ExpressaoIgual -> ExpressaoRelac RestoExpressaoIgual
int ExpressaoIgual(){
    if (ExpressaoRelac()){
        if (RestoExpressaoIgual()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoIgual -> == ExpressaoRelac RestoExpressaoIgual | != ExpressaoRelac RestoExpressaoIgual | ?
int RestoExpressaoIgual(){
    if(tk == TKIgual){// ==
        getToken();
        if (ExpressaoRelac()){
            if (RestoExpressaoIgual()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKDiferente){// !=
        getToken();
        if (ExpressaoRelac()){
            if (RestoExpressaoIgual()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {
        printf("WARNING NA FUNCAO RestoExpressaoIgual esperava '= ou ==' e derivou vazio");
        return 1;}
}//fecha25

//ExpressaoRelac -> ExpressaoDesl RestoExpressaoRelac
int ExpressaoRelac(){
    if (ExpressaoDesl()){
        if (RestoExpressaoRelac()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoRelac -> < ExpressaoDesl RestoExpressaoRelac | > ExpressaoDesl RestoExpressaoRelac | <= ExpressaoDesl RestoExpressaoRelac | >= ExpressaoDesl RestoExpressaoRelac | ?
int RestoExpressaoRelac(){
    if(tk == TKMenor){// <
        getToken();
        if (ExpressaoDesl()){
            if (RestoExpressaoRelac()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKMaior){// >
        getToken();
        if (ExpressaoDesl()){
            if (RestoExpressaoRelac()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKMenorOuIgual){// <=
        getToken();
        if (ExpressaoDesl()){
            if (RestoExpressaoRelac()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKMaiorOuIgual){// >=
        getToken();
        if (ExpressaoDesl()){
            if (RestoExpressaoRelac()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//ExpressaoDesl -> ExpressaoAd RestoExpressaoDesl
int ExpressaoDesl(){
    if (ExpressaoAd()){
        if (RestoExpressaoDesl()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoDesl -> << ExpressaoAd RestoExpressaoDesl | >> ExpressaoAd RestoExpressaoDesl
int RestoExpressaoDesl(){
    if(tk == TKEsquerdaBitwise){// <<
        getToken();
        if (ExpressaoAd()){
            if (RestoExpressaoDesl()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKDireitaBitwise){// >>
        getToken();
        if (ExpressaoAd()){
            if (RestoExpressaoDesl()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ExpressaoAd -> ExpressaoMult RestoExpressaoAd
int ExpressaoAd(){
    if (ExpressaoMult()){
        if (RestoExpressaoAd()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoAd -> + ExpressaoMult RestoExpressaoAd | - ExpressaoMult RestoExpressaoAd | ?
int RestoExpressaoAd(){
    if(tk == TKSoma){// +
        getToken();
        if (ExpressaoMult()){
            if (RestoExpressaoAd()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKSubtrai){// -
        getToken();
        if (ExpressaoMult()){
            if (RestoExpressaoAd()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//ExpressaoMult -> ExpressaoUnaria RestoExpressaoMult
int ExpressaoMult(){
    if (ExpressaoUnaria()){
        if (RestoExpressaoMult()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoMult -> * ExpressaoUnaria RestoExpressaoMult | / ExpressaoUnaria RestoExpressaoMult | % ExpressaoUnaria RestoExpressaoMult | ?
int RestoExpressaoMult(){
    if(tk == TKMultiplicacao){// *
        getToken();
        if (ExpressaoUnaria()){
            if (RestoExpressaoMult()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKDivide){// /
        getToken();
        if (ExpressaoUnaria()){
            if (RestoExpressaoMult()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKResto){// %
        getToken();
        if (ExpressaoUnaria()){
            if (RestoExpressaoMult()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//ExpressaoUnaria -> ExpressaoPosfix | ++ ExpressaoUnaria | -- ExpressaoUnaria | OperadorUnario ExpressaoUnaria
int ExpressaoUnaria(){
    if (ExpressaoPosfix()){
        return 1;
    }//fecha21
    else if(tk == TKIncrementa){// ++
        getToken();
        if (ExpressaoUnaria()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKDecrementa){// --
        getToken();
        if (ExpressaoUnaria()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else 	if (OperadorUnario()){
        if (ExpressaoUnaria()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ExpressaoPosfix -> ExpressaoPrim RestoExpressaoPosfix
int ExpressaoPosfix(){
    if (ExpressaoPrim()){
        if (RestoExpressaoPosfix()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressaoPosfix -> [ Expressao ] RestoExpressaoPosfix | ( RestoExpressaoPosfix2 | . id RestoExpressaoPosfix | -> id RestoExpressaoPosfix | ++ RestoExpressaoPosfix | -- RestoExpressaoPosfix | ?
int RestoExpressaoPosfix(){
    if(tk == TKAbreColchetes){// [
        getToken();
        if (Expressao()){
            if(tk == TKFechaColchetes){// ]
                getToken();
                if (RestoExpressaoPosfix()){
                    return 1;
                }//fecha22
                else{return 0;}
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKAbreParenteses){// (
        getToken();
        if (RestoExpressaoPosfix2()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKPonto){// .
        getToken();
        if(tk == TKId){// id
            getToken();
            if (RestoExpressaoPosfix()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKPontero){// ->
        getToken();
        if(tk == TKId){// id
            getToken();
            if (RestoExpressaoPosfix()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKIncrementa){// ++
        getToken();
        if (RestoExpressaoPosfix()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKDecrementa){// --
        getToken();
        if (RestoExpressaoPosfix()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25

//RestoExpressaoPosfix2 -> ArgumentList ) RestoExpressaoPosfix | ) RestoExpressaoPosfix
int RestoExpressaoPosfix2(){
    if (ArgumentList()){
        if(tk == TKFechaParenteses){// )
            getToken();
            if (RestoExpressaoPosfix()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else if(tk == TKFechaParenteses){// )
        getToken();
        if (RestoExpressaoPosfix()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//ExpressaoPrim -> id | constant | string_literal | ( Expressao )
int ExpressaoPrim(){
    if(tk == TKId){// id
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKConstante){// constant
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKString){// string_literal
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKAbreParenteses){// (
        getToken();
        if (Expressao()){
            if(tk == TKFechaParenteses){// )
                getToken();
                return 1;
            }//fecha24
            else{return 0;}
        }//fecha24
        else{return 0;}
    }//fecha24
    else{return 0;}
}//fecha25

//ArgumentList -> Atribuicao RestoArgumentList
int ArgumentList(){
    if (Atribuicao()){
        if (RestoArgumentList()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoArgumentList -> , Atribuicao RestoArgumentList
int RestoArgumentList(){
    if(tk == TKVirgula){// ,
        getToken();
        if (Atribuicao()){
            if (RestoArgumentList()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//OperadorUnario -> & | * | + | - | ~ | !
int OperadorUnario(){
    if(tk == TKEBitwise){// &
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKMultiplicacao){// *
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKSoma){// +
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKSubtrai){// -
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKNaoBitwise){// ~
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKNegacao){// !
        getToken();
        return 1;
    }//fecha23
    else{return 0;}
}//fecha25

//OperadorAtribuicao -> = | *= | /= | %= | += | -= | <<= | >>= | &= | ^= | or_assign
int OperadorAtribuicao(){
    if(tk == TKAtribuicao){// =
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKMultiplicacaoAtribuicao){// *=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKDivisaoAtribuicao){// /=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKRestoAtribuicao){// %=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKSomaAtribuicao){// +=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKSubtraiAtribuicao){// -=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKAtribuicaoEsquerdaBitwise){// <<=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKAtribuicaoDireitoBitwise){// >>=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKEAtribuicaoBitwise){// &=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKXorAtribuicaoBitwise){// ^=
        getToken();
        return 1;
    }//fecha23
    else if(tk == TKOuAtribuicaoBitwise){// or_assign
        getToken();
        return 1;
    }//fecha23
    else{return 0;}
}//fecha25

//Expressao -> Atribuicao RestoExpressao
int Expressao(){
    if (Atribuicao()){
        if (RestoExpressao()){
            return 1;
        }//fecha22
        else{return 0;}
    }//fecha22
    else{return 0;}
}//fecha25

//RestoExpressao -> , Atribuicao RestoExpressao | ?
int RestoExpressao(){
    if(tk == TKVirgula){// ,
        getToken();
        if (Atribuicao()){
            if (RestoExpressao()){
                return 1;
            }//fecha22
            else{return 0;}
        }//fecha22
        else{return 0;}
    }//fecha22
    else {return 1;}
}//fecha25



int main(){
    arqin=fopen("C:\\Users\\filip\\Documents\\Formais\\TDE4\\codigoTeste.txt","rb");

    if (!arqin) {
        printf("Erro na abertura do fonte.\n");
        return 0;
    }

    proxC(); // le primeiro caracter do arquivo
    getToken(); // le primeiro token

    while (tk!=TKFimArquivo)
    {
        //struct palavraReservada token_encontrado = buscar_token(tk);
        getToken();
    }

    printf("Passou no lexico!!!\nAgora Vamos testar o sintatico\n");
    printf("*********************************\n"
           "*********************************\n"
           "*********************************\n"
           "*********************************\n\n");

    arqin=fopen("C:\\Users\\filip\\Documents\\Formais\\TDE4\\codigoTeste.txt","rb");

    proxC(); // le primeiro caracter do arquivo
    getToken(); // le primeiro token
    if(ProgramaC()){
        printf("Passou no sintatico!\n");
    }
    else printf("Erro sintatico!\n");
    return 1;
}