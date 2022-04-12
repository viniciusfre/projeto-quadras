#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUADRA 2
#define MIN_QUADRA 1
#define TURNO 12
#define pD 1
#define uD 30
#define pM 1
#define uM 12

/*
A struct dados reune tudo que é possivel para o nosso programa

Cada quadra tem como valor associado um mês;
O mês tem 30 dias e se associa com 12 horarios;
e nesses horarios é onde podemos marcar a reserva e o nome do cliente;


Dados:
  Quadra[MAX_QUADRA]:
    Mes[12]:
      Dia[30]:
        Horario[TURNO]:
          Nome[50];
          Estado da reserva;

  Primeiro Horario; -> Abreviado para pH
  Ultimo Horario; -> Abreviado para uH

*/

/*
  LEGENDA para chamada da struct

  h - HORARIO
  d - DIA
  m - MES
  q - QUADRA
  pH - PRIMEIRO HORARIO
  uH - ULTIMO HORARIO
  d - DADOS
  pQ - PRIMEIRA QUADRA
  uQ - ULTIMA QUADRA
  pD - PRIMEIRO DIA
  uD - ULTIMO DIA
  pM - PRIMEIRO MES
  uM - ULTIMO MES

*/

struct reserva
{
  char nome[50];
  int reservado; // 1 ou 0
};

struct ldia
{
  struct reserva h[TURNO];
  // 12 horas de funcionamento.
};

struct lmes
{
  struct ldia d[30];
};

struct quadra
{
  struct lmes m[12];
};

struct dados
{
  struct quadra q[MAX_QUADRA]; // QUANTIDADE DE QUADRAS
  int pH;                      // PRIMEIRO HORARIO
  int uH;                      // ULTIMO HORARIO
};

void inserir_dados_txt(int horario, int dia, int mes, int quadra, char nome[])
{ // Pega a informaçãos da reserva e insere no arquivo .txt [Stringhetta]
  FILE *HORARIOS;
  HORARIOS = fopen("horarios.txt", "a");
  fprintf(HORARIOS, "%d %d %d %d %s\n", horario, dia, mes, quadra, nome);
  fclose(HORARIOS);
  return;
}

void ler_dados_txt(struct dados T[])
{ // Incluida na função main, lê todo o arquivo .txt e atribui os valores as variáveis, criando as respectivas reservas que estavam já salvas no arquivo toda vez que o sistema é iniciado. [Stringhetta]
  FILE *HORARIOS;
  HORARIOS = fopen("horarios.txt", "r");
  int horario, dia, mes, quadra;
  char nome[50];
  while (1)
  {
    fscanf(HORARIOS, "%d %d %d %d %[^\n]", &horario, &dia, &mes, &quadra,
           &nome);
    if (feof(HORARIOS))
    {
      break;
    }
    strcpy(T->q[quadra].m[mes].d[dia].h[horario].nome, nome);
    T->q[quadra].m[mes].d[dia].h[horario].reservado = 1;
  }
  fclose(HORARIOS);
  return;
}

void remover_dados_txt(int horario, int dia, int mes, int quadra, char nome[])
{ // Lê todo o arquivo txt, encontra a reserva a qual o usuário quer excluir, ignora a linha, cria um novo arquivo .txt auxiliar com as novas informações menos a que o usuário quer excluir (foi ignorada), remove o arquivo principal e renomeia o aux para o nome principal, excluindo assim a reserva desejada. [Stringhetta]
  FILE *file, *file_2;
  file = fopen("horarios.txt", "r");

  int h, d, m, q;
  char n[50];
  file_2 = fopen("horarios_aux.txt", "w");
  while (1)
  {
    fscanf(file, "%d %d %d %d %[^\n]", &h, &d, &m, &q, n);
    if (feof(file))
    {
      break;
    }
    if (h == horario && d == dia && m == mes && q == quadra && strcmp(n, nome) == 0)
    {
      continue;
    }
    fprintf(file_2, "%d %d %d %d %s\n", h, d, m, q, n);
  }
  fclose(file);
  fclose(file_2);
  if (remove("horarios.txt") != 0)
  {
    printf("Erro ao remover arquivo");
  }
  if (rename("horarios_aux.txt", "horarios.txt") != 0)
  {
    printf("Erro ao renomear arquivo");
  }
  return;
}

