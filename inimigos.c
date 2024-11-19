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
    unsigned char mov = 1;

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
            inimigo->x -= passos * PASSOS_INIMIGO;

            break;
        case 4:
            inimigo->x -= passos * PASSOS_INIMIGO;

            break;
        case 5:
            if (inimigo->y <= 0 + tam_lado / 2) {
                inimigo->y += passos * PASSOS_INIMIGO;  // Move para baixo
            }
            else if (inimigo->y >= max_y - tam_lado / 2) {
                inimigo->y -= passos * PASSOS_INIMIGO;  // Move para cima
            }
            else {
                if (mov == 1) {
                    inimigo->y -= passos * PASSOS_INIMIGO; // Move para cima
                    if (inimigo->y <= 0 + tam_lado / 2)
                        mov = 0; // Altera direção
                } else {
                    inimigo->y += passos * PASSOS_INIMIGO; // Move para baixo
                    if (inimigo->y >= max_y - tam_lado / 2)
                        mov = 1; // Altera direção
                }
            }
            break;
        default:
            break;
    
    }
        

    // Verifica se o inimigo saiu da tela
    if (inimigo->x + tam_lado / 2 < 0) {
        printf("Inimigo fora da tela. Reaparecendo...\n"); // DEBUG
        inimigo->x = max_x + tam_lado / 2;
        inimigo->y = aleat(tam_lado / 2, max_y - tam_lado / 2);
        printf("Nova posição do inimigo: x = %d, y = %d\n", inimigo->x, inimigo->y); // DEBUG
    }

}

void ataque_inimigo(inimigo *inimigo);
void especial_inimigo(inimigo *inimigo);

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);
}