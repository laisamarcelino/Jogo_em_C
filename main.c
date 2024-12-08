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

/*#define X_TELA 1920
#define Y_TELA 1080*/
#define X_TELA 1280
#define Y_TELA 720
#define FRAME_RATE 30.0
#define MAX_INIMIGOS 100
#define MAX_ESPECIAIS 100


void libera_mem(ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp, jogador* player, inimigo* inimigos[], infos_inimigos* infos_inimigos) {
    if (player) destroi_jogador(player);

    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i] != NULL) {
            destroi_inimigo(inimigos[i]);
        }
    }

    if (infos_inimigos) {
        if (infos_inimigos->inimigo1) al_destroy_bitmap(infos_inimigos->inimigo1);
        if (infos_inimigos->inimigo2) al_destroy_bitmap(infos_inimigos->inimigo2);
        if (infos_inimigos->inimigo3) al_destroy_bitmap(infos_inimigos->inimigo3);
        if (infos_inimigos->inimigo4) al_destroy_bitmap(infos_inimigos->inimigo4);
        if (infos_inimigos->boss1) al_destroy_bitmap(infos_inimigos->boss1);

        free(infos_inimigos);
    }

    if (font) al_destroy_font(font);
    if (disp) al_destroy_display(disp);
    if (timer) al_destroy_timer(timer);
    if (queue) al_destroy_event_queue(queue);

    al_shutdown_primitives_addon();
    al_uninstall_keyboard();
    al_shutdown_image_addon();
}

int main(){
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar Primitives Addon.\n");
        al_shutdown_primitives_addon();
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao instalar o teclado.\n");
        al_shutdown_primitives_addon();
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar Image Addon.\n");
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return -1;
    }

    srand(time(NULL));
    al_set_new_display_flags(ALLEGRO_OPENGL);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FRAME_RATE);
    if (!timer) {
        fprintf(stderr, "Falha ao criar timer.\n");
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_timer(timer);
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return -1;
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Falha ao criar fonte.\n");
        al_destroy_event_queue(queue);
        al_destroy_timer(timer);
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return -1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(X_TELA, Y_TELA);
    if (!disp) {
        fprintf(stderr, "Falha ao criar display.\n");
        al_destroy_font(font);
        al_destroy_event_queue(queue);
        al_destroy_timer(timer);
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    /* -------------------- Variáveis de Controle -------------------- */

    unsigned char chave_joystick[6] = {0};
    unsigned char fase_atual = 1;
    unsigned int frame_count = 0;
    inimigo* inimigos[MAX_INIMIGOS] = {NULL};
    bool running = true;

    /* -------------------- Criação dos Personagens -------------------- */

    jogador* player = cria_jogador(20, 928, 106, 50, Y_TELA/2, X_TELA, Y_TELA);
    if (!player){
        fprintf(stderr, "Erro ao criar jogador.\n");
        libera_mem(timer, queue, font, disp, NULL, inimigos, NULL);
        return -1;
    }

    infos_inimigos *infos_inimigos = carrega_sprites();
    if (infos_inimigos == NULL){
        fprintf(stderr, "Erro ao carregar sprites.\n");
        libera_mem(timer, queue, font, disp, player, inimigos, infos_inimigos);
        return -1;
    }

    /* -------------------- Loop Principal --------------------*/
    
    while (running) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (player->hp > 0){
                fase1(timer, player, inimigos, infos_inimigos, X_TELA, Y_TELA);
            }
            else {
                // O jogador morreu; encerrar o loop principal
                printf("Jogador morreu. Encerrando o jogo.\n");
                running = false;
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

        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false;
        }

    }

    // Após o loop, realizar a limpeza e encerrar o programa
    libera_mem(timer, queue, font, disp, player, inimigos, infos_inimigos);
    return 0;
}
