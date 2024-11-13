#ifndef __JOGADOR__ 
#define __JOGADOR__

#include "joystick.h"
#include "pistola.h"
#include "especial.h"
typedef struct {
    unsigned char hp; // Quantidade de vida
    unsigned char lado; // Tamanho da lateral
    unsigned char face; // Face frontal 
    unsigned short x; // Posição x do centro do jogador
	unsigned short y; // Posição y do centro do jogador
	joystick *controle; // Controle do jogador
	pistola *pistola; // Ataque normal 
    especial *especial; // Ataque especial

} jogador;



#endif																									