void agendar(struct dados T[])
{                //&d => T->q[0].m[mes-1].d[d-1].h[horario].{nome,reservado}
  system("cls"); // Limpa a tela
  int horario, dia, mes, quadra, reserva;
  char nome[50];

  printf("\nBem vindo ao Sistema de Gerenciamento de Quadras \n");
  printf("O Horario de funcionamento do estabelecimento e das %dh as %dh.\n", T->pH, T->uH);
  printf("Caso os valores digitados nao sejam validos, o programa ira repetir a pergunta ate ser um valor valido\n");
  printf("\n");

  do
  {
    printf("Dia (%d-%d): ", pD, uD);
    scanf("%d", &dia); // escolhendo o dia do agendamento
    setbuf(stdin, NULL);
  } while (dia < pD || dia > uD); // condição dos dias

  do
  {
    printf("Mes (%d-%d): ", pM, uM);
    scanf("%d", &mes); // escolhendo o mês do agendamento
    setbuf(stdin, NULL);
  } while (mes < pM || mes > uM); // condição dos meses

  do
  {
    printf("Escolha um horario entre %d e %d.\n", T->pH, T->uH);
    printf("Horario: ");
    scanf("%d", &horario); // escolhendo o horário do agendamento
    setbuf(stdin, NULL);
  } while (horario < T->pH || horario > T->uH); // condição dos horários (verificando se é menor que o horario de abertura ou maior que o horário de fechamento)

  horario = horario - (T->pH);
  // horario = 9
  // v[9] != v[0]
  // horario = 9 - 9 = 1 => v[1];

  setbuf(stdin, NULL);
  printf("Nome: ");
  scanf("%[^\n]s", nome); // lendo o nome do cliente

  for (int i = 0; i < MAX_QUADRA; i++)
  {
    if (T->q[i].m[mes - 1].d[dia - 1].h[horario].reservado != 1)
    {
      strcpy(T->q[i].m[mes - 1].d[dia - 1].h[horario].nome, nome);
      T->q[i].m[mes - 1].d[dia - 1].h[horario].reservado = 1;
      inserir_dados_txt(horario, dia - 1, mes - 1, i, nome);
      system("cls");
      printf("\nMARCADO COM SUCESSO!\n");
      printf("QUADRA %d\n", i + 1);
      printf("NOME: %s\n", nome);
      printf("DATA: %d/%d\n", dia, mes);
      printf("HORARIO: %d\n", horario + (T->pH));
      system("pause");
      system("cls");
      return; // sai da função
    }
  }

  printf("\nO horario escolhido nao esta disponivel em nenhuma quadra.\n");
  printf("Por favor, tente novamente o cadastro.\n");

  system("pause");
  system("cls");

  return;
}
// CANCELAR---------------------------------------
void cancelar(struct dados T[])
{
  system("cls");
  int q, d, m, h;
  char ch;
  char res[30];

  do
  {
    printf("\nOla, para usar a funcao cancelar, e necessario preencher os seguintes parametros\n");
    do
    {
      printf("Quadra:  ");
      scanf("%d", &q);
      setbuf(stdin, NULL);
    } while (q < MIN_QUADRA || q > MAX_QUADRA); // digitando 1 ou maximo de quadras o proximo passo é exibir qual dia do mes será cancelado

    do
    {
      printf("Dia: "); // aqui o dia do mes é escolhido
      scanf("%d", &d);
      setbuf(stdin, NULL);
    } while (d < pD || d > uD); // apos escolher entre o dia 1 e 30, vamos escolher o mes

    do
    {
      printf("Mes: "); // escolhemos por numero qual o mes da reserva
      scanf("%d", &m);
      setbuf(stdin, NULL);
    } while (m < pM || m > uM);
    // apos escolher o mes, entramos no horario a ser cancelado

    do
    {
      printf("Escolha um horario entre %d e %d.\n", T->pH, T->uH);
      // escolhendo entre primeiro horario e último horario
      printf("Horario: ");
      scanf("%d", &h);
      setbuf(stdin, NULL);
    } while (h < T->pH || h > T->uH);
    // aqui, o algoritmo prepara a proxima função, que é exibir o nome do cliente

    setbuf(stdin, NULL);
    printf(" O nome do cliente que deseja cancelar e: %s\n", T->q[q - 1].m[m - 1].d[d - 1].h[h - (T->pH)].nome); // aqui os filtros sao aplicados
    printf("Este e o nome do cliente que voce estava procurando? \tS/N (USAR CAPSLOCK): \n");                    // apos o filtro o cliente e exibido
    setbuf(stdin, NULL);                                                                                         // teclado é limpo
    scanf("%c", &ch);                                                                                            // aqui o algoritomo recebe se o nome do cliente está correto (s) ou não (n)
    setbuf(stdin, NULL);
    if (ch == 'S')
    {                                                                                                       // caso esteja correto (S) a reserva é cancelada
      remover_dados_txt(h - T->pH, d - 1, m - 1, q - 1, T->q[q - 1].m[m - 1].d[d - 1].h[h - (T->pH)].nome); // Remove a reserva do .txt
      strcpy(T->q[q - 1].m[m - 1].d[d - 1].h[h - (T->pH)].nome, "");
      T->q[q - 1].m[m - 1].d[d - 1].h[h - (T->pH)].reservado = 0;
      printf("CANCELADO COM SUCESSO\n");
    }
    printf("\nDeseja cancelar novamente? S/N (USAR CAPSLOCK): \n");
    setbuf(stdin, NULL);
    scanf("%s", res);
    setbuf(stdin, NULL);

    system("cls");
  } while (strcmp(res, "S") == 0);
  /* STRCMP= compara meu teclado com minha string, caso comparação for 'S'
  strcmp(string1, string2); retorna 0 se as duas forem iguais.
Caso nao queira cancelar mais nenhum horario, o algoritmo interrompe (break) a estrutura;*/
  return;
}
// CANCELAR---------------------------------------
void reagendar(struct dados T[])
{
  system("cls");
  char res[30]; // resposta do usuario
  do
  {
    system("cls");
    int q1, m1, d1, h1;                                    // quadra,mes,dia e horario marcado
    int q2, m2, d2, h2;                                    // quadra,mes,dia e horario para trocar
    char nome[50];                                         // Nome agendado
    printf("\nDigite a antiga marcacao do agendamento\n"); // Printa
    do
    {
      printf("Quadra: ");
      // 1
      scanf("%d", &q1);
      // 2
      setbuf(stdin, NULL);
      // 3
    } while (q1 < MIN_QUADRA || q1 > MAX_QUADRA);
    // 4

    // 1 Pergunta a quadra do agendamento anterior,2 escaneia a entrada do usuário, 3 limpa o buffer do teclado e 4 enquanto o usuário não colocar um valor válido, ou seja, entre a primeira e ultima quadra, continua perguntando e escaneando novamente.

    do
    {
      printf("Dia: ");

      scanf("%d", &d1);

      setbuf(stdin, NULL);

    } while (d1 < pD || d1 > uD);

    do
    {
      printf("Mes: ");

      scanf("%d", &m1);

      setbuf(stdin, NULL);

    } while (m1 < pM || m1 > uM);

    do
    {
      printf("Horario: ");

      scanf("%d", &h1);

      setbuf(stdin, NULL);

    } while (h1 < T->pH || h1 > T->uH);

    h1 = h1 - (T->pH);
    // Faz a conversão da entrada de horário do usuário, pois caso essa conversão não ocorra, quando for chamar a struct horario, ela entrará na struct errada. Já que se o usuário desejar por exemplo o horário das 9, e essa conversão não ocorrer, o horário a ser buscado será na verdade o das 18. Isso ocorre pois a struct horário recebe a constante TURNO ou seja, a quantidade de horas entre o primeiro horario e o ultimo. Então o horário das 9 por exemplo, que é o primeiro horário, se refere à primeira struct, ou seja struct horario[0].
    printf("Nome: ");

    scanf("%[^\n]s", nome);

    setbuf(stdin, NULL);

    if (T->q[q1 - 1].m[m1 - 1].d[d1 - 1].h[h1].reservado != 1 || strcmp(T->q[q1 - 1].m[m1 - 1].d[d1 - 1].h[h1].nome, nome) != 0)
    {
      // 1 e 2
      printf("\nHorario de marcacao nao encontrado ou nome incorreto, digite novamente\n");
      system("pause");
      // Pausa até que o usuário aperte qualquer tecla
      system("cls");
      // Limpa o terminal
      return;
      // Volta para o menu
    }
    // Verifica se os dados do usuário batem com um registro de agendamento anterior,se eles não baterem printa uma mensagem de erro, pausa por um instante até que o usuário aperte qualquer tecla limpa a tela e volta para o menu inicial. 1 A primeira parte confere se o agendamento não existe, 2 a segunda confere se os dados não batem, se qualquer uma dessas condições forem satisfeitas, entra no if e realiza as outras ações.
    else
    {
      // Caso o usuário digite dados de um agendamento que realmente estava reservado e com seus dados corretos, pergunta agora os dados para o reagendamento
      do
      {
        printf("\nAgora digite a marcacao desejada\n");
        do
        {
          printf("Quadra: ");

          scanf("%d", &q2);

          setbuf(stdin, NULL);

        } while (q2 < MIN_QUADRA || q2 > MAX_QUADRA);

        do
        {
          printf("Dia: ");

          scanf("%d", &d2);

          setbuf(stdin, NULL);

        } while (d2 < pD || d2 > uD);

        do
        {
          printf("Mes: ");

          scanf("%d", &m2);

          setbuf(stdin, NULL);

        } while (m2 < pM || m2 > uM);

        do
        {
          printf("Horario: ");

          scanf("%d", &h2);

          setbuf(stdin, NULL);

        } while (h2 < T->pH || h2 > T->uH);

        h2 = h2 - (T->pH);

        if (T->q[q2 - 1].m[m2 - 1].d[d2 - 1].h[h2].reservado != 1)
        {
          // 1
          strcpy(T->q[q1 - 1].m[m1 - 1].d[d1 - 1].h[h1].nome, "");
          // 2
          T->q[q1 - 1].m[m1 - 1].d[d1 - 1].h[h1].reservado = 0;
          // 3
          strcpy(T->q[q2 - 1].m[m2 - 1].d[d2 - 1].h[h2].nome, nome);
          // 4
          T->q[q2 - 1].m[m2 - 1].d[d2 - 1].h[h2].reservado = 1;
          // 5
          remover_dados_txt(h1, d1 - 1, m1 - 1, q1 - 1, nome);
          // remove a antiga marcação do txt
          inserir_dados_txt(h2, d2 - 1, m2 - 1, q2 - 1, nome);
          // adiciona a nova marcação do txt
          printf("\nReagendamento feito com sucesso\n");
          // 6
          system("pause");
          // 7
          system("cls");
          // 8
          break;
          // 9
        }
        // 1 Verifica se a data e horarios de reagendamento estão livres, se estiver, entra no if e 2 apaga o antigo agendamento do usuário, colocando um espaço em branco no registro. 3 Após isso, define a variavel de reserva naquela data e horario como 0, para também dar como apagado aquele registro e livre para outra pessoa. 4 Depois, faz a reserva da pessoa, colocando seus dados na data e horario desejados, e 5 mudando a variavel de reserva para 1, indicando que agora se trata de uma marcação reservada.Por fim, 6 printa uma mensagem de confirmação, 7 pausa até que o usuário digite uma tecla e 8 limpa tela e 9 volta para o menu, quebrando o laço de perguntas.
        else
          printf("\nHorario de marcacao ja esta reservado.\n");
        // Caso o usuário digite uma marcação que ja esta reservada, imprime uma mensagem de erro e retorna ao inicio do ciclo, pedindo novamente os dados, quadra, dia, mes e horário, até que coloque uma marcação de reagendamento válida.
      } while (1);
    }
    printf("\nDeseja reagendar novamente? S/N (USAR CAPSLOCK): \n");
    setbuf(stdin, NULL);
    scanf("%s", res);
    setbuf(stdin, NULL);
    system("cls");
  } while (strcmp(res, "S") == 0);
  return;
}

