#ifndef FASES_H
#define FASES_H

#include "jogador.h"
#include "inimigos.h"
#include <allegro5/allegro.h>

#define MAX_INIMIGOS 100

unsigned char verifica_colisao_players(jogador *player, inimigo *inimigo, unsigned short max_x);
unsigned char colisao_projeteis(nodo_bala *projetil, jogador *player, inimigo *inimigo);
unsigned char verifica_colisao_projeteis(jogador *player, inimigo *inimigo, unsigned short max_x);


ALLEGRO_BITMAP* get_sprite(unsigned char tipo, infos_inimigos* infos_inimigos);
void fase1(ALLEGRO_TIMER *timer, jogador *player, inimigo *inimigos[], infos_inimigos *infos_inimigos, unsigned short max_x, unsigned short max_y);
void fase2(ALLEGRO_TIMER* timer, jogador* player, unsigned short max_x, unsigned short max_y);
void atualiza_fase(ALLEGRO_TIMER* timer, unsigned char* fase_atual);

#endif
