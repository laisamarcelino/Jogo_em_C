#ifndef __JOGADOR__ 
#define __JOGADOR__

#include "joystick.h"
#include "projeteis.h"
#include "especial.h"

#define PASSOS_JOGADOR 10

typedef struct {
    unsigned char hp; // Quantidade de vida
    unsigned char largura;
    unsigned char altura;
    unsigned short x; // Posicao x do centro do jogador
	unsigned short y; // Posicao y do centro do jogador
	joystick *controle; // Controle do jogador
	projetil_lista *projeteis; // Lista de projéteis do jogador
    especial *especial; // Ataque especial
} jogador;

jogador* cria_jogador(unsigned char hp, unsigned char largura, unsigned char altura, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void mov_jogador(jogador* jog, char passos, unsigned short max_x, unsigned short max_y);
void ataque_jogador(jogador *jog);
void especial_jogador(jogador *jog);
void destroi_jogador(jogador *jog);

#endif																									
