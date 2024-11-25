#include "projeteis.h"
#include <stdio.h>
#include <stdlib.h>

projetil_lista *cria_projetil_lista(){
    projetil_lista *lista = (projetil_lista*) malloc(sizeof(projetil_lista));
    if (!lista)
        return NULL;
    
    lista->inicio = NULL;
    lista->tamanho = 0;

    return lista;
}

void destroi_projetil_lista(projetil_lista **lista){
    nodo_bala *aux;

    while ((*lista)->inicio != NULL){
        aux = (*lista)->inicio;
        (*lista)->inicio = (*lista)->inicio->prox;
        free(aux);
    }

    free(*lista);
    (*lista) = NULL;
}

void insere_bala(projetil_lista *lista, unsigned short x, unsigned short y, unsigned char dano){
    nodo_bala *novo = (nodo_bala*)malloc(sizeof(nodo_bala));
    if (!novo)
        return;

    novo->x = x;
    novo->y = y;
    novo->dano = dano;
    novo->prox = lista->inicio;
    lista->inicio = novo;
    (lista->tamanho)++; 
}
void lista_remove_inicio(projetil_lista *lista){
    nodo_bala *aux;

    if(!lista->tamanho)
        return;
    
    aux = lista->inicio;
    lista->inicio = lista->inicio->prox;
    (lista->tamanho)--;
    free(aux);
    aux = NULL;
}
// Atualiza a posição dos projéteis e remove os que sairam da tela
void atualiza_projetil(projetil_lista *lista, short trajetoria, unsigned short max_x, unsigned short max_y) {
    if (!lista || lista->tamanho == 0)
        return;

    nodo_bala *atual = lista->inicio;
    nodo_bala *anterior = NULL;

    while (atual) {
        if (trajetoria == 1) {
            // Atualiza posição da bala
            atual->x += VEL_BALA;
        }
        else {
            atual->x -= VEL_BALA;
        }

        // Verifica se o projetil está fora da tela
        if (atual->x > max_x || atual->x < 0 || atual->y > max_y || atual->y < 0) {
            if (anterior == NULL) {
                // Remove o primeiro nó
                lista->inicio = atual->prox;
            } 
            else {
                // Remove o nó atual
                anterior->prox = atual->prox;
            }
            nodo_bala *removido = atual;
            atual = atual->prox;
            free(removido);
            (lista->tamanho)--;
        } 
        else {
            // Avança para o próximo nó
            anterior = atual;
            atual = atual->prox;
        }
    }
}
