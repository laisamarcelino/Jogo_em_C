#include "inimigos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char tam_lado, unsigned char dano, unsigned short x, unsigned short y,  unsigned short max_x, unsigned short max_y){
    
    inimigo *novo_inimigo = (inimigo*)malloc(sizeof(inimigo));
    if (!novo_inimigo)
        return NULL;
    
    novo_inimigo->tipo = tipo;
    novo_inimigo->hp = hp;
    novo_inimigo->tam_lado = tam_lado;
    novo_inimigo->dano = dano;
    novo_inimigo->x = x;
    novo_inimigo->y = y;
    novo_inimigo->projeteis = cria_projetil_lista();

    return novo_inimigo;
}
unsigned short aleat (unsigned short min, unsigned short max){
    return min + rand() % (max+1 - min);
}

void mov_inimigo(inimigo* inimigo, char passos, unsigned char tam_lado, unsigned short max_x, unsigned short max_y) {
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
            inimigo->y = (int)(max_y / 2 + (sin(inimigo->x * 0.05) * 100)); // Produz um movimento ondulatorio
            break;
        case 4:
            inimigo->y += passos * PASSOS_INIMIGO; // Move para baixo
            break;
        case 5: 
            // Verifica se o inimigo atingiu o limite superior
            if (inimigo->y <= 0 + tam_lado / 2)
                fator = 1;

            // Verifica se o inimigo atingiu o limite inferior
            if (inimigo->y >= max_y - tam_lado / 2)
                fator = -1;

            inimigo->y += passos * PASSOS_INIMIGO * fator;  // Move para cima

            break;

        default:
            break;
    
    }

    // Verifica se o inimigo saiu da tela 
    
    if (inimigo->x - tam_lado / 2 < 0) {
        inimigo->x = max_x + tam_lado / 2;
        inimigo->y = aleat(tam_lado / 2, max_y - tam_lado / 2);
    }

}

void ataque_inimigo(inimigo *inimigo);
void especial_inimigo(inimigo *inimigo);

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);
}