void mostrar_todos(struct dados T[])
{
  // LIMPA A TELA DO TERMINAL, tirando então o menu que estava anteriormente
  system("cls");

  int q, m, d, h;
  // Pelo MAX_QUADRA (Quantidade de quadras definidas.) ele roda de 0 a MAX_QUADRA-1.
  for (q = 0; q < MAX_QUADRA; q++)
  {
    printf("\nQUADRA %d\n", q + 1);
    for (m = 0; m < 12; m++)
    {
      // Os meses do ano não se alteram, por isso 12
      for (d = 0; d < 30; d++)
      {
        // Trabalhamos com todos os meses do ano tendo 30 dias.
        for (h = 0; h < TURNO; h++)
        {
          // O valor de TURNO é o horario de funcionamento do estabelecimento, que no momento é de 12h, mas poderia ser aumentado.
          // So seria necessário tambem trocar o valor pH e uH (primeio Horario e ultimo Horario)
          if (T->q[q].m[m].d[d].h[h].reservado == 1)
          {
            // Aqui se chega se o valor de reservado na struct é 1. Pois caso fosse 0, o horario estaria vago.
            printf("NOME: %s\t DATA: %d/%d\t HORARIO:%dh\n", T->q[q].m[m].d[d].h[h].nome, d + 1, m + 1, h + (T->pH));
          }
        }
      }
    }
  }
  // Se pausa o sistema para o usuario poder ler com calma o que é printado, e apos isso é limpado a tela para voltar ao menu.
  system("pause");
  system("cls");
  return;
}

