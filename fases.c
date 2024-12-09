#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "fases.h"
#include "inimigos.h"
#include "jogador.h"

// Variáveis estáticas para os fundos parallax
static ALLEGRO_BITMAP* bg1 = NULL;
static ALLEGRO_BITMAP* bg2 = NULL;
static ALLEGRO_BITMAP* bg3 = NULL;

// Posições dos fundos
static float bg1_x = 0;
static float bg2_x = 0;
static float bg3_x = 0;

// Velocidades das camadas de parallax
static float bg1_speed = 2;  // camada mais lenta (fundo mais distante)
static float bg2_speed = 4;  // camada intermediária
static float bg3_speed = 8;  // camada mais rápida (mais próxima)

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
    if (inimigo->tipo == 5 || inimigo->tipo == 6){
        if (verifica_colisao(player, inimigo))
            return 0;
    }

    if (verifica_colisao(player, inimigo)) {
        if (inimigo->dano >= player->hp)
            player->hp = 0;
        else
            player->hp -= inimigo->dano;

        inimigo->hp = 0; // Inimigo é destruído
        printf("Colisão detectada entre jogador e inimigo. Player HP: %u\n", player->hp); // DEBUG
        return 1;
    }
    return 2;
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
        case 6:
            return infos_inimigos->boss2;
        default:
            return NULL;
    }
}

