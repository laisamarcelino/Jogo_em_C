#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "fases.h"
#include "inimigos.h"

// Função para verificar colisão entre retângulos
unsigned char verifica_colisao(jogador *player, inimigo *inimigo) {
    // Calcula os limites dos retângulos
    int player_left = player->x - player->largura / 2;
    int player_right = player->x + player->largura / 2;
    int player_top = player->y - player->altura / 2;
    int player_bottom = player->y + player->altura / 2;

    int inimigo_left = inimigo->x - inimigo->largura / 2;
    int inimigo_right = inimigo->x + inimigo->largura / 2;
    int inimigo_top = inimigo->y - inimigo->altura / 2;
    int inimigo_bottom = inimigo->y + inimigo->altura / 2;

    // Verifica sobreposição
    if (player_right < inimigo_left || player_left > inimigo_right ||
        player_bottom < inimigo_top || player_top > inimigo_bottom) {
        return 0; // Sem colisão
    }
    return 1; // Colisão detectada
}

// Função para verificar colisão de projéteis
unsigned char colisao_projeteis(nodo_bala *projetil, jogador *player, inimigo *inimigo) {
    if (inimigo != NULL) {
        // Colisão com inimigo
        int proj_x = projetil->x;
        int proj_y = projetil->y;

        int inimigo_left = inimigo->x - inimigo->largura / 2;
        int inimigo_right = inimigo->x + inimigo->largura / 2;
        int inimigo_top = inimigo->y - inimigo->altura / 2;
        int inimigo_bottom = inimigo->y + inimigo->altura / 2;

        if (proj_x >= inimigo_left && proj_x <= inimigo_right &&
            proj_y >= inimigo_top && proj_y <= inimigo_bottom) {
            return 1; // Colidiu com inimigo
        }
    } else if (player != NULL) {
        // Colisão com jogador
        int proj_x = projetil->x;
        int proj_y = projetil->y;

        int player_left = player->x - player->largura / 2;
        int player_right = player->x + player->largura / 2;
        int player_top = player->y - player->altura / 2;
        int player_bottom = player->y + player->altura / 2;

        if (proj_x >= player_left && proj_x <= player_right &&
            proj_y >= player_top && proj_y <= player_bottom) {
            return 2; // Colidiu com jogador
        }
    }
    return 0; // Sem colisão
}

// Função para verificar colisão entre jogador e inimigo
unsigned char verifica_colisao_players(jogador *player, inimigo *inimigo, unsigned short max_x){
    if (verifica_colisao(player, inimigo)) {
        if (inimigo->dano >= player->hp) {
            player->hp = 0;
        } else {
            player->hp -= inimigo->dano;
        }
        inimigo->hp = 0; // Inimigo é destruído
        printf("Colisão detectada entre jogador e inimigo. Player HP: %u\n", player->hp);
        return 1;
    }
    return 0;
}

// Função para verificar colisão de projéteis
unsigned char verifica_colisao_projeteis(jogador *player, inimigo *inimigo, unsigned short max_x) {
    nodo_bala *atual_proj = player->projeteis->inicio;
    nodo_bala *anterior_proj = NULL;

    // Verifica projéteis do jogador atingindo o inimigo
    while (atual_proj) {
        if (colisao_projeteis(atual_proj, NULL, inimigo)) {
            if (inimigo->hp <= atual_proj->dano) {
                inimigo->hp = 0;
            } else {
                inimigo->hp -= atual_proj->dano;
            }

            // Remove o projétil da lista
            if (!anterior_proj)
                player->projeteis->inicio = atual_proj->prox;
            else
                anterior_proj->prox = atual_proj->prox;

            nodo_bala *remover = atual_proj;
            atual_proj = atual_proj->prox;
            free(remover);

            printf("Projétil atingiu inimigo. Inimigo HP: %u\n", inimigo->hp);

            // Verifica se o inimigo foi destruído
            if (inimigo->hp == 0) {
                printf("Inimigo foi destruído.\n");
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
                if (atual_proj->dano >= player->hp) 
                    player->hp = 0;
                else 
                    player->hp -= atual_proj->dano;
                
                // Remove o projétil da lista
                if (!anterior_proj)
                    inimigo->projeteis->inicio = atual_proj->prox;
                else
                    anterior_proj->prox = atual_proj->prox;

                nodo_bala *remover = atual_proj;
                atual_proj = atual_proj->prox;
                free(remover);

                printf("Projétil inimigo atingiu jogador. Player HP: %u\n", player->hp);
                return 2; // O jogador foi atingido
            } 
            else {
                anterior_proj = atual_proj;
                atual_proj = atual_proj->prox;
            }
        }
    }

    return 0;
}

