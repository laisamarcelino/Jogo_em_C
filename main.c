// gcc main.c jogador.c auxiliares.c joystick.c inimigos.c projeteis.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags) -lm

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>


#include "jogador.h"
#include "inimigos.h"
#include "auxiliares.h"
#include "joystick.h"
#include "projeteis.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
#include <stdbool.h>

#define X_TELA 1920
#define Y_TELA 1080
#define FRAMES_JOGADOR 5
#define TEMPO_POR_FRAME 5

unsigned char verifica_colisao_players(jogador *jogador, inimigo *inimigo){

	if ((((inimigo->y-inimigo->altura/2 >= jogador->y-jogador->altura/2) && (jogador->y+jogador->altura/2 >= inimigo->y-inimigo->altura/2)) ||
		((jogador->y-jogador->altura/2 >= inimigo->y-inimigo->altura/2) && (inimigo->y+inimigo->altura/2 >= jogador->y-jogador->altura/2))) && 
		(((inimigo->x-inimigo->largura/2 >= jogador->x-jogador->largura/2) && (jogador->x+jogador->largura/2 >= inimigo->x-inimigo->largura/2)) ||
		((jogador->x-jogador->largura/2 >= inimigo->x-inimigo->largura/2) && (inimigo->x+inimigo->largura/2 >= jogador->x-jogador->largura/2)))){
            jogador->hp--;
            inimigo->hp = 0;
            inimigo->x = X_TELA + inimigo->largura / 2; // Remove inimigo da tela

            return 1;
        } 
       
    return 0;
}

