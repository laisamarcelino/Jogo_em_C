#include "inimigos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

inimigo* cria_inimigo(unsigned char tipo, unsigned char hp, unsigned char largura, unsigned char altura, unsigned char dano, bool ativa_especial, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y) {
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
    novo_inimigo->tempo_disparo = 30; // Dispara a cada 30 frames
    novo_inimigo->contador_disparo = 0;
    novo_inimigo->frame_atual = 0;
    novo_inimigo->tempo_anim = 0;
    novo_inimigo->frame_atual_proj = 0;
    novo_inimigo->tempo_anim_proj = 0;
    novo_inimigo->ativa_especial = ativa_especial;

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
            inimigo->x -= passos * PASSOS_INIMIGO-3;
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
}

void ataque_inimigo(inimigo *inimigo){
    unsigned char dano = 1; // Dano do projétil

    unsigned short x_tiro = inimigo->x - inimigo->largura / 2; // Inicia na frente do inimigo
    unsigned short y_tiro = inimigo->y; // Na mesma altura do inimigo
    
    switch (inimigo->tipo) {
        case 1:
            dano = 1;
            break;
        case 3:
            dano = 2;
            break;
        case 4:
            dano = 3;
            break;
        case 5: //boss 1
            if (inimigo->ativa_especial)
                dano = 10;
            else
                dano = 5;
            break;
        case 6: //boss 2
            if (inimigo->ativa_especial)
                dano = 15;
            else
                dano = 10;
            break;
        
        default:
            dano = 1;
            break;
    }

    insere_bala(inimigo->projeteis, x_tiro, y_tiro, dano);
}

void destroi_inimigo(inimigo *inimigo){
    destroi_projetil_lista(&(inimigo->projeteis));
    free(inimigo);  
}

void desenha_inimigo(ALLEGRO_BITMAP *sprite, inimigo *inimigo, unsigned short largura, unsigned short altura) {
    if (!sprite) {
        fprintf(stderr, "Sprite é NULL em desenha_inimigo.\n");
        return;
    }

    inimigo->tempo_anim++;

    if (inimigo->tipo == 5) { // Lógica específica para Boss 1
        if (inimigo->hp > 0) {
            // Animação normal (primeira linha com 2 frames)
            if (inimigo->tempo_anim >= TEMPO_POR_FRAME) {
                inimigo->frame_atual = (inimigo->frame_atual + 1) % 2;
                inimigo->tempo_anim = 0;
            }
        } else {
            // Animação de destruição (segunda linha com 6 frames)
            if (inimigo->tempo_anim >= TEMPO_POR_FRAME) {
                if (inimigo->frame_atual < 5) {
                    // Avança até o último frame (0 a 5 total = 6 frames)
                    inimigo->frame_atual++;
                    inimigo->tempo_anim = 0;
                }
            }
        }
    }

    // Cálculo para selecionar o frame correto
    unsigned short frame_x = inimigo->frame_atual * largura;
    unsigned short frame_y = (inimigo->hp > 0) ? 0 : altura;

    al_draw_bitmap_region(
        sprite,
        frame_x, frame_y,
        largura, altura,
        inimigo->x - largura / 2, inimigo->y - altura / 2,
        0
    );
}


infos_inimigos* carrega_sprites() {
    infos_inimigos *novo_infos = (infos_inimigos*)malloc(sizeof(infos_inimigos));
    if (!novo_infos) {
        fprintf(stderr, "Erro ao alocar memória para infos_inimigos.\n");
        return NULL;
    }

    // Carregar os sprites
    novo_infos->inimigo1 = al_load_bitmap("./sprites/inimigo1.png");
    novo_infos->inimigo2 = al_load_bitmap("./sprites/inimigo2.png");
    novo_infos->inimigo3 = al_load_bitmap("./sprites/inimigo3.png");
    novo_infos->inimigo4 = al_load_bitmap("./sprites/inimigo4.png");
    novo_infos->boss1 = al_load_bitmap("./sprites/boss1.png");
    novo_infos->boss2 = al_load_bitmap("./sprites/boss2.png");
    novo_infos->projetil2 = al_load_bitmap("./sprites/projetil1.png");
    novo_infos->projetil_boss1 = al_load_bitmap("./sprites/projetil_boss1.png");
    novo_infos->projetil_boss1_especial = al_load_bitmap("./sprites/projetil_boss1_especial.png");
    

    // Verificação de carregamento
    if (!novo_infos->inimigo1 || !novo_infos->inimigo2 || !novo_infos->inimigo3 || 
        !novo_infos->inimigo4 || !novo_infos->boss1 || !novo_infos->boss2 || 
        !novo_infos->projetil2 || !novo_infos->projetil_boss1 || !novo_infos->projetil_boss1_especial) {
        fprintf(stderr, "Erro ao carregar sprites dos inimigos.\n");
        // Liberar os bitmaps que foram carregados com sucesso
        if (novo_infos->inimigo1) al_destroy_bitmap(novo_infos->inimigo1);
        if (novo_infos->inimigo2) al_destroy_bitmap(novo_infos->inimigo2);
        if (novo_infos->inimigo3) al_destroy_bitmap(novo_infos->inimigo3);
        if (novo_infos->inimigo4) al_destroy_bitmap(novo_infos->inimigo4);
        if (novo_infos->boss1) al_destroy_bitmap(novo_infos->boss1);
        if (novo_infos->boss2) al_destroy_bitmap(novo_infos->boss2);
        if (novo_infos->projetil2) al_destroy_bitmap(novo_infos->projetil2);
        if (novo_infos->projetil_boss1) al_destroy_bitmap(novo_infos->projetil_boss1);
        if (novo_infos->projetil_boss1_especial) al_destroy_bitmap(novo_infos->projetil_boss1_especial);
        free(novo_infos);
        return NULL;
    }

    // Calcula a largura e altura dos inimigos
    novo_infos->l1 = al_get_bitmap_width(novo_infos->inimigo1) / 3;
    novo_infos->a1 = al_get_bitmap_height(novo_infos->inimigo1);
    novo_infos->l2 = al_get_bitmap_width(novo_infos->inimigo2) / 5;
    novo_infos->a2 = al_get_bitmap_height(novo_infos->inimigo2);
    novo_infos->l3 = al_get_bitmap_width(novo_infos->inimigo3) / 6;
    novo_infos->a3 = al_get_bitmap_height(novo_infos->inimigo3);
    novo_infos->l4 = al_get_bitmap_width(novo_infos->inimigo4) / 3;
    novo_infos->a4 = al_get_bitmap_height(novo_infos->inimigo4);
    novo_infos->lb1 = al_get_bitmap_width(novo_infos->boss1) / 6;
    novo_infos->ab1 = al_get_bitmap_height(novo_infos->boss1) / 2;

    // Calcula a largura e altura dos projeteis
    novo_infos->lp2 = al_get_bitmap_width(novo_infos->projetil2) / 8;
    novo_infos->ap2 = al_get_bitmap_height(novo_infos->projetil2); 
    novo_infos->lpb1 = al_get_bitmap_width(novo_infos->projetil_boss1) / 4;
    novo_infos->apb1 = al_get_bitmap_height(novo_infos->projetil_boss1); 
    novo_infos->lpbe1 = al_get_bitmap_width(novo_infos->projetil_boss1_especial) / 5;
    novo_infos->apbe1 = al_get_bitmap_height(novo_infos->projetil_boss1_especial); 

    return novo_infos;
}

