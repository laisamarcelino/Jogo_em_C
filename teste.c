#include "projeteis.h"
#include <stdio.h>
#include <stdlib.h>

void imprime_lista(projetil_lista *lista) {
    nodo_bala *atual = lista->inicio;
    printf("Lista de projéteis:\n");
    while (atual) {
        printf("Posição: (%hu, %hu), Trajetória: %hhu, Dano: %hhu\n", 
               atual->x, atual->y, atual->trajetoria, atual->dano);
        atual = atual->prox;
    }
    printf("Total de projéteis: %d\n", lista->tamanho);
}

int main() {
    projetil_lista *lista = cria_projetil_lista();
    insere_bala(lista, 10, 20, 1, 5);
    insere_bala(lista, 50, 30, 1, 7);
    insere_bala(lista, 100, 40, 1, 10);

    printf("Antes da atualização:\n");
    imprime_lista(lista);

    atualiza_projetil(lista, 60, 50); // Limite da tela: 60x50

    printf("\nApós a atualização:\n");
    imprime_lista(lista);

    destroi_projetil_lista(&lista);
    return 0;
}

