// @Humolot - Gianck 
// Projeto Estoque em C 
// ------ // ----------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>

#define ARQ "Dados.dat" /* Arquivo com os dados*/
#define OP_INSERIR '1'
#define OP_ALTERAR '2'
#define OP_APAGAR '3'
#define OP_LISTAR '4'
#define OP_LISTAR_IMC '5'
#define OP_SAIR '0'

char *MainMenu[]={
    "1. Inserir Aluno",
    "2. Alterar Aluno",
    "3. Apagar Aluno",
    "4. Listar Alunos",
    "5. Listar IMC Alunos",
    "0. Sair",
    NULL /* ACABARAM AS OPÇÕES*/
};

FILE *fp; /*variável global pois é útil ao longo do programa*/

typedef struct{
    char nome[20+1];
    int peso;
    float altura;
    char Status; /* '*' indica que o registro está apagado*/
}Pessoa;

void Mensagem(char *msg);

/* lê os dados de um registro introduzido pelo usuário*/
void Ler_Pessoa(Pessoa *p){
    printf("Nome:    "); gets(p->nome);
    printf("Peso:   "); scanf("%d",&p->peso);
    printf("Altura: "); scanf("%f",&p->altura);
    p->Status = ' ';
    fflush(stdin);
}


/*Mostra na tela os dados existentes no registro*/
void Mostrar_Pessoa(Pessoa p){
	printf("Aluno: %-20s Peso: %3d kg - Altura: %10.2f\n",p.nome, p.peso, p.altura);
}

void Mostrar_PessoaIMC(Pessoa p){
	
float peso, altura, IMC;
 IMC = p.peso/(pow(p.altura,2));
		
  if(IMC<17)
    {
       printf("Muito abaixo do peso.\n");
    }
    else
    {
        if(IMC>17 && IMC<18.49)
        {
            printf("Abaixo do peso.\n");
        }
        else
        {
            if(IMC>18.5 && IMC<24.99)
            {
                printf("Peso normal.\n");
            }
            else
            {
                if(IMC>25 && IMC<29.99)
                {
                    printf("Acima do peso.\n");
                }
                else
                {
                    if(IMC>30 && IMC<34.99)
                    {
                        printf("voce esta Obeso.\n");
                    }
                    else
                    {
                        if(IMC>35 && IMC<39.99)
                        {
                            printf("Obesidade Severa.\n");
                        }
                        else
                        {
                            if(IMC>40)
                            {
                                printf("voce esta em Obesidade MORBIDA.\n");
                            }
                        }
                    }
                }
            }
        }
    }
	
	
    printf("Aluno: %-20s Peso: %3d kg - Altura: %10.2f IMC: %10.2f \n",p.nome, p.peso, p.altura, IMC);
    printf("-------------------------------------------------------------------------------------\n");
}

/*Adiciona uma pessoa ao arquivo*/
void Adiciona_Pessoa(Pessoa p){
    fseek(fp, 0L, SEEK_END);
    if(fwrite(&p, sizeof(p), 1, fp)!=1)
        Mensagem("Adicionar pessoa: Falhou a escrita do registro");
}

/*Colocar uma mensagem na tela*/
void Mensagem(char *msg){
    printf(msg);
    getchar();
}



/*Verificar se o arquivo já existe. Se não existir, ele é criado
se já existir, abre-o em modo de leitura e escrita (r+b)
*/
void Inic(){
    fp= fopen(ARQ, "r+b"); //tentar abrir
    if(fp==NULL){
        fp = fopen(ARQ, "w+b"); // criar o arquivo
    }
}

/* Faz um menu simples com as opções do vetor de strings.
seleciona a opção, usando o primeiro caracter de cada string.
devolve o primeiro caracter da opção.
*/
char Menu(char *opcoes[]){
    int i;
    char ch;
    while(1){
        /*CLS */
        printf("\n\n\ SELECIONE UMA OPCAO ABAIXO: \n\n\n");
            for(i=0; opcoes[i]!=NULL; i++)
                printf("\t\t%s\n\n",opcoes[i]);
               
            printf("\n\n\t\tOpcao: ");
            ch = getchar(); fflush(stdin);
            for(i=0; opcoes[i]!= NULL; i++)
                if(opcoes[i][0]==ch)
                    return ch;
    }
}

