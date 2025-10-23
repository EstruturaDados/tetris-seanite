#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 5

// Estrutura que representa uma peça do jogo
typedef struct {
    int id;
    char tipo[2]; // Tipos: 'I', 'O', 'T', 'L'
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca fila[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} FilaCircular;

// Estrutura da pilha de reserva
typedef struct {
    Peca pilha[TAM_PILHA];
    int topo;
} Pilha;

// Estrutura para armazenar a última jogada (para desfazer)
typedef struct {
    Peca ultimaPeca;
    int houveJogada;
} Historico;

// Gera uma nova peça aleatória com ID único
Peca gerarPeca(int id) {
    Peca nova;
    nova.id = id;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    nova.tipo[0] = tipos[rand() % 4];
    nova.tipo[1] = '\0';
    return nova;
}

// Inicializa a fila com 5 peças geradas automaticamente
void inicializarFila(FilaCircular *f, int *idCounter) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->fila[f->fim] = gerarPeca((*idCounter)++);
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// Inicializa a pilha de reserva
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Exibe o estado atual da fila
void visualizarFila(FilaCircular *f) {
    printf("\nFila de Peças:\n");
    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("Posição %d: Peça %s (ID %d)\n", i + 1, f->fila[idx].tipo, f->fila[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
}

// Exibe o estado atual da pilha
void visualizarPilha(Pilha *p) {
    printf("\nPilha de Reserva:\n");
    if (p->topo == -1) {
        printf("Pilha vazia!\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("Topo %d: Peça %s (ID %d)\n", i, p->pilha[i].tipo, p->pilha[i].id);
    }
}

// Remove a peça da frente da fila e salva no histórico
void jogarPeca(FilaCircular *f, Historico *h) {
    if (f->quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }
    h->ultimaPeca = f->fila[f->inicio];
    h->houveJogada = 1;
    printf("Jogando peça %s (ID %d)\n", h->ultimaPeca.tipo, h->ultimaPeca.id);
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
}

// Insere nova peça no final da fila
void inserirPeca(FilaCircular *f, int *idCounter) {
    if (f->quantidade == TAM_FILA) {
        printf("Fila cheia!\n");
        return;
    }
    f->fila[f->fim] = gerarPeca((*idCounter)++);
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

// Reserva a peça da frente da fila (move para pilha)
void reservarPeca(FilaCircular *f, Pilha *p) {
    if (f->quantidade == 0 || p->topo == TAM_PILHA - 1) {
        printf("Não é possível reservar peça!\n");
        return;
    }
    p->pilha[++(p->topo)] = f->fila[f->inicio];
    printf("Peça %s (ID %d) reservada!\n", f->fila[f->inicio].tipo, f->fila[f->inicio].id);
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
}

// Usa a peça reservada (remove do topo da pilha)
void usarPecaReservada(Pilha *p) {
    if (p->topo == -1) {
        printf("Não há peça reservada!\n");
        return;
    }
    printf("Usando peça reservada %s (ID %d)\n", p->pilha[p->topo].tipo, p->pilha[p->topo].id);
    p->topo--;
}

// Troca a peça do topo da pilha com a da frente da fila
void trocarTopoComFrente(FilaCircular *f, Pilha *p) {
    if (f->quantidade == 0 || p->topo == -1) {
        printf("Não é possível trocar!\n");
        return;
    }
    Peca temp = f->fila[f->inicio];
    f->fila[f->inicio] = p->pilha[p->topo];
    p->pilha[p->topo] = temp;
    printf("Troca realizada com sucesso!\n");
}

// Desfaz a última jogada (reinsere peça na frente da fila)
void desfazerJogada(FilaCircular *f, Historico *h) {
    if (!h->houveJogada || f->quantidade == TAM_FILA) {
        printf("Não é possível desfazer!\n");
        return;
    }
    f->inicio = (f->inicio - 1 + TAM_FILA) % TAM_FILA;
    f->fila[f->inicio] = h->ultimaPeca;
    f->quantidade++;
    h->houveJogada = 0;
    printf("Jogada desfeita: Peça %s (ID %d) retornou à fila.\n", h->ultimaPeca.tipo, h->ultimaPeca.id);
}

// Inverte os elementos entre fila e pilha
void inverterFilaComPilha(FilaCircular *f, Pilha *p) {
    if (f->quantidade == 0 && p->topo == -1) {
        printf("Nada para inverter!\n");
        return;
    }

    // Troca os elementos entre fila e pilha
    int i = f->inicio;
    for (int j = 0; j < f->quantidade && j <= p->topo; j++) {
        Peca temp = f->fila[i];
        f->fila[i] = p->pilha[j];
        p->pilha[j] = temp;
        i = (i + 1) % TAM_FILA;
    }
    printf("Fila e pilha invertidas!\n");
}

// Menu principal do jogo
void menu() {
    FilaCircular fila;
    Pilha pilha;
    Historico historico = {.houveJogada = 0};
    int idCounter = 1;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila, &idCounter);
    inicializarPilha(&pilha);

    do {
        printf("\n--- Tetris Stack: Nível Mestre ---\n");
        printf("1. Jogar peça\n");
        printf("2. Reservar peça\n");
        printf("3. Usar peça reservada\n");
        printf("4. Trocar peça do topo da pilha com a da frente da fila\n");
        printf("5. Desfazer última jogada\n");
        printf("6. Inverter fila com pilha\n");
        printf("7. Visualizar fila e pilha\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila, &historico);
                inserirPeca(&fila, &idCounter);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                inserirPeca(&fila, &idCounter);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarTopoComFrente(&fila, &pilha);
                break;
            case 5:
                desfazerJogada(&fila, &historico);
                break;
            case 6:
                inverterFilaComPilha(&fila, &pilha);
                break;
            case 7:
                visualizarFila(&fila);
                visualizarPilha(&pilha);
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
