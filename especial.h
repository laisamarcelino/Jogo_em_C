#ifndef __ESPECIAL__
#define __ESPECIAL__

#include "balas.h"

typedef struct {
    unsigned char timer; // Cadência de tiro
    unsigned char duracao; // Duração do ataque especial
    unsigned char dano;
    balas *balas;

} especial;


#endif