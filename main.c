#include <stdio.h>
#include <string.h>

#define MAX_SALA 10
#define MAX_FILA 100

// Estrutura de um jogador
typedef struct {
    char nome[30];
} Jogador;

// Estrutura da fila circular
typedef struct {
    Jogador dados[MAX_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Variaveis globais
Jogador sala[MAX_SALA];
int quantidadeSala = 0;

Fila filaEspera;

// Inicializar a fila
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}

// Inserir jogador na fila
int enfileirar(Fila *fila, Jogador novo) {
    if (fila->quantidade == MAX_FILA) {
        printf("\nA fila de espera esta cheia!\n");
        return 0;
    }

    fila->dados[fila->fim] = novo;

    fila->fim = (fila->fim + 1) % MAX_FILA;

    fila->quantidade++;

    return 1;
}

// Retirar primeiro jogador da fila
int desenfileirar(Fila *fila, Jogador *removido) {
    if (fila->quantidade == 0) {
        return 0;
    }

    *removido = fila->dados[fila->inicio];

    fila->inicio = (fila->inicio + 1) % MAX_FILA;

    fila->quantidade--;

    return 1;
}

// Procurar jogador na sala
int buscarNaSala(char nome[]) {
    for (int i = 0; i < quantidadeSala; i++) {
        if (strcmp(sala[i].nome, nome) == 0) {
            return 1;
        }
    }

    return 0;
}

// Procurar jogador na fila
int buscarNaFila(char nome[]) {
    for (int i = 0; i < filaEspera.quantidade; i++) {
        int posicao = (filaEspera.inicio + i) % MAX_FILA;

        if (strcmp(filaEspera.dados[posicao].nome, nome) == 0) {
            return 1;
        }
    }

    return 0;
}

// Adicionar jogador na sala ou fila
void entrarSala() {
    Jogador novo;

    printf("\nDigite o nome do jogador: ");
    scanf(" %29[^\n]", novo.nome);

    if (buscarNaSala(novo.nome) || buscarNaFila(novo.nome)) {
        printf("\nEsse jogador ja esta no sistema!\n");
        return;
    }

    if (quantidadeSala >= MAX_SALA) {
        if (enfileirar(&filaEspera, novo)) {
            printf("\nJogador %s entrou na fila de espera!\n",
                   novo.nome);
        }
        return;
    }

    sala[quantidadeSala] = novo;
    quantidadeSala++;

    printf("\nJogador %s entrou na sala!\n", novo.nome);
}

// Remover jogador da sala
void sairSala() {
    char nome[30];
    int posicao = -1;

    if (quantidadeSala == 0) {
        printf("\nA sala esta vazia!\n");
        return;
    }

    printf("\nDigite o nome do jogador que deseja remover: ");
    scanf(" %29[^\n]", nome);

    for (int i = 0; i < quantidadeSala; i++) {
        if (strcmp(sala[i].nome, nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("\nJogador nao encontrado na sala!\n");
        return;
    }

    printf("\nJogador %s saiu da sala!\n", sala[posicao].nome);

    // Reorganizar o vetor
    for (int i = posicao; i < quantidadeSala - 1; i++) {
        sala[i] = sala[i + 1];
    }

    quantidadeSala--;

    sala[quantidadeSala].nome[0] = '\0';

    // Promover o primeiro jogador da fila
    if (filaEspera.quantidade > 0) {
        Jogador proximo;

        desenfileirar(&filaEspera, &proximo);

        sala[quantidadeSala] = proximo;
        quantidadeSala++;

        printf("\nJogador %s entrou automaticamente na sala!\n",
               proximo.nome);
    }
}

// Remover jogador da fila de espera
void sairFila() {
    char nome[30];
    int posicao = -1;

    if (filaEspera.quantidade == 0) {
        printf("\nA fila de espera esta vazia!\n");
        return;
    }

    printf("\nDigite o nome do jogador que deseja sair da fila: ");
    scanf(" %29[^\n]", nome);

    // Procurar jogador na fila
    for (int i = 0; i < filaEspera.quantidade; i++) {
        int atual = (filaEspera.inicio + i) % MAX_FILA;

        if (strcmp(filaEspera.dados[atual].nome, nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("\nJogador nao encontrado na fila!\n");
        return;
    }

    printf("\nJogador %s saiu da fila!\n", nome);

    // Reorganizar os jogadores
    for (int i = posicao; i < filaEspera.quantidade - 1; i++) {
        int atual = (filaEspera.inicio + i) % MAX_FILA;
        int seguinte = (filaEspera.inicio + i + 1) % MAX_FILA;

        filaEspera.dados[atual] = filaEspera.dados[seguinte];
    }

    filaEspera.fim =
        (filaEspera.fim - 1 + MAX_FILA) % MAX_FILA;

    filaEspera.quantidade--;
}

// Mostrar jogadores da sala
void mostrarSala() {
    printf("\n===== SALA MULTIPLAYER =====\n");

    if (quantidadeSala == 0) {
        printf("A sala esta vazia.\n");
        return;
    }

    for (int i = 0; i < quantidadeSala; i++) {
        printf("%d. %s\n", i + 1, sala[i].nome);
    }

    printf("\nJogadores: %d/%d\n",
           quantidadeSala, MAX_SALA);
}

// Mostrar fila de espera
void mostrarFila(Fila *fila) {
    printf("\n===== FILA DE ESPERA =====\n");

    if (fila->quantidade == 0) {
        printf("Nao ha jogadores esperando.\n");
        return;
    }

    for (int i = 0; i < fila->quantidade; i++) {
        int posicao = (fila->inicio + i) % MAX_FILA;

        printf("%d. %s\n",
               i + 1, fila->dados[posicao].nome);
    }

    printf("\nJogadores esperando: %d/%d\n",
           fila->quantidade, MAX_FILA);
}

// Menu principal
int main() {
    int opcao;

    inicializarFila(&filaEspera);

    do {
        printf("\n===== MULTIPLAYER QUEUE MANAGER =====\n");
        printf("1 - Entrar na sala\n");
        printf("2 - Sair da sala\n");
        printf("3 - Mostrar jogadores\n");
        printf("4 - Mostrar fila de espera\n");
        printf("5 - Sair da fila de espera\n");
        printf("0 - Sair do programa\n");
        printf("Escolha: ");

        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                entrarSala();
                break;

            case 2:
                sairSala();
                break;

            case 3:
                mostrarSala();
                break;

            case 4:
                mostrarFila(&filaEspera);
                break;

            case 5:
                sairFila();
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}