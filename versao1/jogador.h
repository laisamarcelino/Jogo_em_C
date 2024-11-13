//square.h

#ifndef __JOGADOR__ 																												
#define __JOGADOR__		

#include "joystick.h"
#define JOGADOR_STEP 20				

//Definição de um quadrado
typedef struct {																											
	unsigned char side;																											
	unsigned short x;																											
	unsigned short y;
	joystick *control;																											
} jogador;							

// Cria o jogador --- !!
jogador* jogador_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);

// Define a movimentação do jogador		
void jogador_move(jogador *element, unsigned char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);	

// Destroi a estrutura do jogador 
void jogador_destroy(jogador *element);																				

#endif	