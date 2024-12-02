#ifndef __PROJETEIS__
#define __PROJETEIS__ 

#define VEL_BALA 15
#define TEMPO_POR_FRAME 6
#define FRAMES_PROJETIL 3

typedef struct nodo_bala {
    unsigned short x;
    unsigned short y;
    unsigned char dano;
    unsigned char frame;
    unsigned char tempo_anim;
    struct nodo_bala *prox;
} nodo_bala;

typedef struct {
    nodo_bala *inicio;
    unsigned char tamanho;
} projetil_lista;

projetil_lista *cria_projetil_lista();
void destroi_projetil_lista(projetil_lista **lista);
void insere_bala(projetil_lista *lista, unsigned short x, unsigned short y, unsigned char dano);
void lista_remove_inicio(projetil_lista *lista);

// Atualiza a posição dos projéteis e remove os que sairam da tela
void atualiza_projetil(projetil_lista *lista, short trajetoria, unsigned short max_x, unsigned short max_y);


#endif