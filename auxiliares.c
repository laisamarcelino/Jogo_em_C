#include "auxiliares.h"
#include "jogador.h"

#include <stdio.h>

void imprime_jogador(jogador *jog){
    printf("%u %u %u %u %u\n", jog->hp, jog->tam_lado, jog->face, jog->x, jog->y);
}

void atualiza_posicao(jogador* jog, unsigned short max_x, unsigned short max_y) {
    mov_jogador(jog, 1, max_x, max_y);
}