void mostrar_data(struct dados T[])
{
  system("cls");
  int mes, dia, i, j;
  char res[30];

  do
  {
    system("cls");
    printf("\nOla, essa funcao mostra os agendamentos do dia!\n");
    // Solicita o Dia até que um valor válido (1 até 30) seja passado pelo usuário
    do
    {
      setbuf(stdin, NULL);
      printf("Dia: ");
      scanf("%d", &dia);
    } while (dia < pD || dia > uD);

    // Solicita o Mes até que um valor válido (1 até 12) seja passado pelo usuário
    do
    {
      setbuf(stdin, NULL);
      printf("Mes: ");
      scanf("%d", &mes);
    } while (mes < pM || mes > uM);
    system("cls");
    printf("\nDIAS RESERVADOS\n");

    // Loop que percorre entre as duas quadras
    for (j = 0; j < MAX_QUADRA; j++)
    {
      printf("\nQUADRA %d\n", j + 1);
      for (i = 0; i < TURNO; i++)
      {
        if (T->q[j].m[mes - 1].d[dia - 1].h[i].reservado == 1)
        {
          printf("\nNOME: %s\n", T->q[j].m[mes - 1].d[dia - 1].h[i].nome);
          printf("HORARIO: %d\n", i + T->pH);
          printf("\n");
        }
      }
    }
    printf("\nDeseja consultar novamente? S/N (USAR CAPSLOCK): \n");
    setbuf(stdin, NULL);
    scanf("%s", res);
    setbuf(stdin, NULL);
    system("cls");
  } while (strcmp(res, "S") == 0); // Loop responsavel por fazer a consulta novamente

  system("cls");
  return;
}

