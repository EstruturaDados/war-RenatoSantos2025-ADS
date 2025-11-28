// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define TAM_NOME        30
#define TAM_COR         10
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_NOME];
    char corExercito[TAM_COR];  /* cor do exercito que domina o territorio */
    int tropas;
} Territorio;
// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
// Struct que representa um território
Territorio *alocarMapa(int qtdTerritorios);
void liberarMapa(Territorio *mapa);

// Cadastro e exibição
void cadastrarTerritorios(Territorio *mapa, int qtdTerritorios);
void exibirMapa(const Territorio *mapa, int qtdTerritorios);

// Lógica de jogo
void faseDeAtaque(Territorio *mapa, int qtdTerritorios, const char *corJogador);
void simularAtaque(Territorio *mapa, int origem, int destino, const char *corJogador);

// Missoes
int sortearMissao(void);
void exibirMissao(int idMissao);
int verificarMissao(const Territorio *mapa, int qtdTerritorios, int idMissao, const char *corJogador);

// Utilitários
void limparBufferEntrada(void);
int lerInteiro(const char *mensagem, int min, int max);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main(void) {
    setlocale(LC_ALL, "");
    srand((unsigned) time(NULL));

    Territorio *mapa = alocarMapa(NUM_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    char corJogador[TAM_COR];

    printf("=== DESAFIO WAR ESTRUTURADO ===\n\n");
    printf("Digite a cor do seu exercito (ex: Azul, Vermelho): ");
    if (scanf("%9s", corJogador) != 1) {
        printf("Entrada invalida.\n");
        liberarMapa(mapa);
        return 1;
    }
    limparBufferEntrada();

    cadastrarTerritorios(mapa, NUM_TERRITORIOS);

    int missao = sortearMissao();
    int opcao;
    int venceu = 0;

    do {
        printf("\n=====================================\n");
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(missao);

        printf("\nMENU PRINCIPAL\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar missao\n");
        printf("0 - Sair\n");

        opcao = lerInteiro("Escolha uma opcao: ", 0, 2);

        switch (opcao) {
        case 1:
            faseDeAtaque(mapa, NUM_TERRITORIOS, corJogador);
            break;
        case 2:
            venceu = verificarMissao(mapa, NUM_TERRITORIOS, missao, corJogador);
            if (venceu) {
                printf("\nParabens! Voce cumpriu sua missao!\n");
            } else {
                printf("\nMissao ainda nao cumprida.\n");
            }
            break;
        case 0:
            printf("\nSaindo do jogo...\n");
            break;
        }

    } while (opcao != 0 && !venceu);

    liberarMapa(mapa);
    return 0;
}


    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

/* Aloca dinamicamente o vetor de territorios */
Territorio *alocarMapa(int qtdTerritorios) {
    Territorio *mapa = (Territorio *) calloc(qtdTerritorios, sizeof(Territorio));
    return mapa;
}

/* Libera a memória do mapa */
void liberarMapa(Territorio *mapa) {
    free(mapa);
}

/* Cadastro inicial dos territórios (Nivel Novato) */
void cadastrarTerritorios(Territorio *mapa, int qtdTerritorios) {
    int i;

    printf("\n=== Cadastro de Territorios ===\n");

    for (i = 0; i < qtdTerritorios; i++) {
        printf("\nTerritorio %d\n", i + 1);

        printf("Nome: ");
        if (fgets(mapa[i].nome, TAM_NOME, stdin) == NULL) {
            strcpy(mapa[i].nome, "Indefinido");
        } else {
            mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        }

        printf("Cor do exercito dominante: ");
        if (fgets(mapa[i].corExercito, TAM_COR, stdin) == NULL) {
            strcpy(mapa[i].corExercito, "Neutro");
        } else {
            mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';
        }

        printf("Numero de tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            mapa[i].tropas = 0;
        }
        limparBufferEntrada();
    }
}

/* Mostra o estado atual do mapa */
void exibirMapa(const Territorio *mapa, int qtdTerritorios) {
    int i;

    printf("\n=== MAPA ATUAL ===\n");
    for (i = 0; i < qtdTerritorios; i++) {
        printf("%d - %-20s | Cor: %-10s | Tropas: %d\n",
               i,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].tropas);
    }
}

