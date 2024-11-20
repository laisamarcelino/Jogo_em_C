// gcc main.c jogador.c auxiliares.c joystick.c inimigos.c projeteis.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags) -lm

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> // (!) Figuras basicas - trocar 

#include "jogador.h"
#include "inimigos.h"
#include "auxiliares.h"
#include "joystick.h"
#include "projeteis.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h> 

#define X_TELA 800
#define Y_TELA 700

unsigned char colisao(jogador *jogador, inimigo *inimigo){

	if ((((inimigo->y-inimigo->altura/2 >= jogador->y-jogador->altura/2) && (jogador->y+jogador->altura/2 >= inimigo->y-inimigo->altura/2)) ||
		((jogador->y-jogador->altura/2 >= inimigo->y-inimigo->altura/2) && (inimigo->y+inimigo->altura/2 >= jogador->y-jogador->altura/2))) && 
		(((inimigo->x-inimigo->largura/2 >= jogador->x-jogador->largura/2) && (jogador->x+jogador->largura/2 >= inimigo->x-inimigo->largura/2)) ||
		((jogador->x-jogador->largura/2 >= inimigo->x-inimigo->largura/2) && (inimigo->x+inimigo->largura/2 >= jogador->x-jogador->largura/2)))) return 1;
	else return 0;
}

int main(){ 
    al_init(); // Inicia biblioteca Allegro
    al_init_primitives_addon(); // Inicializa imagens basicas
    al_install_keyboard(); // Habilita entrada via teclado
    srand(time(NULL));

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); // Cria o relogio do jogo
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Cria a fila de eventos
	ALLEGRO_FONT* font = al_create_builtin_font(); // Carrega uma fonte padrao para escrever na tela 
	ALLEGRO_DISPLAY* disp = al_create_display(X_TELA, Y_TELA); // Cria uma janela para o programa

	al_register_event_source(queue, al_get_keyboard_event_source()); // Insere  eventos de teclado na fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp)); // Insere eventos de tela na fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer)); // Insere eventos de relogio na fila de eventos

    ALLEGRO_EVENT event; // Guarda o evento capturado
    al_start_timer(timer); // Inicializa o relogio do programa

    jogador* player = cria_jogador(5, 50, 50, 50, Y_TELA/2, X_TELA, Y_TELA);
    if (!player){
        fprintf(stderr, "Erro ao criar jogador");
        return 1;
    }
    unsigned char chave_joystick[6] = {0,0,0,0,0,0}; 

    inimigo* inimigo1 = cria_inimigo(1, 2, 50, 50, -1, X_TELA-10, Y_TELA/3, X_TELA, Y_TELA);
    if (!inimigo1){
        fprintf(stderr, "Erro ao criar inimigo 1");
        return 1;
    }
    inimigo* inimigo2 = cria_inimigo(2, 2, 50, 50, -1, X_TELA-10, Y_TELA/2, X_TELA, Y_TELA);
    if (!inimigo2){
        fprintf(stderr, "Erro ao criar inimigo 2");
        return 1;
    }
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
    
    while (1) {
        al_wait_for_event(queue, &event); // Captura eventos da fila
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            // Atualiza projéteis e verifica posições
            atualiza_projetil(player->projeteis, X_TELA, Y_TELA);

            // Atualiza posição do jogador
            mov_jogador(player, 1, X_TELA, Y_TELA);

            // Movimenta inimigos
            mov_inimigo(inimigo1, 1, 50, 50, X_TELA, Y_TELA);
            //mov_inimigo(inimigo2, 1, 50, 50, X_TELA, Y_TELA);
            //mov_inimigo(inimigo3, 1, 50, 50, X_TELA, Y_TELA);
            //mov_inimigo(inimigo4, 1, 50, 50, X_TELA, Y_TELA);
            
            // Movimenta boss
            //mov_inimigo(boss1, 1, 50, 50, X_TELA, Y_TELA);
            //mov_inimigo(boss2, 1, 50, 50, X_TELA, Y_TELA);

            if (colisao(player, inimigo1)){
                printf("colidiu\n");
            }

            // Limpa a tela
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenha projéteis
            nodo_bala *atual = player->projeteis->inicio;
            while (atual) {
                al_draw_filled_circle(atual->x, atual->y, 5, al_map_rgb(255, 255, 0)); // Projétil amarelo
                atual = atual->prox;
            }

            // Desenha o jogador
            al_draw_filled_rectangle(
                player->x - player->largura / 2, player->y - player->altura / 2, player->x + player->largura / 2, player->y + player->altura / 2, al_map_rgb(255, 0, 0));
            
            // Desenha inimigos
            al_draw_filled_rectangle(inimigo1->x-inimigo1->largura/2, inimigo1->y-inimigo1->altura/2, inimigo1->x+inimigo1->largura/2, inimigo1->y+inimigo1->altura/2, al_map_rgb(0, 0, 255));
            //al_draw_filled_rectangle(inimigo2->x-inimigo2->largura/2, inimigo2->y-inimigo2->altura/2, inimigo2->x+inimigo2->largura/2, inimigo2->y+inimigo2->altura/2, al_map_rgb(255, 255, 255));
            //al_draw_filled_rectangle(inimigo3->x-inimigo3->largura/2, inimigo3->y-inimigo3->altura/2, inimigo3->x+inimigo3->largura/2, inimigo3->y+inimigo3->altura/2, al_map_rgb(230, 200, 250));
            //al_draw_filled_rectangle(inimigo4->x-inimigo4->largura/2, inimigo4->y-inimigo4->altura/2, inimigo4->x+inimigo4->altura/2, inimigo4->y+inimigo4->altura/2, al_map_rgb(255, 255, 255));

            // Desenha boss
            //al_draw_filled_rectangle(boss1->x-boss1->largura/2, boss1->y-boss1->altura/2, boss1->x+boss1->largura/2, boss1->y+boss1->altura/2, al_map_rgb(217, 100, 100));


            // Atualiza a tela
            al_flip_display();
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
    
    destroi_inimigo(inimigo1);
    destroi_inimigo(inimigo2);
    destroi_inimigo(inimigo3);
    destroi_inimigo(inimigo4);
    destroi_jogador(player);
    al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

    return 0;
}