#include <stdlib.h>
#include "pistola.h"

pistola* cria_pistola(unsigned char tipo, unsigned char dano){
    pistola *nova_pistola = (pistola*)malloc(sizeof(pistola));
    if (!nova_pistola)
        return NULL;
    
    nova_pistola->tipo = tipo;
    nova_pistola->timer = 0;
    nova_pistola->dano = dano;

    return nova_pistola;
}

balas* tiro_pistola(unsigned short x, unsigned short y, unsigned char trajetoria, pistola *pistola){
    balas *nova_bala = cria_balas(x, y, trajetoria, pistola->tiros);
    if(!nova_bala)
        return NULL;
    
    return nova_bala;
}

void destroi_pistola(pistola *elemento){
    balas *sentinela;

    for (balas *index = elemento->tiros; index != NULL; index = sentinela){
		sentinela = (balas*) index->prox;
		destroi_balas(index);
	}
	free(elemento);
}