unsigned char colisao_projeteis(nodo_bala *projetil, jogador *player, inimigo *inimigo) {
    
    if (inimigo != NULL){
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
unsigned char verifica_colisao_projeteis(jogador *player, inimigo *inimigo) {
    nodo_bala *atual_proj;

    // Verifica projéteis do jogador atingindo o inimigo
    atual_proj = player->projeteis->inicio;
    nodo_bala *anterior_proj = NULL;

    while (atual_proj) {
        if (colisao_projeteis(atual_proj, NULL, inimigo)) {
            inimigo->hp--;
            if (inimigo->hp <= 0) 
                inimigo->x = X_TELA + inimigo->largura / 2; // Remove o inimigo da tela
            
            // Remove o projétil da lista
            if (!anterior_proj) 
                player->projeteis->inicio = atual_proj->prox;
            else 
                anterior_proj->prox = atual_proj->prox;
            
            nodo_bala *remover = atual_proj;
            atual_proj = atual_proj->prox;
            free(remover);
            return 1; // O inimigo foi atingido
        } 
        else {
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
                player->hp--;
                // Remove o projétil da lista
                if (!anterior_proj) 
                    inimigo->projeteis->inicio = atual_proj->prox;
                else 
                    anterior_proj->prox = atual_proj->prox;
                
                nodo_bala *remover = atual_proj;
                atual_proj = atual_proj->prox;
                free(remover);
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

int main(){ 
    al_init(); // Inicia biblioteca Allegro
    al_init_primitives_addon(); // Inicializa imagens basicas
    al_install_keyboard(); // Habilita entrada via teclado
    srand(time(NULL));
    al_set_new_display_flags(ALLEGRO_OPENGL);
    al_init_image_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); // Cria o relogio do jogo
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Cria a fila de eventos
	ALLEGRO_FONT* font = al_create_builtin_font(); // Carrega uma fonte padrao para escrever na tela 
	ALLEGRO_DISPLAY* disp = al_create_display(X_TELA, Y_TELA); // Cria uma janela para o programa

	al_register_event_source(queue, al_get_keyboard_event_source()); // Insere  eventos de teclado na fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp)); // Insere eventos de tela na fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer)); // Insere eventos de relogio na fila de eventos

    ALLEGRO_EVENT event; // Guarda o evento capturado
    al_start_timer(timer); // Inicializa o relogio do programa

    /* -------------------------- Variaveis de controle  ------------------------ */

    unsigned char fase = 1;
    unsigned char morte_inimigo1, morte_inimigo2, morte_boss = 0;
    unsigned char chave_joystick[6] = {0,0,0,0,0,0};
    gerencia_inimigos gerenciador = {.quantidade = 0}; // Inicializa o gerenciador de inimigos
    unsigned short contador_gera_inimigos = 0;

    /* --------------------Criacao dos personagens -------------------------------------------- */

    jogador* player = cria_jogador(10, 50, 50, 50, Y_TELA/2, X_TELA, Y_TELA);
    if (!player){
        fprintf(stderr, "Erro ao criar jogador");
        return 1;
    }

    adiciona_inimigo(&gerenciador, 1, 2, 50, 50, -1, X_TELA, aleat(50, Y_TELA-50), X_TELA, Y_TELA);

    inimigo* inimigo3 = cria_inimigo(3, 2, 50, 50, -1, X_TELA-10, Y_TELA-10, X_TELA, Y_TELA);
    if (!inimigo3){
        fprintf(stderr, "Erro ao criar inimigo 3");
        return 1;
    }

    inimigo* inimigo4 = cria_inimigo(4, 2, 50, 50, -1, 50, 0, X_TELA, Y_TELA);
    if (!inimigo4){
        fprintf(stderr, "Erro ao criar inimigo 4");
        return 1;
    }

    inimigo* boss1 = cria_inimigo(5, 5, 50, 50, -1, X_TELA-10, Y_TELA/2, X_TELA, Y_TELA);
    if (!boss1){
        fprintf(stderr, "Erro ao criar boos 1");
        return 1;
    }

    inimigo* boss2 = cria_inimigo(5, 10, 50, 50, -1, X_TELA-10, Y_TELA/2, X_TELA, Y_TELA);
    if (!boss2){
        fprintf(stderr, "Erro ao criar boos 2");
        return 1;
    }


    /* --------------------------- Definicao de bitmaps ------------------------------*/
    
    unsigned char frame_atual, tempo_anim = 0; // Quadro atual, contador troca de quadro

    ALLEGRO_BITMAP *sprites_jogador = al_load_bitmap("./sprites/jogador.png");
    if (!sprites_jogador){
        fprintf(stderr, "Erro ao carregar sprites do jogador.\n");
        return 1;
    }
    
    // Converte a cor magenta (255, 0, 255) para transparência
    al_convert_mask_to_alpha(sprites_jogador, al_map_rgb(255, 0, 255));

    // Calcula a largura e altura de cada quadro da sprite
    unsigned short largura_quadro = al_get_bitmap_width(sprites_jogador) / FRAMES_JOGADOR;
    unsigned short altura_quadro = al_get_bitmap_height(sprites_jogador);


    /* --------------------------- Loop principal ------------------------------*/

    while (1) {
        al_wait_for_event(queue, &event); // Captura eventos da fila
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (fase == 1){
                
                /* ------------------------------------------ Movimentacoes ------------------------------------------- */
                if (player->hp > 0)
                    mov_jogador(player, 1, X_TELA, Y_TELA);
    
                if (contador_gera_inimigos >= 200) { // A cada 100 frames (~3 segundos em 30 FPS)
                    adiciona_inimigo(&gerenciador, aleat(1,2), 2, 50, 50, -1,  X_TELA-10, aleat(Y_TELA/4, Y_TELA-50), X_TELA, Y_TELA);
                    contador_gera_inimigos = 0;
                }
                contador_gera_inimigos++; 

                // Limpa a tela
                al_clear_to_color(al_map_rgb(0, 0, 0));

                for (int i = 0; i < gerenciador.quantidade; i++) {
                    inimigo *atual = gerenciador.lista[i];
                    if (atual->hp > 0) {
                        mov_inimigo(atual, 1, 50, 50, X_TELA, Y_TELA);
                        al_draw_filled_rectangle(atual->x - atual->largura / 2, atual->y - atual->altura / 2, atual->x + atual->largura / 2, atual->y + atual->altura / 2, al_map_rgb(0, 255, 0));
                    }

                    unsigned char c_players = verifica_colisao_players(player, atual);
                    unsigned char c_projeteis = verifica_colisao_projeteis(player, atual);

                    if (atual->tipo == 2){
                        atual->contador_disparo++;
                            
                        // Incrementa o contador de disparo do inimigo 2
                        if (atual->contador_disparo >= atual->tempo_disparo) {
                            ataque_inimigo(atual); // Inimigo dispara projétil
                            atual->contador_disparo = 0;
                        }
                        
                        // Desenha projéteis do inimigo 2
                        nodo_bala *atual_inimigo = atual->projeteis->inicio;
                        while (atual_inimigo) {
                            al_draw_filled_circle(atual_inimigo->x, atual_inimigo->y, 5, al_map_rgb(255, 0, 0)); // Vermelho 
                            atual_inimigo = atual_inimigo->prox;
                            
                        }
                        atualiza_projetil(atual->projeteis, -1, X_TELA, Y_TELA);
                    }
                }

                if (boss1->hp > 0)
                    mov_inimigo(boss1, 1, 50, 50, X_TELA, Y_TELA);

                unsigned char c_p3 = verifica_colisao_players(player, boss1);
                unsigned char c_p_boss = verifica_colisao_projeteis(player, boss1);

                /* ------------------------------------------ Desenhos ------------------------------------------- */

                if (player->hp > 0){

                    // Atualiza o contador de tempo para animação
                    tempo_anim++;
                    if (tempo_anim >= TEMPO_POR_FRAME) {
                        tempo_anim = 0;
                        frame_atual = (frame_atual + 1) % FRAMES_JOGADOR; // Alterna entre os quadros
                    }

                    // Calcula a posição do quadro atual na sprite sheet
                    int x_quadro = frame_atual * largura_quadro;

                    // Desenha o quadro atual na posição do jogador
                    al_draw_bitmap_region(
                        sprites_jogador,
                        x_quadro, 0,                // Região do quadro atual
                        largura_quadro, altura_quadro, // Dimensões do quadro
                        player->x - largura_quadro / 2, player->y - altura_quadro / 2, // Posição na tela
                        0 // Sem flags
                    );

                    // Atualiza projéteis do jogador e verifica posições
                    atualiza_projetil(player->projeteis, 1, X_TELA, Y_TELA);

                    // Desenha projéteis do jogador
                    nodo_bala *atual = player->projeteis->inicio;
                    while (atual) {
                        al_draw_filled_circle(atual->x, atual->y, 5, al_map_rgb(255, 255, 0)); // Amarelo
                        atual = atual->prox;
                    }
                }

                /*
                if (c_p1 || c_p2 || c_p3 || c_p_ini2 == 2 || c_p_boss == 2) {
                    // Animação de desenho piscando 
                    printf("pisquei\n");
                }                 
                */

                for (int i = 0; i < gerenciador.quantidade; i++) {
                    if (gerenciador.lista[i]->hp <= 0) {
                        free(gerenciador.lista[i]);
                        gerenciador.lista[i] = gerenciador.lista[--gerenciador.quantidade];
                        i--; // Reajuste o índice
                    }
                }

                // Atualiza a tela
                al_flip_display();

            }
        }

        else if (event.type == ALLEGRO_EVENT_KEY_DOWN){ // O botão foi pressionado
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    chave_joystick[0] = 1;
                    break;
                case ALLEGRO_KEY_S:
                    chave_joystick[1] = 1;
                    break;
                case ALLEGRO_KEY_A:
                    chave_joystick[2] = 1;
                    break;
                case ALLEGRO_KEY_D:
                    chave_joystick[3] = 1;
                    break;
                case ALLEGRO_KEY_E:
                    chave_joystick[4] = 1;
                    ataque_jogador(player);
                    break;
                case ALLEGRO_KEY_X:
                    chave_joystick[5] = 1;
                    break;
                default:
                    break;
            }
            atualiza_joystick(player->controle, chave_joystick);
        }

        else if (event.type == ALLEGRO_EVENT_KEY_UP) { // O botão foi solto
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    chave_joystick[0] = 0;
                    break;
                case ALLEGRO_KEY_S:
                    chave_joystick[1] = 0;
                    break;
                case ALLEGRO_KEY_A:
                    chave_joystick[2] = 0;
                    break;
                case ALLEGRO_KEY_D:
                    chave_joystick[3] = 0;
                    break;
                case ALLEGRO_KEY_E:
                    chave_joystick[4] = 0;
                    break;
                case ALLEGRO_KEY_X:
                    chave_joystick[5] = 0;
                    break;
                default:
                    break;
            }
            atualiza_joystick(player->controle, chave_joystick);
        }

        else if (event.type == 42 || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) // Clique no X da tela ou esc para sair do programa
            break;
        
    }
    
    destroi_jogador(player);
    al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
    al_destroy_bitmap(sprites_jogador);

    return 0;
}