#include "auxiliares.h"
#include "jogador.h"
#include "inimigos.h"

#include <stdio.h>

void imprime_jogador(jogador *jog){
    printf("%u %u %u %u %u\n", jog->hp, jog->altura, jog->largura, jog->x, jog->y);
}



