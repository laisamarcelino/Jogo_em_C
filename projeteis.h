#ifndef __PROJETEIS__
#define __PROJETEIS__ 

#define VEL_BALA 10

typedef struct nodo_bala {
    unsigned short x;
    unsigned short y;
    unsigned char trajetoria;
    unsigned char dano;
    struct nodo_bala *prox;
} nodo_bala;

typedef struct {
    nodo_bala *inicio;
    nodo_bala *ptr; // Ponteiro para algum nodo_bala da lista (iterador)
    unsigned char tamanho;
} projetil_lista;

projetil_lista *cria_projetil_lista();
void destroi_projetil_lista(projetil_lista **lista);
void insere_bala(projetil_lista *lista, unsigned short x, unsigned short y, unsigned char trajetoria, unsigned char dano);
void lista_remove_inicio(projetil_lista *lista);

// Atualiza a posição dos projéteis e remove os que sairam da tela
void atualiza_projetil(projetil_lista *lista, unsigned short max_x, unsigned short max_y);


#endif