void Inserir_Aluno(){
    Pessoa x;
    Ler_Pessoa(&x);
    Adiciona_Pessoa(x);
}

void Alterar_Aluno(){
    Pessoa x;
    long int n_reg;
    printf("Qual o numero do registro: ");
    scanf("%ld", & n_reg); fflush(stdin);
    if(fseek(fp, (n_reg-1)*sizeof(Pessoa), SEEK_SET)!=0){
        Mensagem("Registro inexistente ou problemas no posicionamento!!!");
        return;
    }
    if(fread(&x, sizeof(Pessoa), 1, fp)!= 1){
        Mensagem("Problemas na leitura do registro!!!");
        return;
    }
   
    if(x.Status=='*'){
        Mensagem("Um registro apagado não pode ser alterado!!! \n\n");
        return;
    }
   
    printf("\n\n Dados Atuais \n\n");
    Mostrar_Pessoa(x);
    printf("\n\n Novos Dados \n\n");
    Ler_Pessoa(&x);
   
    // recuar um registro no arquivo
    fseek(fp, -(long) sizeof(Pessoa), SEEK_CUR);
    // reescrever o registro;
    fwrite(&x, sizeof(Pessoa), 1, fp);
    fflush(fp); /*despejar os arquivos no disco rígido*/
}

void Apagar_Aluno(){
    Pessoa x;
    long int n_reg;
    char resp;
   
    printf("Qual o numero do registro: ");
    scanf("%ld", & n_reg); fflush(stdin);
    if(fseek(fp, (n_reg - 1)*sizeof(Pessoa), SEEK_SET)!= 0){
        Mensagem("Registro inexistente ou problemas no registro!!!");
        return;
    }
    if(fread(&x, sizeof(Pessoa), 1, fp)!= 1){
        Mensagem("Problema na leitura do registro!!!");
        return;
    }
    if(x.Status=='*'){
        Mensagem("Registro já está apagado!!!\n\n");
    }
    printf("\n\n Dados atuais \n\n");
    Mostrar_Pessoa(x);
    printf("\n\n Apagar o registro (s/n)???: "); resp = getchar();
    fflush(stdin);
    if(toupper(resp)!= 'S')return;
   
    x.Status= '*';
    // recuar um registro no arquivo
    fseek(fp, -(long) sizeof(Pessoa), SEEK_CUR);
    // reescrever o registro;
    fwrite(&x, sizeof(Pessoa), 1, fp);
    fflush(fp); /*Despejar os arquivos no disco rígido*/
}

void Listar(){
   
    long int n_Linhas = 0;
    Pessoa reg;
    rewind(fp);
    while(1){
        if(fread(&reg, sizeof(reg), 1, fp)!= 1)break; /*Sair do laço*/
        if(reg.Status=='*') continue; /*Passa ao próximo*/
        Mostrar_Pessoa(reg);
        n_Linhas++;
        if(n_Linhas%20==0) /* Paginação a cada 20 registros */
            Mensagem("Pressione <Enter> para continuar .  .  .");
    }
    Mensagem("\n\n Pressione <Enter> para continuar .  .  ."); /*No fim da listagem*/
}

void ListarIMC(){
   
    long int n_Linhas = 0;
    Pessoa reg;
    rewind(fp);
    while(1){
        if(fread(&reg, sizeof(reg), 1, fp)!= 1)break; /*Sair do laço*/
        if(reg.Status=='*') continue; /*Passa ao próximo*/
        Mostrar_PessoaIMC(reg);
        n_Linhas++;
        if(n_Linhas%20==0) /* Paginação a cada 20 registros */
            Mensagem("Pressione <Enter> para continuar .  .  .");
    }
    Mensagem("\n\n Pressione <Enter> para continuar .  .  ."); /*No fim da listagem*/
}


int main(int argc, char *argv[]) {
   
   
   
    char opcao;
    Inic();
    while((opcao = Menu(MainMenu))!= OP_SAIR)
        switch(opcao){
           
            case OP_INSERIR: Inserir_Aluno(); break;
            case OP_ALTERAR: Alterar_Aluno(); break;
            case OP_APAGAR: Apagar_Aluno(); break;
            case OP_LISTAR: Listar(); break;
            case OP_LISTAR_IMC: ListarIMC(); break;
                
        }
   
   
    return 0;
}

