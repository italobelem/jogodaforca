#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "forca.h"
#include <time.h>

char palavrasecreta[TAMANHO_PALAVRA];
char chutes[26];
int chutesdados = 0;

void abertura(){
    
    printf("\n--------------------------------\n");
    printf("Jogo da forca");
    printf("\n--------------------------------\n\n");

}

void chuta(){//* serve para declarar como um ponteiro, ou seja pega o CONTEUDO daquele endereço de memoria (APONTA para o endereço)

    char chute;

        scanf (" %c", &chute); //o espaço em %c serve para limpar o buffer do enter, SEMPRE FAZER ISSO COM CHAR
        
        chutes[chutesdados] = chute; /*chutes->todas as possibilidaddes de acertar (no caso as letras do alfabeto), chutesdados inicia como sendo zero e aumenta de acordo
        com a quantidade de vezes que um chute é dado, enquanto a variavel chute representa o palpite em si*/ 
        chutesdados++; //contabiliza os chutes dados      
 
}

int jachutou(char letra){

    int achou = 0; //inicia a variavel achou como sendo falsa

    for (int j = 0; j < chutesdados; j++){ //loop que procura uma letra chutada dentro da palavra secreta
        if (chutes[j] == letra) {

            achou = 1; //se for igual declara achou como verdadeiro
                break;
        }
    }
    return achou; //retorna o resultado de achou para o int main, ja que ainda vai ser usado la
}

void desenhaforca(){

    int erros = chuteserrados ();

        printf("  _______       \n");
        printf(" |/      |      \n");
        printf(" |      %c%c%c  \n", (erros >= 1 ? '(' : ' '), (erros >= 1 ? '_' : ' '), (erros >= 1 ? ')' : ' '));
        printf(" |      %c%c%c  \n", (erros >= 3 ? '\\' : ' '), (erros >=2  ? '|' : ' '), (erros >= 3 ? '/' : ' '));
        printf(" |       %c     \n", (erros >= 2 ? '|' : ' '));
        printf(" |      %c %c   \n", (erros >= 4 ?'/' : ' '),(erros >= 4 ? '\\' : ' '));
        printf(" |              \n");
        printf("_|___           \n");

        printf("\n\n");

    for (int i = 0; i < strlen(palavrasecreta); i++){
            
        int achou = jachutou(palavrasecreta[i]);

        if(achou){ //como achou é verdadeiro entra nesse if e printa palavra correta
            printf("%c ", palavrasecreta[i]);
        }

        else{ //como a letra nao estava na palavra, achou continua sendo falso e printa o tracinho para indicar que o chute foi errado
                printf("_ ");
        }
            
    }
}

int chuteserrados(){
        
    int erros = 0;

    for(int i = 0; i < chutesdados; i++){
        int existe = 0; //booleano, ou seja falso

        for(int j = 0; j < strlen(palavrasecreta); j++){

            if(chutes[i] == palavrasecreta[j]){

                existe = 1; //booleano pra verdadeiro, ou seja existe essa letra chutada
                break;
            }
        }
        //o [i] em chutes[i] representa a quantidade de vezes que o chute foi usado, e o [j] em palavra secreta se refere ao contador da verificação de encontrar o chute dentro da palavrasecreta
        
        if(!existe) erros++;
    }
    
    return erros;

}

void adcionarpalavra(){

    char quer;

    printf("Quer adcionar uma nova palavra? (S/N)");
    scanf(" %c", &quer);

    if(quer == 'S'){

        char novapalavra[TAMANHO_PALAVRA];
        printf("Qual a nova palavra? ");
        scanf("%s", novapalavra);

        FILE* f;

        f = fopen("palavras.txt", "r+"); //r+ é leitura e escrita em txt

        int qtd; //quantidade de linhas no arquivo (a primeira linha)
        fscanf(f, "%d", &qtd);
        qtd++; //soma um para nao perder a contagem de palavras

        fseek(f, 0, SEEK_SET); //mandou o ponteiro para o começo do arquivo
        fprintf(f, "%d", qtd); //imprime em cima

        fseek(f, 0, SEEK_END); //coloca o ponteiro no final do arquivo
        fprintf(f, "\n%s", novapalavra); //escreve no arquivo f 

        fclose(f);

    }
}

void escolhepalavra(){
FILE *f; //devolve sempre um ponteiro de um arquivo

f = fopen("palavras.txt", "r");//vai abrir o arquivo com as palavras, o r é uma mascara para apenas leitura (read)
if(f==0){
    printf("Banco de dados indisponível");
    exit(1);
}
    int qntddepalavras;
    fscanf(f, "%d", &qntddepalavras);//fscanf para ler um arquivo, primeiro parametro é sempre o arquivo(foi declarado como 'f' na linha anterior)

    srand(time(0));
    int randomico = rand() % qntddepalavras; //aqui rand é dividio pela quantidade de palavras do banco e sorteia um numero que vai de 1 ate a quantidade max de palavaras

    for(int i = 0; i <= randomico; i++){ //le as linhas ate chegar na que bate com o numero sorteado
        fscanf(f, "%s", &palavrasecreta); //importa a palavra do arquivo de texto para o programa
    }

    fclose(f);//"reserva o arquivo apenas para o uso do programa"
}

int acertou(){

    for(int i = 0; i < strlen(palavrasecreta); i++){
        if(!jachutou(palavrasecreta[i])){
            return 0;
        }
    }
    return 1;
//caso nao tenha chutado (!jachutou) todas as letras corretamente, acertou() é dado como 0 (falso), volta para o inicio do loop e vai verificando ate a palavra inteira estar correta,
//e entao retorna o acertou()como sendo verdadeiro, ou seja valendo 1 
}

int enforcou(){
    return chuteserrados() >= 5;
}

int main(){
    
    escolhepalavra();

    abertura();

    do {
        desenhaforca();
        
        printf("\n");
        
        chuta();

    }while (!acertou() && !enforcou());//executa o codigo enquanto o jogador NEM acerta e NEM perde, ou seja enquanto joga efetivamente (booleano)

    if(acertou()) {

        printf("Parabéns, você ganhou!");

    } else{

        printf("Você perdeu! :( \nMais sorte na próxima!)");

    }

    adcionarpalavra();
}