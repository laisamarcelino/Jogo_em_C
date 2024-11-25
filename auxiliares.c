#include "auxiliares.h"
#include "jogador.h"
#include "inimigos.h"

#include <stdio.h>

void imprime_jogador(jogador *jog){
    printf("%u\n", jog->hp);
}

void imprime_inimigo(inimigo *ini){
    printf("%u\n", ini->hp);

}
