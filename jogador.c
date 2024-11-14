#include "jogador.h"
#include "joystick.h"

#include <stdio.h>
#include <stdlib.h>

jogador* cria_jogador(unsigned char hp, unsigned char tam_lado, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){
    
    // Verifica se a posição inicial eh valida
    // Verifica se a face principal eh valida 

    jogador *novo_jogador = (jogador*)malloc(sizeof(jogador));
    if (!novo_jogador) 
        return NULL;
    
    novo_jogador->hp = hp;
    novo_jogador->tam_lado = tam_lado;
    novo_jogador->face = face;
    novo_jogador->x = x;
    novo_jogador->y = y;
    novo_jogador->controle = cria_joystick();
    //novo_jogador->pistola = cria_pistola();
    //novo_jogador->especial = cria_especial();

    return novo_jogador;
}

void mov_jogador(jogador* jog, char passos, unsigned short max_x, unsigned short max_y) {
    if (jog->controle->cima && jog->y > jog->tam_lado / 2) {
        jog->y -= passos * PASSOS_JOGADOR;
    }
    else if (jog->controle->baixo && jog->y < max_y - jog->tam_lado / 2) {
        jog->y += passos * PASSOS_JOGADOR;
    }
    else if (jog->controle->esquerda && jog->x > jog->tam_lado / 2) {
        jog->x -= passos * PASSOS_JOGADOR;
    }
    else if (jog->controle->direita && jog->x < max_x - jog->tam_lado / 2) {
        jog->x += passos * PASSOS_JOGADOR;
    }
}

void ataque_jogador(jogador *jog);
void especial_jogador(jogador *jog);
void destroi_jogador(jogador *jog);