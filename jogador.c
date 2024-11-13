#include "jogador.h"
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

void mov_jogador(jogador *jog, char passos, unsigned char chave_joystick, unsigned short max_x, unsigned short max_y){
    
}
void ataque_jogador(jogador *jog);
void especial_jogador(jogador *jog);
void destroi_jogador(jogador *jog);