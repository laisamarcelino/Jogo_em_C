#ifndef __PISTOLA__
#define __PISTOLA__

#include "balas.h"

#define PISTOLA_COOLDOWN 10 // Tempo de recarga do ataque

typedef struct {
    unsigned char tipo;
    unsigned char timer; // Cadência do tipo, tiros por segundo
    unsigned char dano;
    balas *tiros;
} pistola;

pistola* cria_pistola(unsigned char tipo, unsigned char dano);
balas* tiro_pistola(unsigned short x, unsigned short y, unsigned char trajetoria, pistola *pistola);
void destroi_pistola(pistola *elemento);

#endif