#include "jogador.h"
#include "projeteis.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define FRAMES_JOGADOR 5

jogador* cria_jogador(unsigned char hp, unsigned short largura, unsigned short altura, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y) {
    jogador *novo_jogador = (jogador*)malloc(sizeof(jogador));
    if (!novo_jogador) 
        return NULL;
    
    novo_jogador->hp = hp;
    novo_jogador->largura = largura;
    novo_jogador->altura = altura;
    novo_jogador->x = x;
    novo_jogador->y = y;
    novo_jogador->tipo_ataque = 0;
    novo_jogador->controle = cria_joystick();
    novo_jogador->projeteis = cria_projetil_lista();

    return novo_jogador;
}


void mov_jogador(jogador* jog, char passos, unsigned short max_x, unsigned short max_y) {
    if (jog->controle->cima && jog->y > jog->altura / 2) 
        jog->y -= passos * PASSOS_JOGADOR;
    
    if (jog->controle->baixo && jog->y < max_y - jog->altura / 2) 
        jog->y += passos * PASSOS_JOGADOR;
    
    if (jog->controle->esquerda && jog->x > jog->largura / 2) 
        jog->x -= passos * PASSOS_JOGADOR;
    
    if (jog->controle->direita && jog->x < max_x - jog->largura / 2) 
        jog->x += passos * PASSOS_JOGADOR;
    
}

void ataque_jogador(jogador *jog) {
    unsigned short x_tiro = jog->x + jog->largura / 2; // Inicia na frente do jogador
    unsigned short y_tiro = jog->y; // Na mesma altura do jogador
    unsigned char dano;

    switch (jog->tipo_ataque){
        case 0: // Ataque padrão
            dano = 1; 
            break;
        case 1: // Especial 1
            dano = 3; 
            break;
        case 2: // Especial 2
            dano = 5;
            break;
        default:
            dano = 1;
            break;
        }
    insere_bala(jog->projeteis, x_tiro-50, y_tiro+20, dano);
}

void destroi_jogador(jogador *jog){
    destroi_projetil_lista(&(jog->projeteis));
    destroi_joystick(jog->controle);
    free(jog);
}

void desenha_jogador(jogador *jog) {
    ALLEGRO_BITMAP *sprites_jogador = al_load_bitmap("./sprites/jogador.png");
    unsigned char frame_atual, tempo_anim = 0; // Quadro atual, contador troca de quadro
    
    if (!sprites_jogador) {
        fprintf(stderr, "Erro ao carregar sprites do jogador.\n");
        return;
    }
    
    al_convert_mask_to_alpha(sprites_jogador, al_map_rgb(255, 0, 255));

    unsigned short largura_quadro_jogador = al_get_bitmap_width(sprites_jogador) / FRAMES_JOGADOR;
    unsigned short altura_quadro_jogador = al_get_bitmap_height(sprites_jogador);

    if (jog->hp >= 0){

        tempo_anim++;
        if (tempo_anim >= TEMPO_POR_FRAME) {
            frame_atual++;
            tempo_anim = 0;
        }

        // Alterna os frames conforme o movimento
        if (jog->controle->baixo) {
            if (frame_atual < 1 || frame_atual > 2)
                frame_atual = 1;
        } 
        else if (jog->controle->cima) {
            if (frame_atual < 3 || frame_atual > 4)
                frame_atual = 3;
        } 
        else 
            frame_atual = 0;

        // Desenha o jogador
        al_draw_bitmap_region(
            sprites_jogador,
            frame_atual * largura_quadro_jogador, 0,
            largura_quadro_jogador, altura_quadro_jogador,
            jog->x - largura_quadro_jogador / 2, jog->y - altura_quadro_jogador / 2, 0
        );
    }
}

void desenha_projeteis_jog(jogador *jog, unsigned short max_x, unsigned short max_y) {
    ALLEGRO_BITMAP *sp_p = al_load_bitmap("./sprites/shot_jogador.png");
    ALLEGRO_BITMAP *sp_e1 = al_load_bitmap("./sprites/projetil_boss1.png");
    ALLEGRO_BITMAP *sp_e2 = al_load_bitmap("./sprites/projetil_boss2.png");

    if (!sp_p || !sp_e1 || !sp_e2) {
        fprintf(stderr, "Erro ao carregar sprites dos projeteis do jogador.\n");
        return;
    }

    unsigned short l_p = al_get_bitmap_width(sp_p) / FRAMES_PROJETIL;
    unsigned short a_p = al_get_bitmap_height(sp_p);
    unsigned short l_e1 = al_get_bitmap_width(sp_e1) / FRAMES_PROJETIL;
    unsigned short a_e1 = al_get_bitmap_height(sp_e1);
    unsigned short l_e2 = al_get_bitmap_width(sp_e2) / FRAMES_PROJETIL;
    unsigned short a_e2 = al_get_bitmap_height(sp_e2);
   
    atualiza_projetil(jog->projeteis, 1, max_x, max_y);

    // Desenha os projéteis com animação
    nodo_bala *atual = jog->projeteis->inicio;
    
    while (atual) {
        switch (jog->tipo_ataque) {
        case 0:
            al_draw_bitmap_region(sp_p, atual->frame * l_p, 0, l_p, a_p,atual->x - l_p / 2, atual->y - a_p / 2, 0);
            break;
        case 1:
            al_draw_bitmap_region(sp_e1, atual->frame * l_e1, 0, l_p, a_e1,atual->x - l_e1 / 2, atual->y - a_e1 / 2, 0);
            break;
        case 2:
            al_draw_bitmap_region(sp_e2, atual->frame * l_e2, 0, l_e2, a_p,atual->x - l_e2 / 2, atual->y - a_e2 / 2, 0);
            break;
        
        default:
            al_draw_bitmap_region(sp_p, atual->frame * l_p, 0, l_p, a_p,atual->x - l_p / 2, atual->y - a_p / 2, 0);
            break; 
        }

        atual = atual->prox;
    }
}