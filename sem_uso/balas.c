#include "balas.h"
#include <stdio.h>
#include <stdlib.h>

balas* cria_balas(unsigned short x, unsigned short y, unsigned char trajetoria, balas *prox){
    if (trajetoria < 0 || trajetoria > 1)
        return NULL;
    
    balas *new_balas = (balas*) malloc(sizeof(balas));
	if (!new_balas) 
        return NULL;
        	
	new_balas->x = x;
	new_balas->trajetoria = trajetoria;
	new_balas->prox = (struct balas*) prox;			

	return new_balas;	
}

void mov_balas(balas *elemento){
    for (balas *index = elemento; index != NULL; index = (balas*) index->prox){
        if(!index->trajetoria)
            index->x = index->x - MOVIMENTO_BALA;
        else
            index->x = index->x + MOVIMENTO_BALA;
    }
}

void destroi_balas(balas *elemento){
    free(elemento);
}