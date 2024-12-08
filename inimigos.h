#ifndef __INIMIGOS__ 
#define __INIMIGOS__

#include "projeteis.h"
#include "jogador.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define PASSOS_INIMIGO 8
#define FRAMES_INIMIGO 3

typedef struct {
    unsigned char tipo;
    unsigned char hp; // Quantidade de vida
    unsigned char largura;
    unsigned char altura;
    unsigned char dano; // Dano ao encostar no personagem
    unsigned short x; // Posição x do centro do personagem
    unsigned short y; // Posição y do centro do personagem
    projetil_lista *projeteis;	
    unsigned short tempo_disparo; // Intervalo entre disparos
    unsigned short contador_disparo; // Contador para controle
    unsigned char frame_atual; // Frame atual da animação
    unsigned char tempo_anim; // Contador para troca de frame
    unsigned char frame_atual_proj; // Frame atual da animação dos projeteis
    unsigned char tempo_anim_proj; // Contador para troca de frame dos projeteis
} inimigo;

typedef struct {
    ALLEGRO_BITMAP *inimigo1, *inimigo2, *inimigo3, *inimigo4;
    ALLEGRO_BITMAP *boss1, *boss2;
    ALLEGRO_BITMAP *projetil2, *projetil3, *projetil4, *projetil_boss1, *projetil_boss2; 
    unsigned short l1, a1, l2, a2, l3, a3, l4, a4, lb1, ab1, lb2, ab2;
    unsigned short lp2, lp3, lp4, lpb1, lpb2, ap2, ap3, ap4, apb1, apb2;
} infos_inimigos;

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
unsigned short aleat (unsigned short min, unsigned short max);
void mov_inimigo(inimigo *inimigo, char passos, unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y);
void ataque_inimigo(inimigo *inimigo);
void especial_inimigo(inimigo *inimigo);
void destroi_inimigo(inimigo *inimigo);
void desenha_inimigo(ALLEGRO_BITMAP *sprite, inimigo *inimigo, unsigned short largura, unsigned short altura);
void desenha_projeteis_inimigo(inimigo *inimigo, unsigned short max_x, unsigned short max_y, infos_inimigos *infos_inimigos);
infos_inimigos* carrega_sprites();

#endif