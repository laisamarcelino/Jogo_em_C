#include "joystick.h"
#include <stdio.h>

joystick* cria_joystick(){
    joystick *elemento = (joystick*) malloc (sizeof(joystick));						//Aloca memória na heap para um novo controle
	if (!elemento) 
        return NULL;	
 
	elemento->cima = 0;	
	elemento->baixo = 0;
	elemento->esquerda = 0;
	elemento->direita = 0;
	elemento->tiro = 0;
    elemento->especial = 0;

	return elemento;	
}
joystick* atualiza_joystick(joystick *elemento, unsigned char chave_joystick[]){
    if (chave_joystick[0])
        elemento->cima = 1;
    else if (chave_joystick[1])
        elemento->baixo = 1;
    else if (chave_joystick[2])
        elemento->esquerda = 1;
    else if (chave_joystick[3])
        elemento->direita = 1;
    else
        return;

    return elemento;
}