void mostrar_cliente(struct dados T[])
{
  int q, m, d, h, cont = 0;
  char nome[50];
  char res[30];

  system("cls");
  printf("\nPara o uso desta funcao, escreva apenas o nome do cliente.\nAtente-se para escreve-lo da mesma forma como foi cadastrado.\n");

  do
  {
    setbuf(stdin, NULL); // Limpa o buf
    printf("Digite o nome do cliente: \n");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", nome); // Pede inserção do nome
    setbuf(stdin, NULL);
    system("cls");
    printf("Agendamentos do Cliente %s \n", nome);
    for (q = 0; q < MAX_QUADRA; q++)
    { // Loop que percorre entre as duas quadras
      for (m = 0; m < 12; m++)
      { // Loop que percorre entre todos os meses
        for (d = 0; d < 30; d++)
        { // Loop que percorre entre todos os dias
          for (h = 0; h < TURNO; h++)
          { // Loop que percorre entre todas as horas do TURNO

            if (strcmp(T->q[q].m[m].d[d].h[h].nome, nome) == 0)
            {                                                                                    // Compara o nome que acaba de ser recebido pelo usuario com o nome armarzenado nos dados
              printf("QUADRA: %d\t DATA: %d/%d\t HORARIO:%d\n", q + 1, d + 1, m + 1, h + T->pH); // Mostra o agendamento completo de X cliente
              cont += 1;
            }
          }
        }
      }
    }

    printf("\nDeseja consultar novamente? S/N (USAR CAPSLOCK): \n");
    setbuf(stdin, NULL);
    scanf("%s", res);
    setbuf(stdin, NULL);
    system("cls");
  } while (strcmp(res, "S") == 0); // Loop responsavel por fazer a consulta novamente

  if (cont == 0)
  {
    system("cls");
    printf("NENHUM HORARIO FOI ENCONTRADO \n");
    system("pause");
  }

  system("cls");
  return;
}

