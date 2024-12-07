#ifndef __ESPECIAL__
#define __ESPECIAL__

#include "stdbool.h"
#include "jogador.h"

#define PASSOS_ESPECIAL 10

typedef struct {
    unsigned char largura;
    unsigned char altura;
    unsigned short x;
    unsigned short y;
    unsigned char tipo_ataque;
    unsigned int duracao_especial;
    bool ativo;
} especial_jog;

especial_jog* cria_especial(unsigned char tipo, unsigned short largura, unsigned short altura, unsigned short x, unsigned short y);
void desenha_especial(especial_jog *especial, jogador *player);
void mov_especial (especial_jog* especial, unsigned char passos, unsigned char max_x, unsigned char max_y);
void destroi_especial(especial_jog *especial);

#endif