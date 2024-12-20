#ifndef __INIMIGOS__ 
#define __INIMIGOS__

#include "projeteis.h"
#include "jogador.h"

#define PASSOS_INIMIGO 5
#define MAX_INIMIGOS 15
#define FRAMES_INIMIGO 3

typedef struct {
    unsigned char tipo;
    unsigned char hp; // Quantidade de vida
    unsigned char largura;
    unsigned char altura;
    unsigned char dano; // Dano ao encostar no personagem
    unsigned short x; // Posição x do centro do personagem
	unsigned short y; // Posição y do centro do personagem
    projetil_lista *projeteis;	
	unsigned short tempo_disparo; // Intervalo entre disparos
    unsigned short contador_disparo; // Contador para controle
    unsigned short delay_contador; // Atraso para desenhar este inimigo
} inimigo;

typedef struct nodo_inimigo {
    inimigo *chave_inimigo;
    struct nodo_inimigo *prox;
} nodo_inimigo;

typedef struct {
    nodo_inimigo *ini;
    struct nodo_inimigo *ptr; /* ponteiro para algum nodo da lista (iterador) */
} lista_inimigo;

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
unsigned short aleat (unsigned short min, unsigned short max);
void mov_inimigo(lista_inimigo* lista, char passos, unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y);
void ataque_inimigo(inimigo *inimigo);
void especial_inimigo(inimigo *inimigo);
void destroi_inimigo(inimigo *inimigo);
lista_inimigo* cria_lista_inimigo();
void insere_inimigo(lista_inimigo *lista, inimigo* chave_inimigo);
lista_inimigo* gera_lista_inimigos(unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y);
void destroi_lista_inimigos(lista_inimigo* lista);
//void desenha_inimigos(lista_inimigo *lista, unsigned short max_x, unsigned short max_y);
void desenha_inimigo(lista_inimigo *lista, unsigned short max_x, unsigned short max_y);
void lista_inicia_iterador (lista_inimigo *lista);
unsigned char lista_incrementa_iterador (lista_inimigo *lista, inimigo **chave_inimigo);


//void manipula_inimigos (lista_inimigo* lista, unsigned short max_x, unsigned short max_y);

#endif