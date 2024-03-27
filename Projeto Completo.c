#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_DIAS 31
#define VERDE "\033[0;32m"
#define AMARELO "\033[0;33m"
#define VERMELHO "\033[0;31m"
#define RESET "\033[0m"
#define AZULCL "\x1B[36m"
#define AZULESC "\x1B[34m"

int dia;
int meses;
int horario;
int codigoMatricula;

typedef struct
{
    int codigoMatricula;
    char nome[50];
    char curso[50];
    char turma[50];
} Aluno;

bool verificarLogin(int codigoMatricula, int senha, Aluno *alunos, int numAlunos)
{
    for (int i = 0; i < numAlunos; i++)
    {
        if (alunos[i].codigoMatricula == codigoMatricula && alunos[i].codigoMatricula % 10000 == senha)
        {
            return true; // Login válido
        }
    }
    return false; // Login inválido
}

bool validarEmail(const char* email)
{
    // Verifica se o email possui um @
    const char* arroba = strchr(email, '@');
    if (arroba == NULL)
    {
        return false;
    }

    // Verifica se o email possui pelo menos um caractere antes do @
    if (arroba == email)
    {
        return false;
    }

    // Verifica se o email possui pelo menos um caractere depois do @
    if (*(arroba + 1) == '\0')
    {
        return false;
    }

    // Verifica se o email possui um ponto após o @
    const char* ponto = strchr(arroba, '.');
    if (ponto == NULL)
    {
        return false;
    }

    // Verifica se o ponto não é o último caractere do email
    if (*(ponto + 1) == '\0')
    {
        return false;
    }

    return true;
}

void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Descarta os caracteres do buffer de entrada
    }
}

