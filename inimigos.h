#ifndef __INIMIGOS__ 
#define __INIMIGOS__

#include "projeteis.h"
#define PASSOS_INIMIGO 5

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
} inimigo;

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
unsigned short aleat (unsigned short min, unsigned short max);
void mov_inimigo(inimigo* inimigo, char passos, unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y);
void ataque_inimigo(inimigo *inimigo);
void especial_inimigo(inimigo *inimigo);
void destroi_inimigo(inimigo *inimigo);



#endif