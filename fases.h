#ifndef FASES_H
#define FASES_H

#include "jogador.h"
#include "inimigos.h"
#include <allegro5/allegro.h>

#define MAX_INIMIGOS 18

//inimigo* cria_inimigos (unsigned short max_x, unsigned short max_y);
void fase1 (ALLEGRO_TIMER* timer, jogador* player, inimigo* inimigos[], tamanho *tamanho, unsigned short max_x, unsigned short max_y);
void fase2(ALLEGRO_TIMER* timer, jogador* player, unsigned short max_x, unsigned short max_y);
void atualiza_fase(ALLEGRO_TIMER* timer, unsigned char* fase_atual);

#endif
