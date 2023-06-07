#define TKId 1
#define TKVoid 2
#define TKInt 3
#define TKFloat 4
#define TKVirgula 5
#define TKDoisPontos 6
#define TKAbreParenteses 7
#define TKFechaParenteses 8
#define TKAtrib 9
#define TKPontoEVirgula 10
#define TKAbreChaves 11
#define TKFechaChaves 12
#define TKSoma 13
#define TKDuploMais 14
#define TKProd 15
#define TKChar 16
#define TKSub 17
#define TKAbreColchete 18
#define TKFechaColchete 19
#define TKAtribMais 20
#define TKDouble 21
#define TKCteInt 22
#define TKFimArquivo 23
#define TKPonto 49
#define TKAcessoPointer 50
#define TKFimDaTabela 1000

//TKS Criados BitWise
#define TKBWAnd 24
#define TKBWOr 25
#define TKBWXOr 26
#define TKBWNot 27
#define TKBWRightShift 28
#define TKBWLeftShift 29

//TKS Criados relacionais
#define TKMaior 30
#define TKMenor 31
#define TKMaiorIgual 32
#define TKMenorIgual 33
#define TKIgual 34
#define TKDiferente 35

//TKS Criados Aritmeticos
#define TKDivisao 36
#define TKRestoDiv 37

//TKS Criados Referencias OK
#define TKReservadaDo 38
#define TKReservadaWhile 39
#define TKReservadaFor 40
#define TKReservadaIf 41
#define TKReservadaElse 42
#define TKReservadaStruct 43
#define TKReservadaCase 44
#define TKReservadaDefault 45

//TKS Criados Logicos
#define TKReservadaELogico 46
#define TKReservadaOuLogico 47
#define TKReservadaNegacao 48

#include <string.h>
#include <stdio.h>

bool erroLexico = false;
int tokenAtual = 0;
int tokenFinal = 0;
int linha = 1;
int coluna = 0;
int colunaInicial = 0;
int tk;
char lex[20]; //lexema gerado pela sequencia de leituras de tokens
FILE *arqin;
FILE *filePtr;

char c; //

struct infoToken{int linha; int coluna; int token;};

struct infoToken lista_tokens[999999];

struct palavraReservada{char palavra[20]; int tk;};

struct palavraReservada lista_pal[]={{"void",TKVoid},
                            {"int",TKInt},
                            {"float",TKFloat},
                            {"char",TKChar},
                            {"double",TKDouble},
                            {"do",TKReservadaDo},
                            {"while",TKReservadaWhile},
                            {"for",TKReservadaFor},
                            {"if",TKReservadaIf},
                            {"else",TKReservadaElse},
                            {"case",TKReservadaCase},
                            {"default",TKReservadaDefault},
                            {"struct",TKReservadaStruct},
                            {"fimtabela", TKFimDaTabela}};

