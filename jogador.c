#include "jogador.h"

#include <stdio.h>
#include <stdlib.h>

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
    if (jog->controle->cima && jog->y > jog->altura / 2) {
        jog->y -= passos * PASSOS_JOGADOR;
    }
    else if (jog->controle->baixo && jog->y < max_y - jog->altura / 2) {
        jog->y += passos * PASSOS_JOGADOR;
    }
    else if (jog->controle->esquerda && jog->x > jog->largura / 2) {
        jog->x -= passos * PASSOS_JOGADOR;
    }
    else if (jog->controle->direita && jog->x < max_x - jog->largura / 2) {
        jog->x += passos * PASSOS_JOGADOR;
    }
}

void ataque_jogador(jogador *jog) {
    unsigned short x_tiro = jog->x + jog->largura / 2; // Inicia na frente do jogador
    unsigned short y_tiro = jog->y; // Na mesma altura do jogador
    unsigned char trajetoria = 1; // Direção do projetil 
    unsigned char dano = 1;

    insere_bala(jog->projeteis, x_tiro, y_tiro, trajetoria, dano);
}

void especial_jogador(jogador *jog);

void destroi_jogador(jogador *jog){
    destroi_projetil_lista(&(jog->projeteis));
    destroi_joystick(jog->controle);
    free(jog);
}