#include "jogador.h"
#include "projeteis.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define FRAMES_JOGADOR 5
#define TEMPO_POR_FRAME 5

jogador* cria_jogador(unsigned char hp, unsigned char largura, unsigned char altura, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){
    
    // Verifica se a posição inicial eh valida
    // Verifica se a face principal eh valida 

    jogador *novo_jogador = (jogador*)malloc(sizeof(jogador));
    if (!novo_jogador) 
        return NULL;
    
    novo_jogador->hp = hp;
    novo_jogador->largura = largura;
    novo_jogador->altura = altura;
    novo_jogador->x = x;
    novo_jogador->y = y;
    novo_jogador->controle = cria_joystick();
    novo_jogador->projeteis = cria_projetil_lista(); 
    //novo_jogador->especial = cria_especial();

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
    unsigned char dano = 1;

    insere_bala(jog->projeteis, x_tiro, y_tiro, dano);
}

void especial_jogador(jogador *jog);

void destroi_jogador(jogador *jog){
    destroi_projetil_lista(&(jog->projeteis));
    destroi_joystick(jog->controle);
    free(jog);
}

void manipula_jogador (jogador *jog, unsigned short max_x, unsigned short max_y){
    unsigned char frame_atual, tempo_anim = 0; // Quadro atual, contador troca de quadro

    ALLEGRO_BITMAP *sprites_jogador = al_load_bitmap("./sprites/jogador.png");
    if (!sprites_jogador){
        fprintf(stderr, "Erro ao carregar sprites do jogador.\n");
        return;
    }
    
    // Converte a cor magenta (255, 0, 255) para transparência
    al_convert_mask_to_alpha(sprites_jogador, al_map_rgb(255, 0, 255));

    // Calcula a largura e altura de cada quadro da sprite
    unsigned short largura_quadro = al_get_bitmap_width(sprites_jogador) / FRAMES_JOGADOR;
    unsigned short altura_quadro = al_get_bitmap_height(sprites_jogador);

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
        } else if (jog->controle->cima) {
            if (frame_atual < 3 || frame_atual > 4)
                frame_atual = 3;
        } else {
            frame_atual = 0;
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap_region(
            sprites_jogador,
            frame_atual * largura_quadro, 0,
            largura_quadro, altura_quadro,
            jog->x - largura_quadro / 2, jog->y - altura_quadro / 2, 0
        );

        //al_draw_filled_rectangle(jog->x - jog->largura / 2, jog->y - jog->altura / 2, jog->x + jog->largura / 2, jog->y + jog->altura / 2, al_map_rgb(255, 0, 0));
        mov_jogador(jog, 1, max_x, max_y);
        atualiza_projetil(jog->projeteis, 1, max_x, max_y);

        // Desenha projéteis do jogador
        nodo_bala *atual = jog->projeteis->inicio;
        while (atual) {
            al_draw_filled_circle(atual->x, atual->y, 5, al_map_rgb(255, 255, 0)); // Amarelo
            atual = atual->prox;
        }
    }  
}