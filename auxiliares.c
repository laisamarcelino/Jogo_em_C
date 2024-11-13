#include "auxiliares.h"

#include <stdio.h>

void imprime_jogador(jogador *jog){
    printf("%u %u %u %u %u\n", jog->hp, jog->tam_lado, jog->face, jog->x, jog->y);
}