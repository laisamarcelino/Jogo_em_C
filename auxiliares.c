#include "auxiliares.h"
#include "jogador.h"

#include <stdio.h>

void imprime_jogador(jogador *jog){
    printf("%u %u %u %u\n", jog->hp, jog->tam_lado, jog->x, jog->y);
}

