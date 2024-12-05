#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "fases.h"
#include "inimigos.h"

unsigned char verifica_colisao_players(jogador *player, inimigo *inimigo, unsigned short max_x){

    if ((((inimigo->y - inimigo->altura / 2 >= player->y - player->altura / 2) && (player->y + player->altura / 2 >= inimigo->y - inimigo->altura / 2)) ||
        ((player->y - player->altura / 2 >= inimigo->y - inimigo->altura / 2) && (inimigo->y + inimigo->altura / 2 >= player->y - player->altura / 2))) &&
        (((inimigo->x - inimigo->largura / 2 >= player->x - player->largura / 2) && (player->x + player->largura / 2 >= inimigo->x - inimigo->largura / 2)) ||
        ((player->x - player->largura / 2 >= inimigo->x - inimigo->largura / 2) && (inimigo->x + inimigo->largura / 2 >= player->x - player->largura / 2)))) {

        player->hp -= inimigo->dano; // Diminui o HP do jogador pelo dano do inimigo
        inimigo->hp = 0; // Inimigo é destruído

        return 1;
    }

    return 0;
}

unsigned char colisao_projeteis(nodo_bala *projetil, jogador *player, inimigo *inimigo) {

    if (inimigo != NULL) {
        return (
            projetil->x >= inimigo->x - inimigo->largura / 2 &&
            projetil->x <= inimigo->x + inimigo->largura / 2 &&
            projetil->y >= inimigo->y - inimigo->altura / 2 &&
            projetil->y <= inimigo->y + inimigo->altura / 2
        );
    }

    return (
        projetil->x >= player->x - player->largura / 2 &&
        projetil->x <= player->x + player->largura / 2 &&
        projetil->y >= player->y - player->altura / 2 &&
        projetil->y <= player->y + player->altura / 2
    );
}

unsigned char verifica_colisao_projeteis(jogador *player, inimigo *inimigo, unsigned short max_x) {
    nodo_bala *atual_proj = player->projeteis->inicio;
    nodo_bala *anterior_proj = NULL;

    // Verifica projéteis do jogador atingindo o inimigo
    while (atual_proj) {
        if (colisao_projeteis(atual_proj, NULL, inimigo)) {
            inimigo->hp -= atual_proj->dano; // Utiliza o dano do projétil atual

            // Remove o projétil da lista
            if (!anterior_proj)
                player->projeteis->inicio = atual_proj->prox;
            else
                anterior_proj->prox = atual_proj->prox;

            nodo_bala *remover = atual_proj;
            atual_proj = atual_proj->prox;
            free(remover);

            // Verifica se o inimigo foi destruído
            if (inimigo->hp <= 0) {
                return 1; // O inimigo foi destruído
            }
        } else {
            anterior_proj = atual_proj;
            atual_proj = atual_proj->prox;
        }
    }

    // Verifica projéteis dos inimigos atingindo o jogador
    if (inimigo) {
        atual_proj = inimigo->projeteis->inicio;
        anterior_proj = NULL;

        while (atual_proj) {
            if (colisao_projeteis(atual_proj, player, NULL)) {
                player->hp -= atual_proj->dano; // Utiliza o dano do projétil atual

                // Remove o projétil da lista
                if (!anterior_proj)
                    inimigo->projeteis->inicio = atual_proj->prox;
                else
                    anterior_proj->prox = atual_proj->prox;

                nodo_bala *remover = atual_proj;
                atual_proj = atual_proj->prox;
                free(remover);
                return 2; // O jogador foi atingido
            } else {
                anterior_proj = atual_proj;
                atual_proj = atual_proj->prox;
            }
        }
    }

    return 0;
}


ALLEGRO_BITMAP* get_sprite(unsigned char tipo, infos_inimigos* infos_inimigos){
    switch(tipo){
        case 1:
            return infos_inimigos->inimigo1;
        case 2:
            return infos_inimigos->inimigo2;
        case 3:
            return infos_inimigos->inimigo3;
        case 4:
            return infos_inimigos->inimigo4;
        default:
            return NULL;
    }
}

void fase1(ALLEGRO_TIMER *timer, jogador *player, inimigo *inimigos[], infos_inimigos *infos_inimigos, unsigned short max_x, unsigned short max_y){
    static unsigned int frame_count = 0;
    frame_count++;

    // A cada 300 frames (10 segundos)
    if (frame_count % 300 == 0){
        // Cria 3 inimigos por vez
        for (int i = 0; i < 4; i++){
            // Encontrar um slot livre no array inimigos[]
            int index = -1;
            for (int j = 0; j < MAX_INIMIGOS; j++){
                if (inimigos[j] == NULL){
                    index = j;
                    break;
                }
            }
            if (index != -1){
                unsigned char tipo = aleat(1, 2); 
                unsigned char hp, dano;
                unsigned char largura, altura;

                switch(tipo) {
                    case 1:
                        hp = 3;
                        dano = 1;
                        largura = infos_inimigos->l1;
                        altura = infos_inimigos->a1;
                        break;
                    case 2:
                        hp = 2;
                        dano = 2;
                        largura = infos_inimigos->l2;
                        altura = infos_inimigos->a2;
                        break;
                    default:
                        continue; // Tipo inválido, pular para o próximo loop
                }

                unsigned short x = max_x + largura / 2;
                unsigned short y = aleat(altura / 2, max_y - altura / 2);
                inimigos[index] = cria_inimigo(tipo, hp, largura, altura, dano, x, y, max_x, max_y);
            }
        }
    }

    mov_jogador(player, 1, max_x, max_y);
    al_clear_to_color(al_map_rgb(0, 0, 0)); 
    desenha_projeteis_jog(player, max_x, max_y);
    desenha_jogador(player);

    for (int i = 0; i < MAX_INIMIGOS; i++){
        if (inimigos[i] != NULL){
            mov_inimigo(inimigos[i], 1, inimigos[i]->largura, inimigos[i]->altura, max_x, max_y);

            unsigned char c_players = verifica_colisao_players(player, inimigos[i], max_x);

            // Verifica colisão de projéteis
            unsigned char c_projeteis = verifica_colisao_projeteis(player, inimigos[i], max_x);
            if (inimigos[i]->tipo == 1)
                desenha_projeteis_inimigo(inimigos[i], max_x, max_y, infos_inimigos);

            if (inimigos[i]->hp > 0){
                ALLEGRO_BITMAP *sprite = get_sprite(inimigos[i]->tipo, infos_inimigos);
                if (sprite){
                    desenha_inimigo(sprite, inimigos[i], inimigos[i]->largura, inimigos[i]->altura);
                }
            } 

            if (inimigos[i] != NULL && inimigos[i]->x + inimigos[i]->largura / 2 < 0){
                destroi_inimigo(inimigos[i]);
                inimigos[i] = NULL;
            }
        }
    }

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