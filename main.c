// gcc main.c jogador.c auxiliares.c joystick.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> // (!) Figuras basicas - trocar 

#include "jogador.h"
#include "inimigos.h"
#include "auxiliares.h"
#include "joystick.h"

#include <stdio.h>


#define X_TELA 800
#define Y_TELA 700

int main(){ 
    al_init(); // Inicia biblioteca Allegro
    al_init_primitives_addon(); // Inicializa imagens basicas
    al_install_keyboard(); // Habilita entrada via teclado

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); // Cria o relogio do jogo
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Cria a fila de eventos
	ALLEGRO_FONT* font = al_create_builtin_font(); // Carrega uma fonte padrao para escrever na tela 
	ALLEGRO_DISPLAY* disp = al_create_display(X_TELA, Y_TELA); // Cria uma janela para o programa

	al_register_event_source(queue, al_get_keyboard_event_source()); // Insere  eventos de teclado na fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp)); // Insere eventos de tela na fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer)); // Insere eventos de relogio na fila de eventos

    ALLEGRO_EVENT event; // Guarda o evento capturado
    al_start_timer(timer); // Inicializa o relogio do programa

    jogador* player = cria_jogador(3, 20, 1, 50, Y_TELA/2, X_TELA, Y_TELA);
    if (!player){
        fprintf(stderr, "Erro ao criar jogador");
        return 1;
    }
    unsigned char chave_joystick[6] = {0,0,0,0,0,0}; 

    while (1) {
        al_wait_for_event(queue, &event); // Captura eventos da fila

        if (event.type == 30){
            atualiza_posicao(player, X_TELA, Y_TELA);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(player->x-player->tam_lado/2, player->y-player->tam_lado/2, player->x+player->tam_lado/2, player->y+player->tam_lado/2, al_map_rgb(255, 0, 0)); // Insere o quadrado do jogador na tela
            al_flip_display();
        }

        else if (event.type == ALLEGRO_EVENT_KEY_DOWN){ // O botão foi pressionado
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    chave_joystick[0] = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                    chave_joystick[1] = 1;
                    break;
                case ALLEGRO_KEY_LEFT:
                    chave_joystick[2] = 1;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    chave_joystick[3] = 1;
                    break;
                case ALLEGRO_KEY_E:
                    chave_joystick[4] = 1;
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
                case ALLEGRO_KEY_UP:
                    chave_joystick[0] = 0;
                    break;
                case ALLEGRO_KEY_DOWN:
                    chave_joystick[1] = 0;
                    break;
                case ALLEGRO_KEY_LEFT:
                    chave_joystick[2] = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
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
    
    al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

    return 0;
}