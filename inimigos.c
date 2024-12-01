#include "inimigos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y) {
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

    insere_bala(inimigo->projeteis, x_tiro+90, y_tiro, dano);
}

void especial_inimigo(inimigo *inimigo);

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);
}

void desenha_inimigo(ALLEGRO_BITMAP *sprite, inimigo *inimigo, unsigned short largura, unsigned short altura) {
    static unsigned char frame_atual = 0; // Torna estático para persistir entre chamadas
    static unsigned char tempo_anim = 0;

    tempo_anim++;
    if (tempo_anim >= TEMPO_POR_FRAME) {
        frame_atual = (frame_atual + 1) % FRAMES_INIMIGO; // Cicla entre os frames disponíveis
        tempo_anim = 0;
    }

    al_draw_bitmap_region(
        sprite,
        frame_atual * largura, 0, // Define o frame atual
        largura, altura,         // Dimensões do frame
        inimigo->x - largura / 2, inimigo->y - altura / 2, 0
    );
}


tamanho* carrega_sprites() {
    tamanho *tamanho;

    tamanho->inimigo1 = al_load_bitmap("./sprites/inimigo1.png");
    // tamanho->inimigo2 = al_load_bitmap("./sprites/inimigo2.png");
    // tamanho->inimigo3 = al_load_bitmap("./sprites/inimigo3.png");
    // tamanho->inimigo4 = al_load_bitmap("./sprites/inimigo4.png");
    // tamanho->boss1 = al_load_bitmap("./sprites/boss1.png");
    // tamanho->boss2 = al_load_bitmap("./sprites/boss1.png");
    // tamanho->projetil2 = al_load_bitmap("./sprites/projetil2.png");
    // tamanho->projetil3 = al_load_bitmap("./sprites/projetil3.png");
    // tamanho->projetil4 = al_load_bitmap("./sprites/projetil4.png");
    // tamanho->projetil_boss1 = al_load_bitmap("./sprites/iprojetil_boss1.png");
    // tamanho->projetil_boss2 = al_load_bitmap("./sprites/projetil_boss2.png");
    /*
    if (!inimigo1 || !inimigo2 || !inimigo3 || !inimigo4 || !boss1 || !boss2 || !projetil2 || !projetil3 || !projetil4 || !projetil_boss1 || !projetil_boss2) {
        fprintf(stderr, "Erro ao carregar sprites dos inimigos.\n");
        return NULL;
    }*/

    // Calcula a largura e altura dos inimigos
    tamanho->l1 = al_get_bitmap_width(tamanho->inimigo1) / FRAMES_INIMIGO;
    tamanho->a1 = al_get_bitmap_height(tamanho->inimigo1);
    // tamanho->l2 = al_get_bitmap_width(inimigo2) / FRAMES_INIMIGO;
    // tamanho->a2 = al_get_bitmap_height(inimigo2);
    // tamanho->l3 = al_get_bitmap_width(inimigo3) / FRAMES_INIMIGO;
    // tamanho->a3 = al_get_bitmap_height(inimigo3);
    // tamanho->l4 = al_get_bitmap_width(inimigo4) / FRAMES_INIMIGO;
    // tamanho->a4 = al_get_bitmap_height(inimigo4);
    // tamanho->lb1 = al_get_bitmap_width(boss1) / FRAMES_INIMIGO;
    // tamanho->ab1 = al_get_bitmap_height(boss1);
    // tamanho->lb2 = al_get_bitmap_width(boss2) / FRAMES_INIMIGO;
    // tamanho->ab2 = al_get_bitmap_height(boss2);

    // Calcula a largura e altura dos projeteis
    // tamanho->lp2 = al_get_bitmap_width(projetil2) / FRAMES_PROJETIL;
    // tamanho->ap2 = al_get_bitmap_height(projetil2); 
    // tamanho->lp3 = al_get_bitmap_width(projetil3) / FRAMES_PROJETIL;
    // tamanho->ap3 = al_get_bitmap_height(projetil3); 
    // tamanho->lp4 = al_get_bitmap_width(projetil4) / FRAMES_PROJETIL;
    // tamanho->lp4 = al_get_bitmap_height(projetil4); 
    // tamanho->lpb1 = al_get_bitmap_width(projetil_boss1) / FRAMES_PROJETIL;
    // tamanho->apb1 = al_get_bitmap_height(projetil_boss1); 
    // tamanho->lpb2 = al_get_bitmap_width(projetil_boss2) / FRAMES_PROJETIL;
    // tamanho->apb2 = al_get_bitmap_height(projetil_boss2); 

    return tamanho;

}

void desenha_projeteis_inimigo(inimigo *inimigo, unsigned short max_x, unsigned short max_y) {
    unsigned char frame_atual, tempo_anim = 0; // Quadro atual, contador troca de quadro
    ALLEGRO_BITMAP *sprite = al_load_bitmap("./sprites/shot_jogador.png");

    unsigned short largura = al_get_bitmap_width(sprite) / FRAMES_PROJETIL;
    unsigned short altura = al_get_bitmap_height(sprite);

    tempo_anim++;
    if (tempo_anim >= TEMPO_POR_FRAME) {
        frame_atual++;
        tempo_anim = 0;
    }

    // Incrementa o contador de disparo do inimigo 2
    inimigo->contador_disparo++;
    if (inimigo->contador_disparo >= inimigo->tempo_disparo) {
        ataque_inimigo(inimigo); // Inimigo dispara projétil
        inimigo->contador_disparo = 0;
    }

    atualiza_projetil(inimigo->projeteis, -1, max_x, max_y);

    nodo_bala *atual = inimigo->projeteis->inicio;
    while (atual) {
        al_draw_bitmap_region(sprite, atual->frame * largura, 0,largura, altura,atual->x - largura / 2, atual->y - altura / 2, 0);
        atual = atual->prox;
    }

}
