#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5

typedef struct {
    int id;
    char tipo[2]; // 'I', 'O', 'T', 'L'
} Peca;

typedef struct {
    Peca fila[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} FilaCircular;

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    nova.id = id;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    nova.tipo[0] = tipos[rand() % 4];
    nova.tipo[1] = '\0';
    return nova;
}

// Inicializa a fila com 5 peças
void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->fila[f->fim] = gerarPeca(i + 1);
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// Exibe o estado atual da fila
void visualizarFila(FilaCircular *f) {
    printf("\nFila de Peças:\n");
    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("Posição %d: Peça %s (ID %d)\n", i + 1, f->fila[idx].tipo, f->fila[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    if (f->quantidade == 0) {
        printf("Fila vazia!\n");
    }
}

// Remove a peça da frente (dequeue)
void jogarPeca(FilaCircular *f) {
    if (f->quantidade == 0) {
        printf("Não há peças para jogar!\n");
        return;
    }
    printf("Jogando peça %s (ID %d)\n", f->fila[f->inicio].tipo, f->fila[f->inicio].id);
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
}

// Insere nova peça no final (enqueue)
void inserirPeca(FilaCircular *f, int *idCounter) {
    if (f->quantidade == TAM_FILA) {
        printf("Fila cheia! Não é possível inserir nova peça.\n");
        return;
    }
    f->fila[f->fim] = gerarPeca((*idCounter)++);
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
    printf("Peça inserida com sucesso!\n");
}

// Menu principal
void menu() {
    FilaCircular fila;
    int opcao;
    int idCounter = TAM_FILA + 1;
    srand(time(NULL));

    inicializarFila(&fila);

    do {
        printf("\n--- Tetris Stack: Nível Novato ---\n");
        printf("1. Visualizar fila\n");
        printf("2. Jogar peça\n");
        printf("3. Inserir nova peça\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                visualizarFila(&fila);
                break;
            case 2:
                jogarPeca(&fila);
                break;
            case 3:
                inserirPeca(&fila, &idCounter);
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

int main() {
    menu();
    return 0;
}