struct palavraReservada lista_nome_tokens[]={{"TKId               ",TKId},
                                        {"TKVoid             ",TKVoid},
                                        {"TKInt              ",TKInt},
                                        {"TKFloat            ",TKFloat},
                                        {"TKVirgula          ",TKVirgula},
                                        {"TKDoisPontos       ",TKDoisPontos},
                                        {"TKAbreParenteses   ",TKAbreParenteses},
                                        {"TKFechaParenteses  ",TKFechaParenteses},
                                        {"TKAtrib            ",TKAtrib},
                                        {"TKPontoEVirgula    ",TKPontoEVirgula},
                                        {"TKAbreChaves       ",TKAbreChaves},
                                        {"TKFechaChaves      ",TKFechaChaves},
                                        {"TKSoma             ",TKSoma},
                                        {"TKDuploMais        ",TKDuploMais},
                                        {"TKProd             ",TKProd},
                                        {"TKChar             ",TKChar},
                                        {"TKSub              ",TKSub},
                                        {"TKAbreColchete     ",TKAbreColchete},
                                        {"TKFechaColchete    ",TKFechaColchete},
                                        {"TKAtribMais        ",TKAtribMais},
                                        {"TKDouble           ",TKDouble},
                                        {"TKCteInt           ",TKCteInt},
                                        {"TKFimArquivo       ",TKFimArquivo},
                                        {"TKPonto            ",TKPonto},
                                        {"TKBWAnd            ",TKBWAnd},
                                        {"TKBWOr             ",TKBWOr},
                                        {"TKBWXOr            ",TKBWXOr},
                                        {"TKBWNot            ",TKBWNot},
                                        {"TKBWRightShift     ",TKBWRightShift},
                                        {"TKBWLeftShift      ",TKBWLeftShift},
                                        {"TKMaior            ",TKMaior},
                                        {"TKMenor            ",TKMenor},
                                        {"TKMaiorIgual       ",TKMaiorIgual},
                                        {"TKMenorIgual       ",TKMenorIgual},
                                        {"TKIgual            ",TKIgual},
                                        {"TKDiferente        ",TKDiferente},
                                        {"TKDivisao          ",TKDivisao},
                                        {"TKRestoDiv         ",TKRestoDiv},
                                        {"TKReservadaDo      ",TKReservadaDo},
                                        {"TKReservadaWhile   ",TKReservadaWhile},
                                        {"TKReservadaFor     ",TKReservadaFor},
                                        {"TKReservadaIf      ",TKReservadaIf},
                                        {"TKReservadaElse    ",TKReservadaElse},
                                        {"TKReservadaStruct  ",TKReservadaStruct},
                                        {"TKReservadaCase    ",TKReservadaCase},
                                        {"TKReservadaDefault ",TKReservadaDefault},
                                        {"TKReservadaELogico ",TKReservadaELogico},
                                        {"TKReservadaOuLogico",TKReservadaOuLogico},
                                        {"TKReservadaNegacao ",TKReservadaNegacao},
                                        {"TKAcessoPointer    ",TKAcessoPointer},
                                        {"TKFimDaTabela      ",TKFimDaTabela}};

//compara o lexema gerado (token) com as palavras da lista de palavras reservadas
int palavra_reservada(char lex[])
{
    int postab=0;
    while (strcmp("fimtabela",lista_pal[postab].palavra)!=0)
    {
        if (strcmp(lex,lista_pal[postab].palavra)==0){
            return lista_pal[postab].tk; //se a palavra for igual alguma da lista, é uma palavra reservada e retornará o token dela
        }
        else postab++;
    }
    return TKId; //caso não seja uma palavra reservada, será considerada como um identificador
}

