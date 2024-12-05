// gcc main.c fases.c jogador.c joystick.c inimigos.c projeteis.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags) -lm

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>

#include "jogador.h"
#include "inimigos.h"
#include "joystick.h"
#include "projeteis.h"
#include "fases.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
#include <stdbool.h>

#define X_TELA 1920
#define Y_TELA 1080

int main(){ 
    al_init(); // Inicia biblioteca Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }
    
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

    unsigned char chave_joystick[6] = {0};
    unsigned char fase_atual = 1;
    unsigned int frame_count = 0;
    inimigo* inimigos[MAX_INIMIGOS] = {NULL};

    /* --------------------Criacao dos personagens -------------------------------------------- */
    
    jogador* player = cria_jogador(20, 928, 106, 50, Y_TELA/2, X_TELA, Y_TELA);
    if (!player){
        fprintf(stderr, "Erro ao criar jogador");
        return 1;
    }

    infos_inimigos *infos_inimigos = carrega_sprites();
    if (infos_inimigos == NULL){
        fprintf(stderr, "Erro ao carregar sprites.\n");
        return 1;
    }

    /* hp
    jogador -> hp 20 / dano 1 / especial1 2 / especial2 3
    inimigo1 -> hp 3 / dano 2
    inimigo2 -> hp 2 / sem dano por tiro
    inimigo3 -> hp 4 / dano 3
    inimigo4 -> hp 3 / dano 4
    boss1 -> hp -> hp 30 / dano 5 / especial 10
    boss2 -> hp -> hp 40 / dano 5 / especial 10 
    
    */
    
    /* --------------------------- Loop principal ------------------------------*/

    while (1) {
        al_wait_for_event(queue, &event); // Captura eventos da fila
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (player->hp >= 0){
                fase1(timer, player, inimigos, infos_inimigos, X_TELA, Y_TELA);
                printf("%u\n", player->hp);
            }
            else {
                al_clear_to_color(al_map_rgb(0, 0, 0));
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

    // Destroi inimigos
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i] != NULL) {
            destroi_inimigo(inimigos[i]);
            inimigos[i] = NULL;
        }
    }

    // Destroy loaded bitmaps
    if (infos_inimigos->inimigo1) al_destroy_bitmap(infos_inimigos->inimigo1);
    if (infos_inimigos->inimigo2) al_destroy_bitmap(infos_inimigos->inimigo2);
    if (infos_inimigos->inimigo3) al_destroy_bitmap(infos_inimigos->inimigo3);
    if (infos_inimigos->inimigo4) al_destroy_bitmap(infos_inimigos->inimigo4);

    // Free the infos_inimigos structure
    free(infos_inimigos);


    return 0;
}