void mostrar_vagos(struct dados T[])
{
  system("cls");
  int mes, dia, i, j;
  char res[30];
  printf("\nOla, essa funcao mostra os horarios vagos na data desejada!\n");
  do
  {
    system("cls");
    do
    {
      setbuf(stdin, NULL);
      printf("Dia: ");
      scanf("%d", &dia);
    } while (dia < pD || dia > uD);

    do
    {
      setbuf(stdin, NULL);
      printf("Mes: ");
      scanf("%d", &mes);
    } while (mes < pM || mes > uM);

    // Laço para rodar os horarios livres, roda de 0 a MAX_QUADRAS e 0 a TURNO.
    // Mostrando assim todos horarios possiveis em todas as quadras possiveis, na data entregue pelo usuario.
    printf("\nHORARIOS LIVRES\n");
    for (j = 0; j < MAX_QUADRA; j++)
    {
      printf("\nQUADRA %d\n", j + 1);
      for (i = 0; i < TURNO; i++)
      {
        if (T->q[j].m[mes - 1].d[dia - 1].h[i].reservado == 0)
        {
          // Caso o horario esteja agendado esse IF sera ignorado e assim não sera printado o valor marcado, apenas os vagos
          printf("HORARIO: %d\n", i + T->pH);
        }
      }
    }
    printf("\nDeseja consultar novamente? S/N (USAR CAPSLOCK): \n");
    setbuf(stdin, NULL);
    scanf("%s", res);
    setbuf(stdin, NULL);
    system("cls");

  } while (strcmp(res, "S") == 0);
  // S ou N para continuar
  system("cls");
  return;
}
void mostrar_mes(struct dados T[])
{
  system("cls");
  int mes, i, j, k;
  char res[30];

  do
  {
    system("cls");
    printf("\nOla, essa funcao mostra os agendamentos do mes!\n");
    // Solicita o Dia até que um valor válido (1 até 30) seja passado pelo usuário
    // Solicita o Mes até que um valor válido (1 até 12) seja passado pelo usuário
    do
    {
      setbuf(stdin, NULL);
      printf("Mes: ");
      scanf("%d", &mes);
    } while (mes < pM || mes > uM);
    system("cls");
    printf("\nDIAS RESERVADOS\n");

    // Loop que percorre entre as duas quadras
    for (j = 0; j < MAX_QUADRA; j++)
    {
      printf("\nQUADRA %d\n", j + 1);
      for (k = 0; k < uD; k++)
      {
        for (i = 0; i < TURNO; i++)
        {
          if (T->q[j].m[mes - 1].d[k].h[i].reservado == 1)
          {
            printf("\nNOME: %s\n", T->q[j].m[mes - 1].d[k].h[i].nome);
            printf("HORARIO: %d\n", i + T->pH);
            printf("Data: %d/%d\n", k+1,mes);
            printf("\n");
          }
        }
      }
    }

    printf("\nDeseja consultar novamente? S/N (USAR CAPSLOCK): \n");
    setbuf(stdin, NULL);
    scanf("%s", res);
    setbuf(stdin, NULL);
    system("cls");
  } while (strcmp(res, "S") == 0); // Loop responsavel por fazer a consulta novamente

  system("cls");
  return;
}
void menu()
{
  int opcao;
  struct dados d = {0};
  ler_dados_txt(&d);
  d.pH = 9;
  d.uH = 20;

  while (1)
  {
    printf("\nBem vindo ao Sistema de Gerenciameto de Quadras");
    printf("\n1- Agendar");
    printf("\n2- Cancelar Agendamento");
    printf("\n3- Reagendar");
    printf("\n4- Mostrar Todos Agendamentos");
    printf("\n5- Mostrar Agendamentos por Data");
    printf("\n6- Mostrar Agendamento do cliente X");
    printf("\n7- Mostrar Todos Horarios Vagos por Data");
    printf("\n8- Sair");
    printf("\n9- Mostrar Agendamentos por Mes");
    printf("\nDigite opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1)
      agendar(&d);
    if (opcao == 2)
      cancelar(&d);
    if (opcao == 3)
      reagendar(&d);
    if (opcao == 4)
      mostrar_todos(&d);
    if (opcao == 5)
      mostrar_data(&d);
    if (opcao == 6)
      mostrar_cliente(&d);
    if (opcao == 7)
      mostrar_vagos(&d);
    if (opcao == 8)
      return;
    if (opcao == 9)
      mostrar_mes(&d);
  }
}

int main()
{
  menu();
}