#ifndef MOUSE_H
#define MOUSE_H

#include <pthread.h>

// Variáveis globais para armazenar os movimentos do mouse
extern int xsoma, ysoma;


int init_mouse_reader();
void stop_mouse_reader();

#endif // MOUSE_H
