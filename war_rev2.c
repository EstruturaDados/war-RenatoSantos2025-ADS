#include <stdio.h>
#include <string.h>

// Struct que representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    Territorio territorios[5];  // Vetor com 5 territórios
    int i;

    printf("Cadastro de territorios\n");

    // Cadastro dos dados
    for (i = 0; i < 5; i++) {
        printf("\nTerritorio %d\n", i + 1);

        printf("Nome: ");
        scanf("%29s", territorios[i].nome);

        printf("Cor do exercito: ");
        scanf("%9s", territorios[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos dados cadastrados
    printf("\nTerritorios cadastrados:\n");

    for (i = 0; i < 5; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}

