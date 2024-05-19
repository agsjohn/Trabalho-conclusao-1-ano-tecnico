#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

FILE *arquivo;
char *nomearq="ferramentas.dat";

typedef struct{
  int ID;
  char Item[100];
  char Marca[100];
  char Cor[20];
  float Peso;
  char Fabricante[100];
  int Ano_de_Fabricacao;
  float Preco;
  char Isolado;
}ferramenta;

void MostrarF(){ //Função de mostrar todas ferramentas
  system("clear");
  ferramenta F;
  int vazio = 0, tam = 0;
  arquivo = fopen(nomearq, "rb");
  fseek(arquivo, 0, SEEK_SET);
  while(!feof(arquivo)){
    fread(&F, sizeof(ferramenta), 1, arquivo);
    if(feof(arquivo)){break;}
    tam++;
  }
  printf("Registros: %i\n\n", tam);
  fseek(arquivo, 0, SEEK_SET);
  while(!feof(arquivo)){  //Enquanto não for o fim do arquivo ele mostra cada ferramenta
    fread(&F, sizeof(ferramenta), 1, arquivo);
    if(feof(arquivo)){break;};
    vazio++;
    printf("//------------------------------//");
    printf("\nID %i", F.ID);
    printf("\nItem: %s", F.Item);
    printf("\nMarca: %s", F.Marca);
    printf("\nCor: %s", F.Cor);
    printf("\nPeso: %.2f", F.Peso);
    printf("\nFabricante: %s", F.Fabricante);
    printf("\nAno de Fabricação: %i", F.Ano_de_Fabricacao);
    printf("\nPreço: R$%.2f", F.Preco);
    if(F.Isolado == 's'){
      printf("\nIsolado: sim\n\n");
    } else{
      printf("\nIsolado: não\n\n");
    }
  }
  if(vazio == 0){
    printf("Arquivo vazio\n");
  }
  fclose(arquivo);
  printf("\n");
}
void InserirF(){ //Função de inserir ferramenta
  system("clear");
  ferramenta F;
  ferramenta test;
  int repetido;
  printf("Digite as informações sobre a ferramenta\n");
  printf("Digite o ID: ");
  scanf("%i", &F.ID);
  getchar();
  arquivo = fopen(nomearq, "rb");
  do{ //Verifica se o ID digitado existe, bloqueando um cadastro com o mesmo ID e também bloqueia um cadastro com ID 0 ou menor que 0
    while(F.ID < 0){
      printf("Digite um ID maior ou igual a 0: ");
      scanf("%i", &F.ID);
      getchar();
    }
    fseek(arquivo, 0, SEEK_SET);
    repetido=0;
    while(!feof(arquivo)){
      fread(&test, sizeof(ferramenta), 1, arquivo);
      if(feof(arquivo)){break;}
      if(test.ID == F.ID){
        repetido++;
      }
    }
    if(repetido > 0){
      printf("O ID digitado já existe, digite um ID diferente por favor: ");
      scanf("%i", &F.ID);
      getchar();
    }
  }while(repetido > 0);
  fclose(arquivo);
  printf("Digite qual o item: ");
  gets(F.Item);
  printf("Digite a marca: ");
  gets(F.Marca);
  printf("Cor: ");
  gets(F.Cor);
  printf("Digite o peso: ");
  scanf("%f", &F.Peso);
  getchar();
  printf("Digite a fabricante: ");
  gets(F.Fabricante);
  printf("Digite o ano de fabricação: ");
  scanf("%i", &F.Ano_de_Fabricacao);
  getchar();
  printf("Digite o preço: ");
  scanf("%f", &F.Preco);
  getchar();
  printf("Isolado(s/n):  ");
  scanf("%c", &F.Isolado);
  getchar();
  while(F.Isolado != 's' && F.Isolado != 'n'){ //Verificação do "Isolado" que só aceita 's' e 'n'
    if(F.Isolado == 'S' || F.Isolado == 'N'){
      printf("Digite em letra minúscula: ");
      scanf("%c", &F.Isolado);
      getchar();
    } else {
      printf("Digite novamente, apenas com 's' para sim e 'n' para não: ");
      scanf("%c", &F.Isolado);
      getchar();
    }
  }
  arquivo = fopen(nomearq, "ab+");
  fwrite(&F, sizeof(ferramenta), 1, arquivo);
  fclose(arquivo);
  printf("\n\n");
}
void RemoverF(){
  char esc='n';
  int num;
  int tam=0;
  int existe=0;
  int fazer;
  ferramenta test;
  arquivo = fopen(nomearq, "rb");
  while(!feof(arquivo)){ //Descobre a quantidade de ferramentas cadastradas
    fread(&test, sizeof(ferramenta), 1, arquivo);
    if(feof(arquivo)){break;};
    tam++;
  }
  ferramenta *vet;
  vet = malloc(tam*sizeof(ferramenta));
  do{ //Verifica se o ID digitado já existe, no caso de não existir apresenta a escolha ou não de prosseguir com a exclusão com outro ID
    fazer = 0;
    esc = 'n';
    printf("\nDigite o ID da ferramenta que deseja excluir: ");
    scanf("%i", &num);
    getchar();
    fseek(arquivo, 0, SEEK_SET);
    while(!feof(arquivo)){ //Verifica se o ID digitado existe
      fread(&test, sizeof(ferramenta), 1, arquivo);
      if(feof(arquivo)){break;};
      if(test.ID == num){fazer++;}
    }
    if(fazer == 0){ //Caso o ID digitado não exista faz a escolha se ainda quer digitar outro ID para excluir ou cancelar a operação
      printf("ID inválido, deseja digitar novamente? (s/n)");
      scanf("%c", &esc);
      getchar();
      while(esc != 's' && esc != 'n'){
        printf("Escolha inválida, digite novamente (s/n)");
        scanf("%c", &esc);
        getchar();
      }
    }
  }while(esc == 's');
  if(fazer != 0){ //Aqui realiza a exclusão no caso de o ID existir
    fclose(arquivo);
    arquivo = fopen(nomearq, "rb");
    int x=0;
    while(!feof(arquivo)){ //Passa os registros para um vetor
      ferramenta arq;
      fread(&arq, sizeof(ferramenta), 1, arquivo);
      if(feof(arquivo)){break;};
      vet[x] = arq;
      x++;
    }
    fclose(arquivo);
    arquivo = fopen(nomearq, "wb");
    x=0;
    for(x = 0; x < tam; x++){ //Passa os registros para o arquivo exceto ID que deseja excluir
      ferramenta arq;
      if(vet[x].ID != num){
        arq = vet[x];
        fwrite(&arq, sizeof(ferramenta), 1, arquivo);
        fseek(arquivo, 0, SEEK_END);
      }
    }
    system("clear");
    printf("Ferramenta excluída com sucesso\n\n");
  } else{
    system("clear");
  }
  free(vet);
  fclose(arquivo);
}
void EditarF(){
  char sn;
  int x, y, esc, existe, tam, num, repetido;
  ferramenta *vet;
  do{
    existe = 0;
    x=0;
    tam=0;
    sn = 'n';
    printf("\nDigite o ID da ferramenta que deseja editar: ");
    scanf("%i", &num);
    getchar();
    arquivo = fopen(nomearq, "rb");
    while(!feof(arquivo)){ //Verifica se o ID existe, no caso de o ID existir salva em que posição seu registro está e, conta a quantidade de registros
      ferramenta F;
      fread(&F, sizeof(ferramenta), 1, arquivo);
      if(feof(arquivo)){break;}
      if(F.ID == num){
        existe++;
      }
      if(existe == 0){x++;}
      tam++;
    }
    if(existe == 0){ //No caso de o ID não existir, disponibiliza a opção de escolher outro ID ou cancelar a operação
      printf("O ID digitado não existe, deseja digitar novamente (s/n) ");
      scanf("%c", &sn);
      getchar();
      while(sn != 's' && sn != 'n'){
        printf("Opção inválida, deseja digitar novamente? 's' para sim e 'n' para não: ");
        scanf("%c", &sn);
        getchar();
      }
    }
  }while(sn == 's');
  if(sn == 'n'){
    printf("\n");
  }
  if(existe != 0){ //Realiza se o ID existir
    fseek(arquivo, 0, SEEK_SET);
    vet = malloc(tam * sizeof(ferramenta));
    fread(vet, sizeof(ferramenta), tam, arquivo); //Passa todos os registros para um vetor
    fclose(arquivo);
    system("clear");
    do{
      printf("Escolha uma opção para alterar\n[1] - ID\n[2] - Item\n[3] - Marca\n[4] - Cor\n[5] - Peso\n[6] - Fabricante\n[7] - Ano de fabricação\n[8] - Preço\n[9] - Isolado (s/n)\n[0] - Finalizar\n");
      scanf("%i", &esc);
      getchar();
      switch(esc){
        case 1:
          printf("ID atual: %i\nID novo: ", vet[x].ID);
          scanf("%i", &vet[x].ID);
          getchar();
          do{ //Verifica se o ID digitado existe, bloqueando um cadastro com o mesmo ID e também bloqueia um cadastro com ID menor que 0
            ferramenta test;
            repetido = 0;
            while(vet[x].ID < 0){
              printf("Digite um ID maior ou igual a 0: ");
              scanf("%i", &vet[x].ID);
              getchar();
            }
            repetido=0;
            for(y = 0; y < tam; y++){
              if(y != x){
                if(vet[y].ID == vet[x].ID){
                  repetido++;
                }
              }
            }
            if(repetido > 0){
              printf("O ID digitado já existe, digite um ID diferente por favor: ");
              scanf("%i", &vet[x].ID);
              getchar();
            }
          }while(repetido > 0);
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 2:
          printf("Item atual: %s\nItem novo: ", vet[x].Item);
          gets(vet[x].Item);
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 3:
          printf("Marca atual: %s\nMarca nova: ", vet[x].Marca);
          gets(vet[x].Marca);
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 4:
          printf("Cor atual: %s\nCor novo: ", vet[x].Cor);
          gets(vet[x].Cor);
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 5:
          printf("Peso atual: %.2f\nPeso novo: ", vet[x].Peso);
          scanf("%f", &vet[x].Peso);
          getchar();
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 6:
          printf("Fabricante atual: %s\nFabricante novo: ", vet[x].Fabricante);
          gets(vet[x].Fabricante);
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 7:
          printf("Ano de fabricação atual: %i\nAno de fabricação novo: ", vet[x].Ano_de_Fabricacao);
          scanf("%i", &vet[x].Ano_de_Fabricacao);
          getchar();
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 8:
          printf("Preço atual: R$%.2f\nPreço novo: ", vet[x].Preco);
          scanf("%f", &vet[x].Preco);
          getchar();
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 9:
          printf("Isolado? Atual: %c\nNovo (s/n): ", vet[x].Isolado);
          scanf("%c", &vet[x].Isolado);
          getchar();
          while(vet[x].Isolado != 's' && vet[x].Isolado != 'n'){
            printf("Digite somente com 's' ou 'n': ");
            scanf("%c", &vet[x].Isolado);
            getchar();
          }
          system("clear");
          printf("Atualizado com sucesso\n\n");
        break;
        case 0:
        break;
        default:
          system("clear");
          printf("Escolha inválida\n\n");
      }
    }while(esc != 0);
    arquivo = fopen(nomearq, "wb");
    fwrite(vet, sizeof(ferramenta), tam, arquivo); //Passa os registros para o arquivo
    fclose(arquivo);
    system("clear");
  } else {
    system("clear");
  }
}

main(){
  system("clear");
  int esc;
  setlocale(LC_ALL, "Portuguese");
  do{
    arquivo = fopen(nomearq, "ab+");
    fclose(arquivo);
    printf("Escolha uma das opções\n[1] - Inserir Ferramenta\n[2] - Mostrar todas Ferramentas\n[3] - Editar Ferramenta\n[4] - Excluir Ferramenta\n[0] - Sair\n");
    scanf("%i", &esc);
    getchar();
    if(esc==1){InserirF();}
    else if(esc==2){MostrarF();}
    else if(esc==3){EditarF();}
    else if(esc==4){RemoverF();}
    else if(esc==0){
      printf("\nPrograma finalizado");
      getchar();
    }
    else {
      system("clear");
      printf("Opção inválida\n\n");
    }
  }while(esc!=0);
}