#ifndef __BALAS__
#define __BALAS__

#define MOVIMENTO_BALA 5 // ?

typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned char trajetoria;
    struct balas *prox;
} balas;




#endif