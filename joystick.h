#ifndef __JOYSTICK__
#define __JOYSTICK__


typedef struct {
    unsigned char cima;
    unsigned char baixo;
    unsigned char esquerda;
    unsigned char direita;
    unsigned char tiro;
    unsigned char especial;
} joystick;

joystick* cria_joystick();
joystick* atualiza_joystick(joystick *joy, unsigned char chave_joystick[]);




#endif