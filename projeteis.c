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

    novo->x = x-50;
    novo->y = y+25;
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

void atualiza_projetil(projetil_lista *lista, short trajetoria, unsigned short max_x, unsigned short max_y) {
    if (!lista || lista->tamanho == 0)
        return;

    nodo_bala *atual = lista->inicio;
    nodo_bala *anterior = NULL;

    while (atual) {
        // Atualiza a posição do projétil
        if (trajetoria == 1) {
            atual->x += VEL_BALA;
        } else {
            atual->x -= VEL_BALA;
        }

        // Atualiza a animação do projétil
        atual->tempo_anim++;
        if (atual->tempo_anim >= TEMPO_POR_FRAME) {
            atual->frame = (atual->frame + 1) % FRAMES_PROJETIL;
            atual->tempo_anim = 0;
        }

        // Verifica se o projétil saiu da tela
        if (atual->x > max_x || atual->x < 0 || atual->y > max_y || atual->y < 0) {
            if (anterior == NULL) {
                lista->inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            nodo_bala *removido = atual;
            atual = atual->prox;
            free(removido);
            lista->tamanho--;
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}
