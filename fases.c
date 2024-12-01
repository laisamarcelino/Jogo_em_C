#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "fases.h"
#include "inimigos.h"

inimigo* cria_inimigos (unsigned short max_x, unsigned short max_y){
    inimigo* inimigos[MAX_INIMIGOS];
    tamanho *tamanho;
    
    tamanho = carrega_sprites();

    // Cria lista de inimigos
    for (unsigned char i = 0; i < MAX_INIMIGOS; i++) {
        unsigned char tipo = aleat(1, 2); 
        unsigned char hp = (tipo == 1) ? 3 : 2;
        unsigned char dano = (tipo == 1) ? 0 : 2;
        unsigned char largura = (tipo == 1) ? tamanho->l1 : tamanho->l2;
        unsigned char altura = (tipo == 1) ? tamanho->a1 : tamanho->a2;
        unsigned short x = max_x - 10; 
        unsigned short y = aleat(altura / 2, max_y - altura / 2); 

        inimigos[i] = cria_inimigo(tipo, hp, largura, altura, dano, x, y, max_x, max_y);
        if (!inimigos[i]) {
            fprintf(stderr, "Erro ao criar inimigo %d\n", i);
            return;
        }
    }                                                      
    
    return *inimigos;
}

void fase1 (ALLEGRO_TIMER* timer, jogador* player, inimigo* inimigos[], tamanho *tamanho, unsigned short max_x, unsigned short max_y) {
    static int ultimo_tempo = 0; // Armazena o último tempo processado
    int tempo = (int)(al_get_timer_count(timer) / 30.0);
    
    if (tempo > 0 && tempo <= 10){
        mov_inimigo(inimigos[0], 1, inimigos[0]->largura, inimigos[0]->altura, max_x, max_y);
        printf("tempo 10\n");     
        desenha_inimigo(tamanho->inimigo1, inimigos[0], inimigos[0]->largura, inimigos[0]->altura);
 
    }

    mov_jogador(player, 1, max_x, max_y);

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpar tela

    desenha_projeteis_jog(player, max_x, max_y);
    desenha_jogador(player);

    al_flip_display();
}


/*
void fase2 (ALLEGRO_TIMER* timer, jogador* player, unsigned short max_x, unsigned short max_y) {
    static int ultimo_tempo = 0; // Armazena o último tempo processado
    int tempo = (int)(al_get_timer_count(timer) / 30.0);

    // Executa apenas quando o tempo atual for maior que o último tempo registrado
    if (tempo > ultimo_tempo) {
        ultimo_tempo = tempo; // Atualiza o último tempo

        switch (tempo) {
        case 10:
            printf("tempo 10\n");
            break;
        case 20:
            printf("tempo 20\n");
            break;
        case 30:
            printf("tempo 30\n");
            break;
        case 40:
            printf("tempo 40\n");
            break;
        case 50:
            printf("tempo 50\n");
            break;
        case 60:
            printf("tempo 60\n");
            break;
        case 70:
            printf("tempo 70\n");
            break;
        default:
            break;
        }
    }

    mov_jogador(player, 1, max_x, max_y);

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpar tela

    desenha_projeteis_jog(player, max_x, max_y);
    desenha_jogador(player);

    al_flip_display();
}

void atualiza_fase(ALLEGRO_TIMER* timer, unsigned char* fase_atual) {
    double tempo = al_get_timer_count(timer) / 30.0; // Tempo em segundos
    if (tempo < 60) {
        *fase_atual = 1;
    } else {
        *fase_atual = 2;
    }
}
*/