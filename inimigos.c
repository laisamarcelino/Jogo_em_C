#include "inimigos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y,  unsigned short max_x, unsigned short max_y){
    
    inimigo *novo_inimigo = (inimigo*)malloc(sizeof(inimigo));
    if (!novo_inimigo)
        return NULL;
    
    novo_inimigo->tipo = tipo;
    novo_inimigo->hp = hp;
    novo_inimigo->largura = largura;
    novo_inimigo->altura = altura;
    novo_inimigo->dano = dano;
    novo_inimigo->x = x;
    novo_inimigo->y = y;
    novo_inimigo->projeteis = cria_projetil_lista();
    novo_inimigo->tempo_disparo = 30; // Dispara a cada 60 frames
    novo_inimigo->contador_disparo = 0;

    return novo_inimigo;
}

unsigned short aleat (unsigned short min, unsigned short max){
    return min + rand() % (max+1 - min);
}

void mov_inimigo(inimigo* inimigo, char passos, unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y) {
    static int fator = 1; // Para nao reiniciar a variavel

    if (!inimigo)
        return;
    
    switch (inimigo->tipo) {
        case 1: 
            inimigo->x -= passos * PASSOS_INIMIGO;
            break;
        case 2:
            inimigo->x -= passos * PASSOS_INIMIGO-2;
            inimigo->y = (int)(max_y / 2 + (sin(inimigo->x * 0.01) * 250)); // Produz um movimento ondulatorio
            break;
        case 3:
            inimigo->x -= passos * PASSOS_INIMIGO-3;
            inimigo->y = (int)(max_y / 2 + (sin(inimigo->x * 0.05) * 100)); 
            break;
        case 4:
            inimigo->y += passos * PASSOS_INIMIGO; // Move para baixo
            break;
        case 5: 
            // Verifica se o inimigo atingiu o limite superior
            if (inimigo->y <= 0 + altura / 2)
                fator = 1;

            // Verifica se o inimigo atingiu o limite inferior
            if (inimigo->y >= max_y - altura / 2)
                fator = -1;

            inimigo->y += passos * PASSOS_INIMIGO * fator;  // Move para cima

            break;

        default:
            break;
    
    }

    // Verifica se o inimigo saiu da tela 
    if (inimigo->x - largura / 2 < 0) {
        inimigo->x = max_x + largura / 2;
        inimigo->y = aleat(largura / 2, max_y - largura / 2);
    }
}

void ataque_inimigo(inimigo *inimigo){
    unsigned char dano;

    unsigned short x_tiro = inimigo->x - inimigo->largura / 2; // Inicia na frente do inimigo
    unsigned short y_tiro = inimigo->y; // Na mesma altura do inimigo
    
    switch (inimigo->tipo) {
        case 2:
            dano = 1;
            break;
        case 3:
            dano = 2;
            break;
        case 4:
            dano = 3;
            break;
        case 5:
            dano = 3;
            break;
        
        default:
            break;
    }

    insere_bala(inimigo->projeteis, x_tiro, y_tiro, dano);
}

void especial_inimigo(inimigo *inimigo);

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);
}

/*
lista_inimigo* cria_lista_inimigo(){
    lista_inimigo *nova_lista = (lista_inimigo*)malloc(sizeof(lista_inimigo));
    if (!nova_lista)
        return NULL;
    
    nova_lista->ini = NULL;
    nova_lista->tamanho = 0;

    return nova_lista;
}

void insere_inimigo(lista_inimigo *lista, inimigo* chave_inimigo, unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y,  unsigned short max_x, unsigned short max_y){
    nodo_inimigo *aux;

    nodo_inimigo *novo_nodo = (nodo_inimigo*)malloc(sizeof(novo_nodo));
    if (!novo_nodo) {
        fprintf(stderr, "Erro ao criar inimigo");
        return;
    }
    
    novo_nodo->chave_inimigo = cria_inimigo(tipo, hp, largura, altura, dano, x, y, max_x, max_y);
    novo_nodo->prox = NULL;

    if (lista->ini == NULL)
        lista->ini = novo_nodo;
    else {
        aux = lista->ini;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo_nodo;
    }

    (lista->tamanho)--;

}

// Função para inserir inimigos aleatoriamente na lista
void gera_inimigo(lista_inimigo *lista, unsigned short max_x, unsigned short max_y) {
    unsigned char tipo = aleat(1, 2); 
    unsigned char hp = (tipo == 1) ? 3 : 2;
    unsigned char largura = 50;
    unsigned char altura = 50;
    unsigned char dano = -1;
    unsigned short x = max_x - 10; // Direita da tela
    unsigned short y = aleat(50, max_y - 50); 

    inimigo *novo_inimigo = cria_inimigo(tipo, hp, largura, altura, dano, x, y, max_x, max_y);
    if (!novo_inimigo) {
        fprintf(stderr, "Erro ao criar inimigo");
        return;
    }
    
    insere_inimigo(lista, novo_inimigo, tipo, hp, largura, altura, dano, x, y, max_x, max_y);

}

void atualiza_inimigos(lista_inimigo *lista, jogador *player, unsigned short max_x, unsigned short max_y) {
    nodo_inimigo *atual = lista->ini;
    while (atual) {
        inimigo *inimigo_atual = atual->chave_inimigo;
        if (inimigo_atual->hp > 0) {

            mov_inimigo(inimigo_atual, 1, inimigo_atual->largura, inimigo_atual->altura, max_x, max_y);
            
            verifica_colisao_players(player, inimigo_atual);
            verifica_colisao_projeteis(player, inimigo_atual);

            al_draw_filled_rectangle(
                inimigo_atual->x - inimigo_atual->largura / 2, inimigo_atual->y - inimigo_atual->altura / 2,
                inimigo_atual->x + inimigo_atual->largura / 2, inimigo_atual->y + inimigo_atual->altura / 2,
                al_map_rgb(255, 0, 0)
            );
        }

        // Incrementa o contador de disparo do inimigo 2
        if (inimigo_atual->contador_disparo >= inimigo_atual->tempo_disparo) {
            ataque_inimigo(inimigo_atual); // Inimigo dispara projétil
            inimigo_atual->contador_disparo = 0;
        }
                        
        // Desenha projéteis do inimigo 2
        nodo_bala *atual_inimigo = inimigo_atual->projeteis->inicio;
        while (atual_inimigo) {
            al_draw_filled_circle(atual_inimigo->x, atual_inimigo->y, 5, al_map_rgb(255, 0, 0)); // Vermelho 
             atual_inimigo = atual_inimigo->prox;
        }                
        
        atualiza_projetil(inimigo_atual->projeteis, -1, max_x, max_y);
                
        atual = atual->prox;
    }
}*/