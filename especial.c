#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "especial.h"

especial_jog* cria_especial(unsigned char tipo, unsigned short largura, unsigned short altura, unsigned short x, unsigned short y) {
    especial_jog* novo = (especial_jog*)malloc(sizeof(especial_jog));
    if (!novo) 
        return NULL;

    novo->largura = largura;
    novo->altura = altura;
    novo->x = x;
    novo->y = y;
    novo->ativo = false;
    novo->duracao_especial = 150;


    return novo;
}

void mov_especial (especial_jog* especial, unsigned char passos, unsigned char max_x, unsigned char max_y){
    especial->x -= passos * PASSOS_ESPECIAL;
}

void destroi_especial(especial_jog *especial){
    free(especial);
    especial = NULL;
}

void desenha_especial(especial_jog *especial, jogador *player){
    ALLEGRO_BITMAP *especial1 = al_load_bitmap("./sprites/especial1.png");
    ALLEGRO_BITMAP *especial2 = al_load_bitmap("./sprites/especial2.png");

    if (!especial1 || !especial2) {
        fprintf(stderr, "Erro ao carregar sprites de especial.\n");
        return;
    }

    unsigned short l1 = al_get_bitmap_width(especial1) / 8;
    unsigned short a1 = al_get_bitmap_height(especial1);
    unsigned short l2 = al_get_bitmap_width(especial2) / 8;
    unsigned short a2 = al_get_bitmap_height(especial2);

    static unsigned char frame_atual = 0;
    static unsigned short tempo_animacao = 0; 

 tempo_animacao++;

    if (tempo_animacao >= TEMPO_POR_FRAME) {
        frame_atual = (frame_atual + 1) % 8; // Cicla entre os quadros (0 a 7)
        tempo_animacao = 0;
    }

    switch (player->tipo_ataque) {
        case 1:
            al_draw_bitmap_region(especial1, frame_atual * l1, 0, l1, a1, especial->x - l1 / 2, especial->y - a1 / 2, 0);
            break;

        case 2:
            al_draw_bitmap_region(especial2, frame_atual * l2, 0, l2, a2, especial->x - l2 / 2, especial->y - a2 / 2, 0);
            break;

        default:
            break;
    }

   // al_destroy_bitmap(especial1);
    //al_destroy_bitmap(especial2);
}