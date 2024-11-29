#ifndef MOUSE_READER_H
#define MOUSE_READER_H

#define MOVEMENT_THRESHOLD 1 
#define INACTIVITY_TIMEOUT 1  // Tempo de inatividade em segundos para resetar os valores

// Inicializa a leitura do mouse
int init_mouse_reader(void);

// Finaliza a leitura do mouse
void close_mouse_reader(void);

// Verifica o movimento atual e retorna uma string indicando a direção
const char* get_mouse_direction(void);

#endif
