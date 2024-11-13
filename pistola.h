#ifndef __PISTOLA__
#define __PISTOLA__

#include "balas.h"

#define PISTOLA_COOLDOWN 10 // Tempo de recarga do ataque especial

typedef struct {
    unsigned char tipo;
    unsigned char timer; // Cadência do tipo, tiros por segundo
    unsigned char dano;
    balas *tiros;
} pistola;

#endif