palavraReservada buscar_token(int token){
    int postab=0;
    while (strcmp("fimtabela",lista_nome_tokens[postab].palavra)!=0)
    {
        if (lista_nome_tokens[postab].tk==token){
            return lista_nome_tokens[postab]; //se o token for igual algum da lista
        }
        else postab++;
    }
    return lista_nome_tokens[postab];
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
    int estado=0, //monitora se o char é uma letra, se for, joga pra 1 para verificar se é uma palavra reservada
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
                    tk=TKCteInt;
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
                        tk=TKAtrib;/*printf("Reconheceu token TKAtrib\n");*/return;
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
                        tk=TKDuploMais;/*printf("Reconheceu token TKDuploMais\n");*/return;
                    }
                    else if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                       // proxC();
                        tk=TKAtribMais;/*printf("Reconheceu token TKAtribMais\n");*/return;
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
                        tk=TKAcessoPointer;/*printf("Reconheceu token TKAcessoPointer\n");*/return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        tk=TKSub;/*printf("Reconheceu token TKSub\n");*/return;
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
                        tk=TKBWRightShift;
                        /*printf("Reconheceu token TKBWRightShift\n");*/
                        return;
                    }
                    else if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        proxC();
                        tk=TKMaiorIgual;
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
                        tk=TKBWLeftShift;
                        /*printf("Reconheceu token TKBWLeftShift\n");*/
                        return;
                    }
                    else if (c=='=')
                    {
                        lex[posl++]='=';
                        lex[posl]='\0';
                        proxC();
                        tk=TKMenorIgual;
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
                        tk=TKReservadaNegacao;
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
                        tk=TKReservadaOuLogico;
                        /*printf("Reconheceu token TKDiferente\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        proxC();
                        tk=TKBWOr;/*printf("Reconheceu token TKBWOr\n");*/
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
                        tk=TKReservadaELogico;
                        /*printf("Reconheceu token TKDiferente\n");*/
                        return;
                    }
                    else
                    {
                        lex[posl]='\0';
                        proxC();
                        tk=TKBWAnd;/*printf("Reconheceu token TKBWAnd\n");*/
                        return;
                    }
                }

                if (c=='/'){lex[posl]='\0';proxC();tk=TKDivisao;        return;}
                if (c=='%'){lex[posl]='\0';proxC();tk=TKRestoDiv;       return;}
                if (c=='^'){lex[posl]='\0';proxC();tk=TKBWXOr;          return;}
                if (c=='~'){lex[posl]='\0';proxC();tk=TKBWNot;          return;}
                if (c=='*'){lex[posl]='\0';proxC();tk=TKProd;           return;}
                if (c=='['){lex[posl]='\0';proxC();tk=TKAbreColchete;   return;}
                if (c==']'){lex[posl]='\0';proxC();tk=TKFechaColchete;  return;}
                if (c=='('){lex[posl]='\0';proxC();tk=TKAbreParenteses; return;}
                if (c==')'){lex[posl]='\0';proxC();tk=TKFechaParenteses;return;}
                if (c=='{'){lex[posl]='\0';proxC();tk=TKAbreChaves;     return;}
                if (c=='}'){lex[posl]='\0';proxC();tk=TKFechaChaves;    return;}
                if (c==','){lex[posl]='\0';proxC();tk=TKVirgula;        return;}
                if (c==';'){lex[posl]='\0';proxC();tk=TKPontoEVirgula;  return;}
                if (c==':'){lex[posl]='\0';proxC();tk=TKDoisPontos;     return;}
                //if (c=='-'){lex[posl]='\0';proxC();tk=TKSub;            return;}
                if (c=='.'){lex[posl]='\0';proxC();tk=TKPonto;          return;}
                if (c==-1 ){lex[posl]='\0';proxC();tk=TKFimArquivo;     return;}
                if (c==' ' || c=='\n' || c=='\t' || c=='\r') {proxC();posl--;colunaInicial = coluna;break;}
                if (c=='\0') {tk=-1;return;}

                printf("Erro lexico: encontrou o caracter %c (%d)\n",c,c);
                erroLexico = true;

                while (c!='\n') proxC();
                break;

            case 1:
                if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9') {proxC();break; }
                lex[--posl]='\0';
              //  printf("lexema atual: %s\n", lex);
                tk=palavra_reservada(lex); //joga para função palavra reservada para verificar se é uma ou não
                return;
        } //switch
    }// while
}// funcao

bool validarLexico(){
    proxC(); // le primeiro caracter do arquivo
    getToken(); // le primeiro token

    printf("linha       coluna      token                        lexema\n");

    while (tk!=TKFimArquivo)
    {
        if (erroLexico)
            return false;

        palavraReservada token_encontrado = buscar_token(tk);

        lista_tokens[tokenAtual++] = {linha, coluna, tk};

        printf("%02d          %02d          %s          %s\n",linha,colunaInicial,token_encontrado.palavra,lex);
        getToken();
    }

    tokenFinal = tokenAtual-1;
    tokenAtual = 0;

    return true;
}

int multiplicacao();
int multiplicacao2();
int soma();
int soma2();
int expPrimaria();

int soma(){
    if(multiplicacao()){
        if(soma2()){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

int soma2(){
    if (lista_tokens[tokenAtual].token == TKSoma){
        tokenAtual++;
        if(multiplicacao()){
            if(soma2()){
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        return 1;
    }
}

int multiplicacao(){
    if (expPrimaria()){
        if(multiplicacao2()){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

int multiplicacao2(){
    if(lista_tokens[tokenAtual].token == TKProd){
        tokenAtual++;
        if(expPrimaria()){
            if(multiplicacao2()){
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        return 1;
    }
}

int expPrimaria(){
    if(lista_tokens[tokenAtual].token == TKId || lista_tokens[tokenAtual].token == TKCteInt){
        tokenAtual++;
        return 1;
    }
    else{
        return 0;
    }
}

int validarSintatico(){
    return soma();
}

int main()
{
    arqin=fopen("D:\\UCS\\Linguagens Formais\\TDE4\\codigo.txt","rb");

    if (!arqin) {
        printf("Erro na abertura do fonte.\n");
        return 0;
    }

    if (validarLexico()){
        printf("\nValidador Lexico aprovado!\nIniciado validador Sintatico:");
        if(validarSintatico()){
            printf("VALIDOU SINTATICO");
        }
        else{
            printf("NAO VALIDOU SINTATICO");
        }
    }
}