unsigned char fase1(ALLEGRO_TIMER *timer, jogador *player, inimigo *inimigos[], infos_inimigos *infos_inimigos, unsigned short max_x, unsigned short max_y) {
    static unsigned int frame_count = 0;
    static inimigo *boss1 = NULL;
    frame_count++;

    // Carregamento dos fundos, caso não estejam carregados
    if (bg1 == NULL) {
        bg1 = al_load_bitmap("sprites/Tela 2.png");
        bg2 = al_load_bitmap("sprites/Tela 1.png");
        bg3 = al_load_bitmap("sprites/Tela 3.png");
        // Verificar se foram carregados corretamente
        if (!bg1 || !bg2 || !bg3) {
            fprintf(stderr, "Erro ao carregar imagens de fundo.\n");
            return 0;
        }
    }

    // Atualiza posições dos fundos para efeito parallax
    bg1_x -= bg1_speed;
    bg2_x -= bg2_speed;
    bg3_x -= bg3_speed;

    // Larguras dos fundos
    float bg1_width = al_get_bitmap_width(bg1);
    float bg2_width = al_get_bitmap_width(bg2);
    float bg3_width = al_get_bitmap_width(bg3);

    // Loop horizontal do background
    if (bg1_x <= -bg1_width) bg1_x = 0;
    if (bg2_x <= -bg2_width) bg2_x = 0;
    if (bg3_x <= -bg3_width) bg3_x = 0;

    // Criação de inimigos simples a cada 10s 
    if (frame_count % (2*FPS) == 0 && frame_count < (5*FPS)*2) {
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
                        dano = 1;
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
                        continue;
                }

                unsigned short x = max_x + largura / 2;
                unsigned short y = aleat(altura / 2, max_y - altura / 2);
                inimigos[index] = cria_inimigo(tipo, hp, largura, altura, dano, false, x, y, max_x, max_y);
            }
        }
    }

    // Criação do Boss1 após 610 frames (~10.16s)
    if (frame_count == 610){
        if (boss1 == NULL){
            unsigned char l_boss = infos_inimigos->lb1;
            unsigned char a_boss = infos_inimigos->ab1;
            unsigned short x = max_x - l_boss/2;
            unsigned short y = max_y + a_boss/2;
            boss1 = cria_inimigo(5, 1, l_boss, a_boss, 0, false, x, y, max_x, max_y);
        }
    }

    // Lógica do ataque especial do Boss1
    if (boss1 != NULL) {
        // Calcula tempo de jogo em segundos
        unsigned int tempo_decorrido = frame_count / FPS;

        // A cada 20 segundos: nos primeiros 5s do ciclo, ataque especial
        if ((tempo_decorrido % 20) < 5) {
            boss1->ativa_especial = true;
        } else {
            boss1->ativa_especial = false;
        }

        // Movimentação e colisão do boss
        mov_inimigo(boss1, 1, boss1->largura, boss1->altura, max_x, max_y);
        unsigned char c_boss = verifica_colisao_players(player, boss1, max_x);
        if (!c_boss) {
            destroi_inimigo(boss1);
            boss1 = NULL;
        }

        unsigned char c_projetil_boss = verifica_colisao_projeteis(player, boss1, max_x);

        // Desenhar boss depois dos fundos e do jogador
        ALLEGRO_BITMAP* sprite_boss = get_sprite(5, infos_inimigos);
        if (boss1 != NULL) {
        desenha_inimigo(sprite_boss, boss1, boss1->largura, boss1->altura);        

        // Se o boss for destruído após a animação
        if (boss1->hp <= 0 && boss1->frame_atual == 5 && boss1->tempo_anim >= TEMPO_POR_FRAME) {
            destroi_inimigo(boss1);
            boss1 = NULL;
            printf("Boss1 foi destruído após animação completa.\n");
        }

        desenha_projeteis_inimigo(boss1, max_x, max_y, infos_inimigos);
    }
    }

    // Limpamos a tela antes de desenhar
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Desenha fundos (parallax)
    // bg1 (camada mais distante)
    al_draw_bitmap(bg1, bg1_x, 0, 0);
    al_draw_bitmap(bg1, bg1_x + bg1_width, 0, 0);

    // bg2 (camada intermediária)
    al_draw_bitmap(bg2, bg2_x, 0, 0);
    al_draw_bitmap(bg2, bg2_x + bg2_width, 0, 0);

    // bg3 (camada mais próxima)
    al_draw_bitmap(bg3, bg3_x, 0, 0);
    al_draw_bitmap(bg3, bg3_x + bg3_width, 0, 0);

    // Atualizar e desenhar o jogador
    mov_jogador(player, 1, max_x, max_y);
    desenha_jogador(player);
    desenha_projeteis_jog(player, max_x, max_y);

    // Atualizar e desenhar inimigos comuns
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

            if (inimigos[i] != NULL && inimigos[i]->hp > 0) {
                ALLEGRO_BITMAP* sprite = get_sprite(inimigos[i]->tipo, infos_inimigos);
                if (sprite) {
                    desenha_inimigo(sprite, inimigos[i], inimigos[i]->largura, inimigos[i]->altura);
                }
                desenha_projeteis_inimigo(inimigos[i], max_x, max_y, infos_inimigos);
            }

            if (inimigos[i] != NULL && inimigos[i]->x + inimigos[i]->largura / 2 < 0) {
                destroi_inimigo(inimigos[i]);
                inimigos[i] = NULL;
            }
        }
    }

    // Desenhar o boss (se existir) após jogador e inimigos, já que pode sobrepor
    if (boss1 != NULL) {
        ALLEGRO_BITMAP* sprite_boss = get_sprite(5, infos_inimigos);
        if (sprite_boss != NULL) {
            desenha_inimigo(sprite_boss, boss1, boss1->largura, boss1->altura);

            // Quando a animação do boss em morte acabar
            if (boss1->hp <= 0 && boss1->frame_atual == 5 && boss1->tempo_anim >= TEMPO_POR_FRAME) {
                destroi_inimigo(boss1);
                boss1 = NULL;
            }
        }
    }

    al_flip_display();
    return 1;
}

unsigned char fase2(ALLEGRO_TIMER* timer, jogador* player, unsigned short max_x, unsigned short max_y) {
    // Exemplo vazio para fase2, caso necessário
    return 1;
}

void atualiza_fase(ALLEGRO_TIMER* timer, unsigned char* fase_atual) {
    // Atualizar a fase atual conforme a lógica do jogo
}