void desenha_projeteis_inimigo(inimigo *inimigo, unsigned short max_x, unsigned short max_y, infos_inimigos *infos_inimigos) {
    if (!inimigo || !inimigo->projeteis) {
        fprintf(stderr, "Inimigo ou lista de projeteis é NULL.\n");
        return;
    }

    // Atualizar animação dos projeteis com base no tipo do inimigo
    inimigo->tempo_anim_proj++;
    if (inimigo->tempo_anim_proj >= TEMPO_POR_FRAME) {
        if (inimigo->tipo == 1) { 
            inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % 8;
        }
        else if (inimigo->tipo == 3) { 
            inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % 4;
        }
        else if (inimigo->tipo == 4) { 
            inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % 4;
        }
        else if (inimigo->tipo == 5) { // boss1 com 4 frames
            if(inimigo->ativa_especial)
                inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % 5;
            else
                inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % 4;
        }
        else if (inimigo->tipo == 6) { // boss2 com 4 frames
            inimigo->frame_atual_proj = (inimigo->frame_atual_proj + 1) % 4;
        }
        inimigo->tempo_anim_proj = 0;
    }

    // Controle de disparo dos projeteis
    if (inimigo->hp > 0){
        inimigo->contador_disparo++;
        if (inimigo->contador_disparo >= inimigo->tempo_disparo) {
            ataque_inimigo(inimigo);
            inimigo->contador_disparo = 0;
        }
    }

    // Atualizar a posição dos projeteis
    atualiza_projetil(inimigo->projeteis, -1, max_x, max_y);

    nodo_bala *atual = inimigo->projeteis->inicio;
    while (atual) {
        ALLEGRO_BITMAP *sprite_projetil = NULL;
        unsigned short frame_width = 0;
        unsigned short frame_height = 0;

        switch (inimigo->tipo) {
            case 1: 
                sprite_projetil = infos_inimigos->projetil2;
                frame_width = infos_inimigos->lp2;
                frame_height = infos_inimigos->ap2;
                break;
            case 3: 
                sprite_projetil = infos_inimigos->projetil3;
                frame_width = infos_inimigos->lp3;
                frame_height = infos_inimigos->ap3;
                break;
            case 4: 
                sprite_projetil = infos_inimigos->projetil4;
                frame_width = infos_inimigos->lp4;
                frame_height = infos_inimigos->ap4;
                break;
            case 5: // boss1
                if(inimigo->ativa_especial){
                    sprite_projetil = infos_inimigos->projetil_boss1_especial;
                    frame_width = infos_inimigos->lpbe1;
                    frame_height = infos_inimigos->apbe1; 
                }
                else {
                    sprite_projetil = infos_inimigos->projetil_boss1;
                    frame_width = infos_inimigos->lpb1;
                    frame_height = infos_inimigos->apb1;    
                }
                
                break;
            case 6: // boss2
                sprite_projetil = infos_inimigos->projetil_boss2;
                frame_width = infos_inimigos->lpb2;
                frame_height = infos_inimigos->apb2;
                break;
            default:
                break;
        }

        if (sprite_projetil) {
            al_draw_bitmap_region(
                sprite_projetil,
                inimigo->frame_atual_proj * frame_width, 0, 
                frame_width, frame_height,
                atual->x - frame_width / 2, atual->y - frame_height / 2, 0
            );
        }

        atual = atual->prox;
    }
}