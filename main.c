//Compilação: gcc main.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags) (!)

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> // (!) Figuras basicas - trocar 

#include "jogador.h"
#include "inimigos.h"

#include <stdio.h>

#define X_TELA 800
#define Y_TELA 700

int main(){ 
    al_init(); // Inicia biblioteca Allegro
    al_init_primitives_addon(); // Inicializa imagens básicas
    al_install_keyboard(); // Habilita entrada via teclado

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); // Cria o relógio do jogo
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Cria a fila de eventos
	ALLEGRO_FONT* font = al_create_builtin_font(); // Carrega uma fonte padrão para escrever na tela 
	ALLEGRO_DISPLAY* disp = al_create_display(X_TELA, Y_TELA); // Cria uma janela para o programa

	al_register_event_source(queue, al_get_keyboard_event_source()); // Insere  eventos de teclado na fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp)); // Insere eventos de tela na fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer)); // Insere eventos de relógio na fila de eventos

    ALLEGRO_EVENT event; // Guarda o evento capturado
    al_start_timer(timer); // Inicializa o relógio do programa

    while (1) {
        al_wait_for_event(queue, &event); // Captura eventos da fila

        if (event.type == 30){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_flip_display();
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