/* Fase de ataque (Nivel Aventureiro/Mestre) */
void faseDeAtaque(Territorio *mapa, int qtdTerritorios, const char *corJogador) {
    int origem, destino;

    printf("\n--- Fase de Ataque ---\n");
    origem = lerInteiro("Indice do territorio atacante: ", 0, qtdTerritorios - 1);
    destino = lerInteiro("Indice do territorio defensor: ", 0, qtdTerritorios - 1);

    if (origem == destino) {
        printf("Origem e destino nao podem ser o mesmo.\n");
        return;
    }

    if (mapa[origem].tropas < 2) {
        printf("O territorio atacante precisa ter pelo menos 2 tropas.\n");
        return;
    }

    if (strcmp(mapa[origem].corExercito, corJogador) != 0) {
        printf("Voce so pode atacar a partir de territorios do seu exercito.\n");
        return;
    }

    if (strcmp(mapa[destino].corExercito, corJogador) == 0) {
        printf("Voce nao pode atacar um territorio do seu proprio exercito.\n");
        return;
    }

    simularAtaque(mapa, origem, destino, corJogador);
}

/* Simula um ataque simples com dados */
void simularAtaque(Territorio *mapa, int origem, int destino, const char *corJogador) {
    int dadoAtq, dadoDef;

    if (mapa[destino].tropas <= 0) {
        printf("O territorio defensor nao possui tropas.\n");
        return;
    }

    dadoAtq = (rand() % 6) + 1;
    dadoDef = (rand() % 6) + 1;

    printf("\nAtaque de %s em %s\n", mapa[origem].nome, mapa[destino].nome);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);

    if (dadoAtq >= dadoDef) {
        /* atacante vence ou empata (empate favorece atacante) */
        mapa[destino].tropas--;

        printf("Defensor perde 1 tropa. Tropas restantes em %s: %d\n",
               mapa[destino].nome, mapa[destino].tropas);

        if (mapa[destino].tropas <= 0) {
            printf("Territorio conquistado!\n");
            /* muda cor do exercito do territorio conquistado */
            strncpy(mapa[destino].corExercito, corJogador, TAM_COR - 1);
            mapa[destino].corExercito[TAM_COR - 1] = '\0';
            /* move 1 tropa do atacante para o novo territorio */
            mapa[origem].tropas--;
            mapa[destino].tropas = 1;
        }
    } else {
        /* defensor ganha, atacante perde uma tropa */
        mapa[origem].tropas--;
        printf("Atacante perde 1 tropa. Tropas restantes em %s: %d\n",
               mapa[origem].nome, mapa[origem].tropas);
    }
}

/* Sorteia uma missao (Nivel Mestre) */
int sortearMissao(void) {
    /* 1 = destruir exercito Verde, 2 = conquistar 3 territorios */
    return (rand() % 2) + 1;
}

/* Exibe a missao atual */
void exibirMissao(int idMissao) {
    printf("\n=== MISSAO ATUAL ===\n");
    switch (idMissao) {
    case 1:
        printf("Missao 1: Destruir o exercito Verde (nenhum territorio pode ter cor 'Verde').\n");
        break;
    case 2:
        printf("Missao 2: Conquistar pelo menos 3 territorios com a sua cor.\n");
        break;
    default:
        printf("Missao desconhecida.\n");
        break;
    }
}

/* Verifica se a missao foi cumprida */
int verificarMissao(const Territorio *mapa, int qtdTerritorios, int idMissao, const char *corJogador) {
    int i;
    int contador = 0;

    switch (idMissao) {
    case 1: /* destruir exercito Verde */
        for (i = 0; i < qtdTerritorios; i++) {
            if (strcmp(mapa[i].corExercito, "Verde") == 0 && mapa[i].tropas > 0) {
                return 0; /* ainda existe territorio Verde com tropas */
            }
        }
        return 1; /* nenhum territorio Verde com tropas */
    case 2: /* conquistar 3 territorios */
        for (i = 0; i < qtdTerritorios; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0 && mapa[i].tropas > 0) {
                contador++;
            }
        }
        return (contador >= 3);
    default:
        return 0;
    }
}


    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
/* Limpa o buffer de entrada (stdin) */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres restantes */
    }
}

/* Le um inteiro com validação simples de faixa */
int lerInteiro(const char *mensagem, int min, int max) {
    int valor;
    int leituraValida;

    do {
        printf("%s", mensagem);
        leituraValida = scanf("%d", &valor);
        limparBufferEntrada();

        if (leituraValida != 1 || valor < min || valor > max) {
            printf("Valor invalido. Tente novamente.\n");
            leituraValida = 0;
        }
    } while (!leituraValida);

    return valor;
}



// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
