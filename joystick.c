#include "joystick.h"
#include <stdlib.h>
#include <stdio.h>

joystick* cria_joystick(){
    joystick *elemento = (joystick*) malloc (sizeof(joystick));
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
void atualiza_joystick(joystick* controle, unsigned char chave_joystick[]) {
    controle->cima = chave_joystick[0];
    controle->baixo = chave_joystick[1];
    controle->esquerda = chave_joystick[2];
    controle->direita = chave_joystick[3];
    controle->tiro = chave_joystick[4];
    controle->especial = chave_joystick[5];
}

void destroi_joystick(joystick* controle){
    free(controle);
}