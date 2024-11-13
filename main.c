//Compilação: gcc main.c jogador.c joystick.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags) (!)

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>																																						//Biblioteca de figuras básicas (!)

#include "jogador.h"

#include <stdio.h>																																												//Inclusão da biblioteca de quadrados (!)

#define X_SCREEN 700																																											//Definição do tamanho da tela em pixels no eixo x (!)
#define Y_SCREEN 700    

//arrumar colisao 
unsigned char collision_2D(jogador *element_first, jogador *element_second){

	if ((((element_first->y+element_first->side/2 >= element_second->y-element_second->side/2) && (element_second->y-element_second->side/2 >= element_first->y-element_first->side/2)) || 	//			//Verifica se o primeiro elemento colidiu com o segundo no eixo X			
		((element_second->y+element_second->side/2 >= element_first->y-element_first->side/2) && (element_first->y-element_first->side/2 >= element_second->y-element_second->side/2))) && 	//			//Verifica se o segundo elemento colidiu com o primeiro no eixo X
		(((element_first->x+element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x-element_second->side/2 >= element_first->x-element_first->side/2)) || 	//			//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_second->x+element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x-element_first->side/2 >= element_second->x-element_second->side/2)))) return 1;		//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;
}

void update_position(jogador *player, jogador *objeto){																																				//Função de atualização das posições dos quadrados conforme os comandos do controle (!)
	
	if (player->control->left){																																										//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado... (!)
		jogador_move(player, 1, 0, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para a esquerda (!)
		if (collision_2D(player, objeto)) jogador_move(player, -1, 0, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player->control->right){																																										//Se o botão de movimentação para direita do controle do primeir ojogador está ativado... (!)
		jogador_move(player, 1, 1, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para a direta (!)
		if (collision_2D(player, objeto)) jogador_move(player, -1, 1, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player->control->up) {																																										//Se o botão de movimentação para cima do controle do primeiro jogador está ativado... (!)
		jogador_move(player, 1, 2, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para cima (!)
		if (collision_2D(player, objeto)) jogador_move(player, -1, 2, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player->control->down){																																										//Se o botão de movimentação para baixo do controle do primeiro jogador está ativado... (!)
		jogador_move(player, 1, 3, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para a baixo (!)
		if (collision_2D(player, objeto)) jogador_move(player, -1, 3, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
}

int main(){
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();	
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

    jogador* player = jogador_create(20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do primeiro jogador (!)
	if (!player) return 1;
    
    jogador* objeto = jogador_create(20, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do primeiro jogador (!)
	if (!objeto) return 1;																																									//Verificação de erro na criação do quadrado do primeiro jogador (!)	

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    while (1){
        al_wait_for_event(queue, &event);
        if(event.type == 30){
            update_position(player, objeto);																																						//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(objeto->x-objeto->side/2, objeto->y-objeto->side/2, objeto->x+objeto->side/2, objeto->y+objeto->side/2, al_map_rgb(0, 0, 255));			//Insere o quadrado objeto na tela
            al_draw_filled_rectangle(player->x-player->side/2, player->y-player->side/2, player->x+player->side/2, player->y+player->side/2, al_map_rgb(255, 0, 0));			//Insere o quadrado do primeiro jogador na tela
            al_flip_display();
        }

        else if ((event.type == 10) || (event.type == 12)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado (!)
			if (event.keyboard.keycode == 1) joystick_left(player->control);																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda) (!)
			else if (event.keyboard.keycode == 4) joystick_right(player->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita) (!)
			else if (event.keyboard.keycode == 23) joystick_up(player->control);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima) (!)
			else if (event.keyboard.keycode == 19) joystick_down(player->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo) (!)
		}

        else if (event.type == 42) // Evento de click no X
            break;
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    jogador_destroy(player);
    
    return 0;
}