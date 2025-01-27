

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>  // Para a função sleep no Linux/Unix
#include <conio.h>   // Para kbhit() e getch() no Windows

#define ALTURA 19
#define LARGURA 30
#define MAX_COBRA (ALTURA * LARGURA)

// Definindo as constantes para as direções
#define DIREITA 1
#define ESQUERDA 2
#define CIMA 3
#define BAIXO 4

void exibirMenuPrincipal();
void exibirMenuDificuldade(int *dificuldade);
void comecarJogo(int dificuldade);
void exibirRecord();
void imprimirTabuleiro(char tabuleiro[ALTURA][LARGURA]);
void inicializarTabuleiro(char tabuleiro[ALTURA][LARGURA]);
void moverCobra(char tabuleiro[ALTURA][LARGURA], int cobraX[], int cobraY[], int *tamanhoCobra, int *objetivoX, int *objetivoY, int *pontuacao, bool *jogoAtivo, int *direcao);
void limparTela();
void gerarObjetivo(char tabuleiro[ALTURA][LARGURA], int *objetivoX, int *objetivoY);

// Cores ANSI
#define RESET "\033[0m"
#define AZUL "\033[34m"
#define VERMELHO "\033[31m"

// Variáveis globais
char nomeJogador[50];
int recordePontos = 0;
char recordeJogador[50];

int main() {
    int escolha;
    int dificuldade = 1;  // Padrão para Fácil

    printf("Digite seu nome: ");
    scanf("%s", nomeJogador);  // Nome do jogador

    do {
        exibirMenuPrincipal();  // Exibe o menu principal
        printf("\nEscolha uma opcao: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                exibirMenuDificuldade(&dificuldade);  // Escolha da dificuldade
                comecarJogo(dificuldade);  // Inicia o jogo com a dificuldade escolhida
                break;
            case 2:
                exibirRecord();
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (escolha != 0);

    return 0;
}

void exibirMenuPrincipal() {
    limparTela();
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|        ____                          |\n");
    printf("|       / , ,\\                        |\n");
    printf("|   >----     //                       |\n");
    printf("|       \\   //                        |\n");
    printf("|        || ||                         |\n");
    printf("|        || ||                         |\n");
    printf("|        || ||                   ?     |\n");
    printf("|        || ||                  //     |\n");
    printf("|        \\_/|                 //      |\n");
    printf("|         \\\\________________//       |\n");
    printf("|          \\\\________________/        |\n");
    printf("|                                      |\n");
    printf("|                   Snake              |\n");
    printf("========================================\n");
    printf("1. Comecar o jogo\n");
    printf("2. Record\n");
    printf("0. Sair\n");
    printf("===================\n");
}

void exibirMenuDificuldade(int *dificuldade) {
    limparTela();
    printf("Escolha a dificuldade:\n");
    printf("1. Facil\n");
    printf("2. Medio\n");
    printf("3. Dificil\n");
    printf("Escolha: ");
    scanf("%d", dificuldade);
}

void comecarJogo(int dificuldade) {
    char tabuleiro[ALTURA][LARGURA];
    int cobraX[MAX_COBRA], cobraY[MAX_COBRA];
    int tamanhoCobra = 1;
    int objetivoX, objetivoY;
    int pontuacao = 0;
    bool jogoAtivo = true;
    int direcao = DIREITA;
    int tempoEspera = 1000000;  // Tempo inicial de espera (em microsegundos)

    // Ajustando a velocidade para tornar o jogo mais rápido
    if (dificuldade == 1) tempoEspera = 500000;  // Fácil - tempo de espera muito reduzido
    else if (dificuldade == 2) tempoEspera = 250000;  // Médio - tempo de espera ainda menor
    else if (dificuldade == 3) tempoEspera = 100000;  // Difícil - tempo de espera extremamente curto

    inicializarTabuleiro(tabuleiro);
    cobraX[0] = ALTURA / 2;
    cobraY[0] = LARGURA / 2;
    tabuleiro[cobraX[0]][cobraY[0]] = '@'; // Cabeça da cobra
    gerarObjetivo(tabuleiro, &objetivoX, &objetivoY);

    while (jogoAtivo) {
        limparTela();
        printf("Pontuacao: %d\n\n", pontuacao);
        imprimirTabuleiro(tabuleiro);

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 'W' || tecla == 'w') {
                if (direcao != BAIXO) direcao = CIMA;
            } else if (tecla == 'S' || tecla == 's') {
                if (direcao != CIMA) direcao = BAIXO;
            } else if (tecla == 'A' || tecla == 'a') {
                if (direcao != DIREITA) direcao = ESQUERDA;
            } else if (tecla == 'D' || tecla == 'd') {
                if (direcao != ESQUERDA) direcao = DIREITA;
            }
        }

        moverCobra(tabuleiro, cobraX, cobraY, &tamanhoCobra, &objetivoX, &objetivoY, &pontuacao, &jogoAtivo, &direcao);
        usleep(tempoEspera);  // Movimento automático com tempo de espera ajustado
    }

    printf("\nFim de jogo! Pontuacao final: %d\n", pontuacao);

    // Verificando e atualizando recorde
    if (pontuacao > recordePontos) {
        recordePontos = pontuacao;
        snprintf(recordeJogador, sizeof(recordeJogador), "%s", nomeJogador);
    }

    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getchar(); // Pausa
    getchar();
}

