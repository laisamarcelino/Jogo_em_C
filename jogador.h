#ifndef __JOGADOR__ 
#define __JOGADOR__

#include "joystick.h"
#include "projeteis.h"
#include "stdbool.h"

#define PASSOS_JOGADOR 10

typedef struct {
    unsigned char hp; // Quantidade de vida
    unsigned char largura;
    unsigned char altura;
    unsigned char tipo_ataque;
    unsigned short x; // Posicao x do centro do jogador
	unsigned short y; // Posicao y do centro do jogador
	joystick *controle; // Controle do jogador
	projetil_lista *projeteis; // Lista de projéteis do jogador
} jogador;

jogador* cria_jogador(unsigned char hp, unsigned short largura, unsigned short altura, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void mov_jogador(jogador* jog, char passos, unsigned short max_x, unsigned short max_y);
void ataque_jogador(jogador *jog);
void destroi_jogador(jogador *jog);
void desenha_jogador(jogador *jog);
void desenha_projeteis_jog(jogador *jog, unsigned short max_x, unsigned short max_y);

#endif																									
