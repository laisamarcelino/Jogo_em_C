#include "inimigos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, unsigned short x, unsigned short y,  unsigned short max_x, unsigned short max_y){
    
    inimigo *novo_inimigo = (inimigo*)malloc(sizeof(inimigo));
    if (!novo_inimigo)
        return NULL;
    
    novo_inimigo->tipo = tipo;
    novo_inimigo->hp = hp;
    novo_inimigo->largura = largura;
    novo_inimigo->altura = altura;
    novo_inimigo->dano = dano;
    novo_inimigo->x = x;
    novo_inimigo->y = y;
    novo_inimigo->projeteis = cria_projetil_lista();
    novo_inimigo->tempo_disparo = 30; // Dispara a cada 60 frames
    novo_inimigo->contador_disparo = 0;

    return novo_inimigo;
}

unsigned short aleat (unsigned short min, unsigned short max){
    return min + rand() % (max+1 - min);
}

void mov_inimigo(inimigo* inimigo, char passos, unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y) {
    static int fator = 1; // Para nao reiniciar a variavel

    if (!inimigo)
        return;
    
    switch (inimigo->tipo) {
        case 1: 
            inimigo->x -= passos * PASSOS_INIMIGO;
            break;
        case 2:
            inimigo->x -= passos * PASSOS_INIMIGO-2;
            inimigo->y = (int)(max_y / 2 + (sin(inimigo->x * 0.01) * 250)); // Produz um movimento ondulatorio
            break;
        case 3:
            inimigo->x -= passos * PASSOS_INIMIGO-3;
            inimigo->y = (int)(max_y / 2 + (sin(inimigo->x * 0.05) * 100)); 
            break;
        case 4:
            inimigo->y += passos * PASSOS_INIMIGO; // Move para baixo
            break;
        case 5: 
            // Verifica se o inimigo atingiu o limite superior
            if (inimigo->y <= 0 + altura / 2)
                fator = 1;

            // Verifica se o inimigo atingiu o limite inferior
            if (inimigo->y >= max_y - altura / 2)
                fator = -1;

            inimigo->y += passos * PASSOS_INIMIGO * fator;  // Move para cima

            break;

        default:
            break;
    
    }

    // Verifica se o inimigo saiu da tela 
    if (inimigo->x - largura / 2 < 0) {
        inimigo->x = max_x + largura / 2;
        inimigo->y = aleat(largura / 2, max_y - largura / 2);
    }
}

void ataque_inimigo(inimigo *inimigo){
    unsigned char dano;

    unsigned short x_tiro = inimigo->x - inimigo->largura / 2; // Inicia na frente do inimigo
    unsigned short y_tiro = inimigo->y; // Na mesma altura do inimigo
    
    switch (inimigo->tipo) {
        case 2:
            dano = 1;
            break;
        case 3:
            dano = 2;
            break;
        case 4:
            dano = 3;
            break;
        case 5:
            dano = 3;
            break;
        
        default:
            break;
    }

    insere_bala(inimigo->projeteis, x_tiro, y_tiro, dano);
}

void especial_inimigo(inimigo *inimigo);

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);
}


lista_inimigo* cria_lista_inimigo(){
    lista_inimigo *nova_lista = (lista_inimigo*)malloc(sizeof(lista_inimigo));
    if (!nova_lista)
        return NULL;
    
    nova_lista->ini = NULL;

    return nova_lista;
}

void insere_inimigo(lista_inimigo *lista, inimigo* chave_inimigo){
    nodo_inimigo *aux;

    nodo_inimigo *novo_nodo = (nodo_inimigo*)malloc(sizeof(nodo_inimigo));
    if (!novo_nodo) {
        return;
    }
    
    novo_nodo->chave_inimigo = chave_inimigo;
    novo_nodo->prox = NULL;

    if (lista->ini == NULL)
        lista->ini = novo_nodo;
    else {
        aux = lista->ini;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo_nodo;
    }
}

lista_inimigo* gera_lista_inimigos(unsigned char largura, unsigned char altura, unsigned short max_x, unsigned short max_y) { 
    unsigned short i = 1;

    lista_inimigo* lista = cria_lista_inimigo();

    while (i <= MAX_INIMIGOS) {
        unsigned char tipo = aleat(1, 2); 
        unsigned char hp = (tipo == 1) ? 3 : 2;
        unsigned char dano = 1;
        unsigned short x = max_x - 10; 
        unsigned short y = aleat(altura / 2, max_y - altura / 2); 

        inimigo *novo_inimigo = cria_inimigo(tipo, hp, largura, altura, dano, x, y, max_x, max_y);
        if (!novo_inimigo) {
            return NULL; 
        }

        insere_inimigo(lista, novo_inimigo);
        i++; 
    }

    return lista;
}

void destroi_lista_inimigos(lista_inimigo* lista) {
    if (!lista) return;

    nodo_inimigo* atual = lista->ini;
    while (atual) {
        nodo_inimigo* prox = atual->prox;
        destroi_inimigo(atual->chave_inimigo);
        free(atual);
        atual = prox;
    }

    free(lista);
}

void manipula_inimigos (inimigo *inim, unsigned short max_x, unsigned short max_y){
    unsigned char frame_atual, tempo_anim = 0; // Quadro atual, contador troca de quadro
    nodo_inimigo *ini_atual;

    ALLEGRO_BITMAP *sp_1 = al_load_bitmap("./sprites/explosao.png");
    ALLEGRO_BITMAP *sp_2 = al_load_bitmap("./sprites/shot2.png");
    ALLEGRO_BITMAP *sp_proj2 = al_load_bitmap("./sprites/inimigo1.png");
    ALLEGRO_BITMAP *sp_3 = al_load_bitmap("./sprites/shot2.png");
    ALLEGRO_BITMAP *sp_4 = al_load_bitmap("./sprites/jogador.png");
    ALLEGRO_BITMAP *sp_b1 = al_load_bitmap("./sprites/shot2.png");
    ALLEGRO_BITMAP *sp_b2 = al_load_bitmap("./sprites/shot2.png");

    if (!sp_1 || !sp_2 || !sp_3 || !sp_4 || !sp_b1 ||!sp_b2) {
        fprintf(stderr, "Erro ao carregar sprites dos inimigos.\n");
        return;
    }

    // Calcula a largura e altura de cada quadro da sprite
    unsigned short l_sp1 = al_get_bitmap_width(sp_1) / FRAMES_INIMIGO;
    unsigned short a_sp1 = al_get_bitmap_height(sp_1);

    //unsigned short largura_quadro_projetil = al_get_bitmap_width(sprites_projetil) / FRAMES_PROJETIL;
    //unsigned short altura_quadro_projetil = al_get_bitmap_height(sprites_projetil);

    lista_inimigo* lista = gera_lista_inimigos(50, 50, max_x, max_y);

    if (lista->ini == NULL)
        return;

    ini_atual = lista->ini;
    while (ini_atual->prox != NULL) {
        
        if (ini_atual->chave_inimigo->hp >= 0){
            tempo_anim++;
            if (tempo_anim >= TEMPO_POR_FRAME) {
                frame_atual++;
                tempo_anim = 0;
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Desenha o jogador
        al_draw_bitmap_region(
            sp_1,
            frame_atual * l_sp1, 0,
            l_sp1, a_sp1,
            ini_atual->chave_inimigo->x - l_sp1 / 2, ini_atual->chave_inimigo->y - a_sp1 / 2, 0
        );

        mov_inimigo(ini_atual->chave_inimigo, 1, l_sp1, a_sp1, max_x, max_y);

        ini_atual = ini_atual->prox;  
    }
}

