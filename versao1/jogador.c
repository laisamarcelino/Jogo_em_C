#include <stdlib.h>
#include "jogador.h"

jogador* jogador_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){						//Implementação da função "jogador_create" (!)

	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side/2 < 0) || (y + side/2 > max_y)) return NULL;										//Verifica se a posição inicial é válida; caso não seja, retorna NULL (!)

	jogador *new_jogador = (jogador*) malloc(sizeof(jogador));																						//Aloca memória na heap para um novo quadrado (!)
	new_jogador->side = side;																													//Insere o tamanho do lado de um quadrado (!)
	new_jogador->x = x;																															//Insere a posição inicial central de X (!)
	new_jogador->y = y;	
    new_jogador->control = joystick_create();																														//Insere a posição inicial central de Y (!)
	return new_jogador;																															//Retorna o novo quadrado (!)
}

void jogador_move(jogador *element, unsigned char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){					//Implementação da função "jogador_move" (!)

	if (!trajectory){ if ((element->x - steps*JOGADOR_STEP) - element->side/2 >= 0) element->x = element->x - steps*JOGADOR_STEP;} 				//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->x + steps*JOGADOR_STEP) + element->side/2 <= max_x) element->x = element->x + steps*JOGADOR_STEP;}	//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if ((element->y - steps*JOGADOR_STEP) - element->side/2 >= 0) element->y = element->y - steps*JOGADOR_STEP;}		//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->y + steps*JOGADOR_STEP) + element->side/2 <= max_y) element->y = element->y + steps*JOGADOR_STEP;}	//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
}

void jogador_destroy(jogador *element){
    joystick_destroy(element->control);																											//Implementação da função "square_destroy" (!)
	free(element);																																//Libera a memória do quadrado na heap (!)
}