void exibirRecord() {
    printf("\nRecorde Atual:\n");
    if (recordePontos > 0) {
        printf("Jogador: %s - Pontuacao: %d\n", recordeJogador, recordePontos);
    } else {
        printf("Ainda nao ha recorde.\n");
    }
    printf("\nPressione qualquer tecla para voltar ao menu...\n");
    getchar(); // Pausa
    getchar();
}

void imprimirTabuleiro(char tabuleiro[ALTURA][LARGURA]) {
    printf("+");
    for (int i = 0; i < LARGURA; i++) {
        printf("-");
    }
    printf("+\n");

    for (int i = 0; i < ALTURA; i++) {
        printf("|");
        for (int j = 0; j < LARGURA; j++) {
            if (tabuleiro[i][j] == '@') {
                printf(AZUL "@");
            } else if (tabuleiro[i][j] == 'O') {
                printf(AZUL "O");
            } else if (tabuleiro[i][j] == 'X') {
                printf(VERMELHO "X" RESET);
            } else {
                printf(" ");
            }
        }
        printf("|\n");
    }

    printf("+");
    for (int i = 0; i < LARGURA; i++) {
        printf("-");
    }
    printf("+\n");
}

void inicializarTabuleiro(char tabuleiro[ALTURA][LARGURA]) {
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
}

void gerarObjetivo(char tabuleiro[ALTURA][LARGURA], int *objetivoX, int *objetivoY) {
    do {
        *objetivoX = rand() % ALTURA;
        *objetivoY = rand() % LARGURA;
    } while (tabuleiro[*objetivoX][*objetivoY] == '@' || tabuleiro[*objetivoX][*objetivoY] == 'O');

    tabuleiro[*objetivoX][*objetivoY] = 'X';
}

void moverCobra(char tabuleiro[ALTURA][LARGURA], int cobraX[], int cobraY[], int *tamanhoCobra, int *objetivoX, int *objetivoY, int *pontuacao, bool *jogoAtivo, int *direcao) {
    int novoX = cobraX[0];
    int novoY = cobraY[0];

    if (*direcao == DIREITA) {
        novoY++;
    } else if (*direcao == ESQUERDA) {
        novoY--;
    } else if (*direcao == CIMA) {
        novoX--;
    } else if (*direcao == BAIXO) {
        novoX++;
    }

    if (novoX < 0 || novoX >= ALTURA || novoY < 0 || novoY >= LARGURA) {
        *jogoAtivo = false;
        return;
    }

    for (int i = 0; i < *tamanhoCobra; i++) {
        if (cobraX[i] == novoX && cobraY[i] == novoY) {
            *jogoAtivo = false;
            return;
        }
    }

    if (novoX == *objetivoX && novoY == *objetivoY) {
        (*pontuacao)++;
        (*tamanhoCobra)++;
        gerarObjetivo(tabuleiro, objetivoX, objetivoY);
    } else {
        int caudaX = cobraX[*tamanhoCobra - 1];
        int caudaY = cobraY[*tamanhoCobra - 1];
        tabuleiro[caudaX][caudaY] = ' ';
    }

    for (int i = *tamanhoCobra - 1; i > 0; i--) {
        cobraX[i] = cobraX[i - 1];
        cobraY[i] = cobraY[i - 1];
    }

    cobraX[0] = novoX;
    cobraY[0] = novoY;
    tabuleiro[novoX][novoY] = '@';

    for (int i = 1; i < *tamanhoCobra; i++) {
        tabuleiro[cobraX[i]][cobraY[i]] = 'O';
    }
}

void limparTela() {
    #ifdef _WIN32
        system("cls");  // Para Windows
    #else
        system("clear");  // Para Linux/Unix/MacOS
    #endif
}