// Função para obter o sprite do inimigo
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
        case 5:
            return infos_inimigos->boss1;
        default:
            return NULL;
    }
}

void fase1(ALLEGRO_TIMER *timer, jogador *player, inimigo *inimigos[], infos_inimigos *infos_inimigos, unsigned short max_x, unsigned short max_y) {
    static unsigned int frame_count = 0;
    frame_count++;
    static inimigo *boss1 = NULL;

    printf("frame count %u\n", frame_count);

    // Cria inimigos a cada 300 frames (10 segundos)
    if (frame_count % 300 == 0 && frame_count < 600) {
        for (int i = 0; i < 4; i++) {
            int index = -1;
            for (int j = 0; j < MAX_INIMIGOS; j++) {
                if (inimigos[j] == NULL) {
                    index = j;
                    break;
                }
            }
            if (index != -1) {
                unsigned char tipo = aleat(1, 2);
                unsigned char hp, dano;
                unsigned char largura, altura;

                switch (tipo) {
                    case 1:
                        hp = 2;
                        dano = 1; // dano ao encostar no personagem
                        largura = infos_inimigos->l1;
                        altura = infos_inimigos->a1;
                        break;
                    case 2:
                        hp = 3;
                        dano = 1;
                        largura = infos_inimigos->l2;
                        altura = infos_inimigos->a2;
                        break;
                    default:
                        continue; // Tipo inválido, pular para o próximo loop
                }

                unsigned short x = max_x + largura / 2;
                unsigned short y = aleat(altura / 2, max_y - altura / 2);
                inimigos[index] = cria_inimigo(tipo, hp, largura, altura, dano, x, y, max_x, max_y);
                printf("Inimigo criado: Tipo %u, HP %u, Dano %u, Posição (%u, %u)\n", tipo, hp, dano, x, y); // DEBUG
            }
        }
    }
    
    /* --------------- Implementação do Boss --------------*/

    if (frame_count == 610){ 
        if (boss1 == NULL){
            unsigned char l_boss = infos_inimigos->lb1;
            unsigned char a_boss = infos_inimigos->ab1;
            unsigned short x = max_x - l_boss/2;
            unsigned short y = max_y + a_boss/2;
            boss1 = cria_inimigo(5, 40, l_boss, a_boss, 5, x, y, max_x, max_y);
            printf("Boss1 criado com sucesso: %u\n", boss1->tipo); // DEBUG
        }
        
    }

    if (boss1 != NULL)
        mov_inimigo(boss1, 1, infos_inimigos->lb1, infos_inimigos->ab1, max_x, max_y);
    
// -----------------------------------------------

    // Atualizar o jogador
    mov_jogador(player, 1, max_x, max_y);

    // Limpar a tela antes de desenhar
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Desenhar o jogador
    desenha_jogador(player);

    // Desenhar projéteis do jogador
    desenha_projeteis_jog(player, max_x, max_y);

// ---------------------------- boos -------
    ALLEGRO_BITMAP* sprite_boss = get_sprite(5, infos_inimigos);
    if (boss1 != NULL && sprite_boss != NULL){
        desenha_inimigo(sprite_boss, boss1, boss1->largura, boss1->altura);
        desenha_projeteis_inimigo(boss1, max_x, max_y, infos_inimigos);
    }
// --------------------------------------------------------------------------

    // Iterar sobre todos os inimigos
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i] != NULL) {
            mov_inimigo(inimigos[i], 1, inimigos[i]->largura, inimigos[i]->altura, max_x, max_y);

            unsigned char c_players = verifica_colisao_players(player, inimigos[i], max_x);
            if (c_players == 1) {
                destroi_inimigo(inimigos[i]);
                inimigos[i] = NULL;
            }

            unsigned char c_projeteis = verifica_colisao_projeteis(player, inimigos[i], max_x);
            if (c_projeteis == 1) {
                destroi_inimigo(inimigos[i]);
                inimigos[i] = NULL;
            }

            if (inimigos[i] != NULL && inimigos[i]->tipo == 1) {
                desenha_projeteis_inimigo(inimigos[i], max_x, max_y, infos_inimigos);
            }

            if (inimigos[i] != NULL && inimigos[i]->hp > 0) {
                ALLEGRO_BITMAP* sprite = get_sprite(inimigos[i]->tipo, infos_inimigos);
                if (sprite) {
                    desenha_inimigo(sprite, inimigos[i], inimigos[i]->largura, inimigos[i]->altura);
                }
            }

            if (inimigos[i] != NULL && inimigos[i]->x + inimigos[i]->largura / 2 < 0) {
                printf("Inimigo saiu da tela e será removido.\n"); //DEBUG
                destroi_inimigo(inimigos[i]);
                inimigos[i] = NULL;
            }
        }
    }

    al_flip_display();
}

