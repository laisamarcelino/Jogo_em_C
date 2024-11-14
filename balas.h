#ifndef __BALAS__
#define __BALAS__

#define MOVIMENTO_BALA 5 

typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned char trajetoria;
    struct balas *prox;
} balas;

balas* cria_balas(unsigned short x, unsigned short y, unsigned char trajetoria, balas *prox);
void mov_balas(balas *elemento);
void destroi_balas(balas *elemento);


#endif