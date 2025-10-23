#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo os tamanhos da fila e da pilha
#define TAM_FILA 5
#define TAM_PILHA 3

// Função que gera uma peça aleatória do conjunto clássico do Tetris
char gerarPeca() {
    char pecas[] = {'I', 'O', 'T', 'S', 'Z', 'L', 'J'};
    return pecas[rand() % 7];
}

// Estrutura da fila circular
typedef struct {
    char itens[TAM_FILA]; // Array que armazena as peças
    int inicio;           // Índice do início da fila
    int fim;              // Índice do fim da fila
} Fila;

// Inicializa a fila com 5 peças aleatórias
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->itens[i] = gerarPeca();
        f->fim = (f->fim + 1) % TAM_FILA;
    }
}

// Remove a peça da frente da fila e insere uma nova no final
char removerFila(Fila *f) {
    char peca = f->itens[f->inicio]; // Captura a peça da frente
    f->itens[f->inicio] = gerarPeca(); // Substitui por uma nova peça
    f->inicio = (f->inicio + 1) % TAM_FILA; // Atualiza o índice de início
    return peca;
}

// Exibe o estado atual da fila
void mostrarFila(Fila f) {
    printf("Fila: ");
    for (int i = 0; i < TAM_FILA; i++) {
        printf("[%c] ", f.itens[(f.inicio + i) % TAM_FILA]);
    }
    printf("\n");
}

// Estrutura da pilha linear
typedef struct {
    char itens[TAM_PILHA]; // Array que armazena as peças reservadas
    int topo;              // Índice do topo da pilha
} Pilha;

// Inicializa a pilha como vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adiciona uma peça ao topo da pilha (push)
void empilhar(Pilha *p, char valor) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = valor;
        printf("Peça '%c' reservada.\n", valor);
    } else {
        printf("A pilha está cheia!\n");
    }
}

// Remove a peça do topo da pilha (pop)
char desempilhar(Pilha *p) {
    if (!pilhaVazia(p)) {
        char valor = p->itens[p->topo];
        p->topo--;
        return valor;
    } else {
        printf("A pilha está vazia!\n");
        return '\0'; // Retorna caractere nulo se não houver peça
    }
}

// Exibe o estado atual da pilha
void mostrarPilha(Pilha p) {
    printf("Pilha: ");
    for (int i = 0; i <= p.topo; i++) {
        printf("[%c] ", p.itens[i]);
    }
    printf("\n");
}

// Função principal com menu interativo
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);  // Preenche a fila com peças iniciais
    inicializarPilha(&pilha); // Inicializa a pilha vazia

    int opcao;
    do {
        // Exibe o menu de opções
        printf("\nMenu:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        // Executa a ação conforme a opção escolhida
        switch (opcao) {
            case 1: {
                // Jogar a peça da frente da fila
                char jogada = removerFila(&fila);
                printf("Jogando peça: %c\n", jogada);
                break;
            }
            case 2: {
                // Reservar a peça da frente da fila
                char reserva = removerFila(&fila);
                empilhar(&pilha, reserva);
                break;
            }
            case 3: {
                // Usar a peça do topo da pilha
                char usada = desempilhar(&pilha);
                if (usada != '\0') {
                    printf("Usando peça reservada: %c\n", usada);
                }
                break;
            }
            case 0:
                // Finaliza o programa
                printf("Encerrando...\n");
                break;
            default:
                // Opção inválida
                printf("Opção inválida!\n");
        }

        // Exibe o estado atual da fila e da pilha após cada ação
        mostrarFila(fila);
        mostrarPilha(pilha);

    } while (opcao != 0); // Repete até o usuário escolher sair

    return 0;
}