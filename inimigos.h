#ifndef __INIMIGOS__ 
#define __INIMIGOS__

#include "pistola.h" 

typedef struct {
    unsigned char tipo;
    unsigned char hp; // Quantidade de vida
    unsigned char lado; // Tamanho da lateral
    unsigned char face; // Face frontal 
    unsigned short x; // Posição x do centro do personagem
	unsigned short y; // Posição y do centro do personagem	
    unsigned char dano; // Dano ao encostar no personagem	
	// Movimentação pré-definida do personagem	
	pistola *pistola;

} inimigo;



#endif