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
    novo_inimigo->tempo_disparo = 30; // Dispara a cada 30 frames
    novo_inimigo->contador_disparo = 0;
    novo_inimigo->frame_atual = 0;
    novo_inimigo->tempo_anim = 0;
    novo_inimigo->frame_atual_proj = 0;
    novo_inimigo->tempo_anim_proj = 0;

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
            inimigo->x -= passos * PASSOS_INIMIGO-3;
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
}

void ataque_inimigo(inimigo *inimigo){
    unsigned char dano = 1; // Dano do projétil

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
            dano = 1;
            break;
    }

    insere_bala(inimigo->projeteis, x_tiro, y_tiro, dano);
}

void especial_inimigo(inimigo *inimigo);

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);  
}


void desenha_inimigo(ALLEGRO_BITMAP *sprite, inimigo *inimigo, unsigned short largura, unsigned short altura) {
    if (!sprite) {
        fprintf(stderr, "Sprite é NULL em desenha_inimigo.\n");
        return;
    }

    inimigo->tempo_anim++;
    if (inimigo->tempo_anim >= TEMPO_POR_FRAME) {
        inimigo->frame_atual = (inimigo->frame_atual + 1) % FRAMES_INIMIGO;
        inimigo->tempo_anim = 0;
    }

    al_draw_bitmap_region(
        sprite,
        inimigo->frame_atual * largura, 0,
        largura, altura,
        inimigo->x - largura / 2, inimigo->y - altura / 2, 0
    );
}

infos_inimigos* carrega_sprites() {
    infos_inimigos *novo_infos = (infos_inimigos*)malloc(sizeof(infos_inimigos));

    inimigo *novo_inimigo = (inimigo*)malloc(sizeof(inimigo));

    novo_infos->inimigo1 = NULL;
    novo_infos->inimigo2 = NULL;
    novo_infos->inimigo3 = NULL;
    novo_infos->inimigo4 = NULL;

    novo_infos->inimigo1 = al_load_bitmap("./sprites/inimigo1.png");
    novo_infos->inimigo2 = al_load_bitmap("./sprites/inimigo2.png");
    novo_infos->inimigo3 = al_load_bitmap("./sprites/inimigo3.png");
    novo_infos->inimigo4 = al_load_bitmap("./sprites/inimigo4.png");
    novo_infos->boss1 = al_load_bitmap("./sprites/boss1.png");
    // novo_infos->boss2 = al_load_bitmap("./sprites/boss2.png");
    novo_infos->projetil2 = al_load_bitmap("./sprites/projetil1.png");
    // novo_infos->projetil3 = al_load_bitmap("./sprites/projetil3.png");
    // novo_infos->projetil4 = al_load_bitmap("./sprites/projetil4.png");
    // novo_infos->projetil_boss1 = al_load_bitmap("./sprites/iprojetil_boss1.png");
    // novo_infos->projetil_boss2 = al_load_bitmap("./sprites/projetil_boss2.png");
    /*
    if (!inimigo1 || !inimigo2 || !inimigo3 || !inimigo4 || !boss1 || !boss2 || !projetil2 || !projetil3 || !projetil4 || !projetil_boss1 || !projetil_boss2) {
        fprintf(stderr, "Erro ao carregar sprites dos inimigos.\n");
        return NULL;
    }*/

    // Calcula a largura e altura dos inimigos
    novo_infos->l1 = al_get_bitmap_width(novo_infos->inimigo1) / 3;
    novo_infos->a1 = al_get_bitmap_height(novo_infos->inimigo1);
    novo_infos->l2 = al_get_bitmap_width(novo_infos->inimigo2) / 5;
    novo_infos->a2 = al_get_bitmap_height(novo_infos->inimigo2);
    novo_infos->l3 = al_get_bitmap_width(novo_infos->inimigo3) / 6;
    novo_infos->a3 = al_get_bitmap_height(novo_infos->inimigo3);
    novo_infos->l4 = al_get_bitmap_width(novo_infos->inimigo4) / 3;
    novo_infos->a4 = al_get_bitmap_height(novo_infos->inimigo4);
    novo_infos->lb1 = al_get_bitmap_width(novo_infos->boss1) / 6;
    novo_infos->ab1 = al_get_bitmap_height(novo_infos->boss1) / 2;
    // novo_infos->lb2 = al_get_bitmap_width(boss2) / FRAMES_INIMIGO;
    // novo_infos->ab2 = al_get_bitmap_height(boss2);

    // Calcula a largura e altura dos projeteis
    novo_infos->lp2 = al_get_bitmap_width(novo_infos->projetil2) / 8;
    novo_infos->ap2 = al_get_bitmap_height(novo_infos->projetil2); 
    // novo_infos->lp3 = al_get_bitmap_width(projetil3) / FRAMES_PROJETIL;
    // novo_infos->ap3 = al_get_bitmap_height(projetil3); 
    // novo_infos->lp4 = al_get_bitmap_width(projetil4) / FRAMES_PROJETIL;
    // novo_infos->lp4 = al_get_bitmap_height(projetil4); 
    // novo_infos->lpb1 = al_get_bitmap_width(projetil_boss1) / FRAMES_PROJETIL;
    // novo_infos->apb1 = al_get_bitmap_height(projetil_boss1); 
    // novo_infos->lpb2 = al_get_bitmap_width(projetil_boss2) / FRAMES_PROJETIL;
    // novo_infos->apb2 = al_get_bitmap_height(projetil_boss2); 

    return novo_infos;

}

void desenha_projeteis_inimigo(inimigo *inimigo, unsigned short max_x, unsigned short max_y, infos_inimigos *infos_inimigos) {
    if (!infos_inimigos->projetil2) {
        fprintf(stderr, "Sprite de projéteis do inimigo é NULL.\n");
        return;
    }

    inimigo->tempo_anim_proj++;
    if (inimigo->tempo_anim_proj >= TEMPO_POR_FRAME) {
        inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % FRAMES_PROJETIL;
        inimigo->tempo_anim_proj = 0;
    }

    if (inimigo->hp > 0){
        inimigo->contador_disparo++;
        if (inimigo->contador_disparo >= inimigo->tempo_disparo) {
            ataque_inimigo(inimigo);
            inimigo->contador_disparo = 0;
        }
    }

    atualiza_projetil(inimigo->projeteis, -1, max_x, max_y);

    unsigned short largura = infos_inimigos->lp2;
    unsigned short altura = infos_inimigos->ap2;

    nodo_bala *atual = inimigo->projeteis->inicio;
    while (atual) {
        al_draw_bitmap_region(infos_inimigos->projetil2, inimigo->frame_atual_proj * largura, 0, largura, altura, atual->x - largura / 2, atual->y - altura / 2, 0);
        atual = atual->prox;
    }
}