void adicionarAluno(Aluno *alunos, int *numAlunos)
{
    int codigoMatricula;
    char nome[50];
    char curso[50];
    char turma[50];

    system("cls");
    printf("Digite o código de matrícula do aluno: ");
    scanf("%d", &codigoMatricula);

    printf("Digite o nome do aluno: ");
    scanf(" %[^\n]s", nome);
    limparBufferEntrada();

    printf("Digite o curso do aluno: ");
    scanf(" %[^\n]s", curso);
    limparBufferEntrada();

    printf("Digite a turma do aluno: ");
    scanf(" %[^\n]s", turma);
    limparBufferEntrada();

    alunos[*numAlunos].codigoMatricula = codigoMatricula;
    strcpy(alunos[*numAlunos].nome, nome);
    strcpy(alunos[*numAlunos].curso, curso);
    strcpy(alunos[*numAlunos].turma, turma);

    (*numAlunos)++;

    system("cls");
    printf(VERDE "\nAluno adicionado com sucesso!\n\n" RESET);
    system("pause");

    // Salva os dados em um arquivo
    FILE *arquivo = fopen("alunos.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(arquivo, "%d,%s,%s,%s\n", codigoMatricula, nome, curso, turma);

    fclose(arquivo);
}

void exibirMenu()
{
    system("cls");
    printf(AZULCL "===== Menu de Administração =====\n" RESET);
    printf("| 1. Adicionar novo aluno       |\n");
    printf("| 2. Verificar dados do aluno   |\n");
    printf("| 3. Verificar requerimentos    |\n");
    printf("| 4. Excluir aluno              |\n");
    printf("| 0. Sair                       |\n");
    printf(AZULCL "=================================\n" RESET);
    printf("Digite sua opção: ");
}

void exibirMenuAluno()
{
    system("cls");
    printf(AZULCL "======= Menu do Aluno =======\n" RESET);
    printf("| 1. Requerimento de aulas  |\n");
    printf("| 2. Exibir Calendário      |\n");
    printf("| 0. Sair                   |\n");
    printf(AZULCL "=============================\n" RESET);
    printf("Digite sua opção: ");
}

void exibirCalendario(int meses, int ano, int dias_no_mes, int dia_da_semana)
{
    printf("\n=============================\n");
    printf("   Calendário: %02d/%04d\n", meses, ano);
    printf("=============================\n");
    printf(" Dom Seg Ter Qua Qui Sex Sab\n");

    for (int i = 0; i < dia_da_semana; i++)
    {
        printf("    ");
    }

    for (int i = 0; i <= dias_no_mes; i++)
    {
        if(i != 0)
        {
            if ((dia_da_semana + i - 1) % 7 == 0 || (dia_da_semana + i - 1) % 7 == 6)
            {
                printf(VERMELHO);
            }
            printf(" %3d", i);
            printf(RESET);
            if(meses == 7 || meses == 12)
            {
                printf(AMARELO);
            }
            if ((dia_da_semana + i) % 7 == 0)
            {
                printf("\n");
            }
        }
        else if(i == 0 && ((meses == 7)|| meses == 12))
        {
            printf(AMARELO);
        }
    }

    printf(RESET "\n=============================\n\n");
}

void requerimentoAulas()
{
    int codigoMatricula;
    char nome[50];
    int ano, dias_no_mes, dia_da_semana, i, dia;
    int calendario[MAX_DIAS][4]; // Matriz para armazenar os horários das aulas
    char email[50];
    int aula;

    system("cls");
    printf(AMARELO "\nInformamos que você só poderá agendar a reposição de aula durante o período de um mês referente a falta.\n\n" RESET);
    system("pause");

    system("cls");
    printf("\nCalendário dos dias disponíveis para reposição de aula.\n\n");

    do
    {
        printf("Digite o ano: ");
        scanf("%d", &ano);

        if (ano <= 0)
        {
            printf(VERMELHO "Ano inválido. Por favor, digite um valor maior que 0.\n" RESET);
        }
    }
    while (ano <= 0);

    do
    {
        printf("Digite o mês (1 a 12): ");
        scanf("%d", &meses);

        if (meses < 1 || meses > 12)
        {
            printf(VERMELHO "Mês inválido. Por favor, digite um valor de 1 a 12.\n" RESET);
        }
    }
    while (meses < 1 || meses > 12);

    if (meses == 2)
    {
        if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)
        {
            dias_no_mes = 29;
        }
        else
        {
            dias_no_mes = 28;
        }
    }
    else if (meses == 4 || meses == 6 || meses == 9 || meses == 11)
    {
        dias_no_mes = 30;
    }
    else
    {
        dias_no_mes = 31;
    }

    int a = (14 - meses) / 12;
    int y = ano - a;
    int m = meses + 12 * a - 2;
    dia_da_semana = (1 + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7;

    system("cls");
    exibirCalendario(meses, ano, dias_no_mes, dia_da_semana);

    for (int i = 0; i < MAX_DIAS; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            calendario[i][j] = 0;
        }
    }

    printf("*Menu\n");

    int contador = 0;

    do
    {
        printf("\nDigite o dia disponível no calendário acima que deseja realizar a aula (ou zero (0) para sair): ");
        scanf("%d", &dia);

        if (dia == 0)
        {
            break;
        }

        if (meses == 7)
        {
            printf(VERMELHO "\nJulho é período de férias. Não é possível agendar reposições de aula neste mês.\n" RESET);
            continue;
        }

        if (dia < 1 || dia > dias_no_mes)
        {
            printf(VERMELHO "\nDia inválido. Por favor, escolha um dia válido ou zero (0) para sair.\n" RESET);
            continue;
        }

        bool horarioDisponivel = false;
        for (int j = 0; j < 4; j++)
        {
            if (calendario[dia - 1][j] == 0)
            {
                horarioDisponivel = true;
                break;
            }
        }

        if (!horarioDisponivel)
        {
            printf(VERMELHO "\nNão há mais horários disponíveis para este dia. Por favor, escolha outra data.\n" RESET);
            continue;
        }

        int diaDaSemanaSelecionado = (dia_da_semana + dia - 1) % 7;
        if (diaDaSemanaSelecionado == 0 || diaDaSemanaSelecionado == 6)
        {
            printf(VERMELHO "\nVocê só pode marcar requerimento de aulas em dias úteis. Por favor, escolha outra data.\n" RESET);
            continue;
        }

        printf("\nHorários disponíveis para o dia %d/%d:\n", dia, meses);
        printf("1- 7:00 AM\n");
        printf("2- 8:00 AM\n");
        printf("3- 9:00 AM\n");
        printf("4- 10:00 AM\n");

        printf("Digite o horário desejado (1 a 4): ");
        scanf("%d", &aula);

        if (aula >= 1 && aula <= 4)
        {
            if (calendario[dia - 1][aula - 1] == 0)
            {
                calendario[dia - 1][aula - 1] = aula;
                printf(AMARELO "\nInformamos que a aula foi pré-agendada com sucesso para o dia %d/%d às %d:00 AM.\n\n" RESET, dia, meses, aula + 6);

                contador++;

                system("pause");

                system("cls");
                exibirCalendario(meses, ano, dias_no_mes, dia_da_semana);
            }
            else
            {
                printf(VERMELHO "\nEste horário já está ocupado. Por favor, escolha outro horário.\n" RESET);
            }
        }
        else
        {
            printf(VERMELHO "\nOpção inválida. Por favor, escolha um número entre 1 e 4.\n" RESET);
        }
    }
    while (dia != 0);

    if (contador > 0)
    {
        system("cls");
        printf("Digite seu código de matrícula para confirmação: ");
        scanf("%d", &codigoMatricula);

        printf("Digite seu nome para a confirmação: ");
        scanf("%s", nome);
        limparBufferEntrada();
        system("pause");

        system("cls");
        bool emailValido = false;
        do
        {
            printf("Para receber a confirmação da reposição, digite o seu e-mail: ");
            scanf("%s", email);
            limparBufferEntrada();

            if (validarEmail(email))
            {
                emailValido = true;
            }
            else
            {
                printf(VERMELHO "E-mail inválido. Por favor, digite um e-mail válido.\n"RESET);
            }
        }
        while (!emailValido);

        system("pause");

        FILE *arquivo = fopen("requerimentos.txt", "a");

        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            return;
        }

        for (int i = 0; i < MAX_DIAS; ++i)
        {
            for (int j = 0; j < 4; j++)
            {
                if (calendario[i][j] != 0)
                {
                    dia = i + 1;
                    horario = calendario[i][j]+ 6;
                    fprintf(arquivo, "%d,%s,%d,%d,%d,%s\n",codigoMatricula, nome, dia, meses, horario, email);
                }
            }
        }

        fclose(arquivo);
    }
}

void verificarDados(Aluno *alunos, int numAlunos)
{
    int codigoMatricula;

    system("cls");
    printf("Digite o código de matrícula do aluno: ");
    scanf("%d", &codigoMatricula);


    bool encontrado = false;

    for (int i = 0; i < numAlunos; i++)
    {
        system("cls");
        if (alunos[i].codigoMatricula == codigoMatricula)
        {
            printf(AZULCL "======== Dados do Aluno ========\n" RESET);
            printf("Código de Matrícula: %d\n", alunos[i].codigoMatricula);
            printf("Nome: %s\n", alunos[i].nome);
            printf("Curso: %s\n", alunos[i].curso);
            printf("Turma: %s\n", alunos[i].turma);
            printf(AZULCL "================================\n" RESET);
            system("pause");
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        system("cls");
        printf(VERMELHO "Aluno não encontrado!\n" RESET);
        system("pause");
    }
}

void verificarRequerimento()
{
    FILE* arquivo = fopen("requerimentos.txt", "r");
    int codigoMatAnte = 0, codigoMatricula, dia, meses, horario;

    if (arquivo != NULL)
    {
        if(codigoMatAnte != codigoMatricula)
        {
            char nome[50], email[50];

            while (fscanf(arquivo, "%d,%[^,],%d,%d,%d,%[^\n]", &codigoMatricula, nome, &dia, &meses, &horario, email) == 6)
            {
                if(codigoMatAnte != codigoMatricula)
                {
                    printf("\nMatricula: %d\n", codigoMatricula);
                    printf("Nome: %s\n", nome);
                    printf("Email: %s\n", email);
                    printf(AMARELO "Dia %d/%d às %d:00 AM.\n" RESET, dia, meses, horario);
                }
                else
                {
                    printf(AMARELO "Dia %d/%d às %d:00 AM.\n" RESET, dia, meses, horario);
                }
                codigoMatAnte = codigoMatricula;
            }
            fclose(arquivo);
        }
        else
        {
            printf("O arquivo requerimentos.txt não existe ou não pode ser aberto.\n");
        }
    }
    printf("\n");
}

void excluirAluno(int codigoMatricula)
{

    FILE *arquivo = fopen("Alunos.txt", "r+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    Aluno aluno;
    int encontrado = 0;

    // Procurar o aluno com o código de matrícula especificado
    while (fscanf(arquivo, "%d,%[^,],%[^,],%[^\n]", &aluno.codigoMatricula, aluno.nome, aluno.curso, aluno.turma) == 4)
    {
        if (aluno.codigoMatricula == codigoMatricula)
        {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        printf("Aluno não encontrado!\n");
        fclose(arquivo);
        return;
    }

    // Obter a posição atual do arquivo
    long posicaoAtual = ftell(arquivo);
    if (posicaoAtual == -1)
    {
        printf("Erro ao obter a posição do arquivo!\n");
        fclose(arquivo);
        return;
    }

    // Obter o tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    if (tamanhoArquivo == -1)
    {
        printf("Erro ao obter o tamanho do arquivo!\n");
        fclose(arquivo);
        return;
    }

    // Calcular o tamanho dos dados a serem excluídos
    long tamanhoDadosExcluir = strlen(aluno.nome) + strlen(aluno.curso) + strlen(aluno.turma) + 11;

    // Calcular o tamanho dos dados restantes no arquivo
    long tamanhoDadosRestantes = tamanhoArquivo - posicaoAtual - tamanhoDadosExcluir;

    // Se houver dados restantes no arquivo, mova-os para a posição correta
    if (tamanhoDadosRestantes > 0)
    {
        char* dadosRestantes = malloc(tamanhoDadosRestantes);
        if (dadosRestantes == NULL)
        {
            printf("Erro ao alocar memória!\n");
            fclose(arquivo);
            return;
        }

        // Ler os dados restantes do arquivo
        fseek(arquivo, posicaoAtual + tamanhoDadosExcluir, SEEK_SET);
        fread(dadosRestantes, tamanhoDadosRestantes, 1, arquivo);

        // Voltar à posição correta no arquivo
        fseek(arquivo, posicaoAtual, SEEK_SET);

        // Escrever os dados restantes no arquivo
        fwrite(dadosRestantes, tamanhoDadosRestantes, 1, arquivo);

        free(dadosRestantes);
    }

    // Redimensionar o arquivo, excluindo os dados do aluno indesejado
    int resultado = ftruncate(fileno(arquivo), tamanhoArquivo - tamanhoDadosExcluir);
    if (resultado == -1)
    {
        printf("Erro ao redimensionar o arquivo!\n");
        fclose(arquivo);
        return;
    }

    printf("Aluno excluído com sucesso!\n");

    fclose(arquivo);
}

void imprimirCalendario()
{
    int ano, meses, dias_no_mes, dia_da_semana;
    int calendario[MAX_DIAS][4]; // Matriz para armazenar os horários das aulas

    printf("Digite o ano: ");
    scanf("%d", &ano);

    printf("A partir de que mês deseja exibir? ");
    scanf("%d", &meses);

    for (meses; meses <= 12; meses++)
    {
        if (meses == 2)
        {
            if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)
            {
                dias_no_mes = 29;
            }
            else
            {
                dias_no_mes = 28;
            }
        }
        else if (meses == 4 || meses == 6 || meses == 9 || meses == 11)
        {
            dias_no_mes = 30;
        }
        else
        {
            dias_no_mes = 31;
        }

        int a = (14 - meses) / 12;
        int y = ano - a;
        int m = meses + 12 * a - 2;
        dia_da_semana = (1 + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7;

        exibirCalendario(meses, ano, dias_no_mes, dia_da_semana);
    }
}

int main()
{
    system("color 00");
    setlocale(LC_ALL,"portuguese");
    char usuario[50];
    char senha[50];
    int ano;
    int loop = 1;

    int isAdmin = 0;
    do
    {
        printf(AZULCL "Bem-vindo ao sistema da MICROLINS!\n\n" RESET);
        printf("==== MENU INÍCIO ====\n");
        printf("1-Administrador\n");
        printf("2-Aluno\n");
        printf("=====================\n");
        printf("Digite sua opção: ");

        int option;
        scanf("%d", &option);
        system("cls");

        if (option == 1)
        {
            printf("\nLogin como administrador\n");
            printf("Usuário: ");
            scanf("%s", usuario);
            limparBufferEntrada();
            printf("Senha: ");
            scanf("%s", senha);
            limparBufferEntrada();

            if (strcmp(usuario, "secadm") == 0 && strcmp(senha, "1234") == 0)
            {
                isAdmin = 1;
                printf(VERDE "\nLogin de administrador bem-sucedido!\n\n" RESET);
                system("pause");

                int opcao;
                Aluno alunos[100];
                int numAlunos = 0;

                // Carrega os dados dos alunos a partir do arquivo
                FILE *arquivo = fopen("alunos.txt", "r");
                if (arquivo != NULL)
                {
                    while (!feof(arquivo))
                    {
                        int codigoMatricula;
                        char nome[50];
                        char curso[50];
                        char turma[50];

                        fscanf(arquivo, "%d,%[^,],%[^,],%[^\n]\n", &codigoMatricula, nome, curso, turma);

                        alunos[numAlunos].codigoMatricula = codigoMatricula;
                        strcpy(alunos[numAlunos].nome, nome);
                        strcpy(alunos[numAlunos].curso, curso);
                        strcpy(alunos[numAlunos].turma, turma);

                        numAlunos++;
                    }
                    fclose(arquivo);
                }
                else
                {
                    printf("O arquivo alunos.txt não existe ou não pode ser aberto.\n");
                }

                do
                {
                    exibirMenu();
                    scanf("%d", &opcao);

                    switch (opcao)
                    {
                    case 1:
                        adicionarAluno(alunos, &numAlunos);
                        break;
                    case 2:
                        verificarDados(alunos, numAlunos);
                        break;
                    case 3:
                        verificarRequerimento();
                        system("pause");
                        break;
                    case 4:
                        system("cls");
                        printf("Digite o codigo de matricula do aluno que deseja excluir: ");
                        scanf("%d", &codigoMatricula);
                        excluirAluno(codigoMatricula);
                        system("pause");
                        break;
                    case 0:
                        printf("\nEncerrando o programa.\n");
                        break;
                    default:
                        printf("\nOpção inválida.\n");
                        system("pause");
                        break;
                    }
                }
                while (opcao != 0);
            }
            else
            {
                printf(VERMELHO "\nLogin de administrador falhou. Acesso negado.\n\n" RESET);
                system("pause");
            }
        }
        else if (option == 2)
        {

            // Lógica para a opção de aluno
            int codigoMatricula;
            int senha;

            printf("\nLogin como aluno\n");
            printf("Digite o código de matrícula do aluno: ");
            scanf("%d", &codigoMatricula);
            printf("Digite os últimos 4 dígitos do código de matrícula: ");
            scanf("%d", &senha);

            Aluno alunos[100];
            int numAlunos = 0;

            // Carrega os dados dos alunos a partir do arquivo
            FILE *arquivo = fopen("alunos.txt", "r");
            if (arquivo != NULL)
            {
                while (!feof(arquivo))
                {
                    int codigoMatricula;
                    char nome[50];
                    char curso[50];
                    char turma[50];

                    fscanf(arquivo, "%d,%[^,],%[^,],%[^\n]\n", &codigoMatricula, nome, curso, turma);

                    alunos[numAlunos].codigoMatricula = codigoMatricula;
                    strcpy(alunos[numAlunos].nome, nome);
                    strcpy(alunos[numAlunos].curso, curso);
                    strcpy(alunos[numAlunos].turma, turma);

                    numAlunos++;
                }
                fclose(arquivo);
            }

            if (verificarLogin(codigoMatricula, senha, alunos, numAlunos))
            {
                printf(VERDE "\nLogin de aluno bem-sucedido!\n\n" RESET);
                system("pause");

                int opcao;
                do
                {
                    // Exibir Menu do aluno
                    exibirMenuAluno();
                    scanf("%d", &opcao);

                    switch (opcao)
                    {
                    case 1:
                        system("cls");
                        requerimentoAulas();
                        break;
                    case 2:
                        system("cls");
                        imprimirCalendario();
                        system("pause");
                        break;
                    case 0:
                        printf("Encerrando o programa.\n");
                        break;
                    default:
                        printf("Opção inválida.\n");
                        system("pause");
                        break;
                    }
                }
                while (opcao != 0);

            }
            else
            {
                printf(VERMELHO "\nLogin de aluno falhou. Acesso negado.\n" RESET);
                system("pause");
            }
        }
        else
        {
            printf(VERMELHO "\nOpção inválida. Encerrando o programa.\n" RESET);
            system("pause");
        }
        // Verifica se o usuário deseja reiniciar o processo de login
        if (loop)
        {
            int reiniciar;
            printf("\nDeseja reiniciar o login? (1-Sim / 0-Não): ");
            scanf("%d", &reiniciar);
            system("cls");
            loop = (reiniciar == 1);  // Define o valor de loop com base na resposta do usuário
        }
    }
    while (loop);

}
