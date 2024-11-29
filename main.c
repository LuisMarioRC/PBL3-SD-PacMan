#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // Para usleep()
#include "acl345.c"
#include <pthread.h>
#include "globals.h"
#include <sys/mman.h>
#include <fcntl.h>
#include "labirinto.c"
#include "sprites.c"
#include "dicSprites.c"


// Declarações das funções assembly
extern void open_devmem();
extern void bg_block(int posicao, int cor);
extern void poligono(int cor, int tamanho, int forma, int endereco);
extern void bg_color(int cor);
extern void set_sprite(int sp, int px, int py, int sprite, int reg);
extern void close_devmen();
extern int acess_btn();
extern void acess_display();
extern void save_sprite(unsigned short sprite[400], int endereco);



#define PRETO   0
#define LARANJA 38
#define VERDE   56
#define AMARELO 63
#define ROXO    467
#define CIANO   504
#define BRANCO  511
#define INCOLOR 510
#define ROSA 342
#define AZUL 400

int sprite_x = 150;  // Posição inicial X
int sprite_y = 50;  // Posição inicial Y




void mover_sprite() {
    int direcao = get_direcao_movimento(); // Obtém a direção de movimento
    
    // Atualiza a posição do sprite com base na direção
    if (direcao == 1) {         // Direita
        sprite_x += 4;
        set_sprite(1, sprite_x, sprite_y, 19, 1);
    } else if (direcao == -1) { // Esquerda
        sprite_x -= 4;
        set_sprite(1, sprite_x, sprite_y, 21, 1);
    } else if (direcao == 2) {  // Para cima
        sprite_y -= 1;
    } else if (direcao == -2) { // Para baixo
        sprite_y += 1;
    }

}
void labirinto_pacman() {
    int leftBorderX = 10;    // Posição inicial x para o labirinto
    int topBorderY = 5;      // Posição inicial y para o labirinto
    int num_colunas = 80;    // Número de colunas na tela
    int num_linhas = 60;     // Número de linhas na tela


        for (int i = 0; i < 54; i++) {
        for (int j = 0; j < 43; j++) {
            int posicao = (topBorderY + i) * num_colunas + (leftBorderX + j);

            // Desenha as paredes (1)
            if (lab[i][j] == 1) {
                bg_block(posicao, AZUL);  // Cor das paredes
            }
            // Desenha os caminhos (2)
            else if (lab[i][j] == 2) {
                bg_block(posicao, PRETO);  // Cor do caminho
                //set_sprite(0,i,j,20, j);
            }
            else if (lab[i][j] == 3) {
                bg_block(posicao, BRANCO);  // Cor do caminho
                //set_sprite(20,i,j,1, 1);
            }
            // Espaços vazios (0) - onde o Pac-Man pode coletar pontos
            else if (lab[i][j] == 0) {
                bg_block(posicao, PRETO);  // Cor dos pontos
            }
        }
    }
}


void sprites_saved(){
    int endereco = 10;
    for(int i = 0; i < 11; i++){
        unsigned short sprite = dicio_sprites[i];
        save_sprite(sprite, endereco);
        endereco += 1;
    }
}


void bg_desable(){
    int num_colunas = 80;
    int num_linhas = 4800;
    int c;
    bg_color(PRETO);
    for (c=0; c < 16; c++) {
        poligono(0, 0, 0, c);
    }
    for(c=0; c < 4800; c++){
        bg_block(c, 510);
    }
}
/**
int checar_colisao(int x, int y) {
    // Verificar se a posição está dentro dos limites do mapa
    if (x < 0 || x >= NUM_COLUNAS || y < 0 || y >= NUM_LINHAS) {
        return 1;  // Fora dos limites
    }
    return map[y][x] == 1;  // Retorna 1 se for uma parede
}

void mover_sprite_com_acelerometro() {
    if (accelX > 0) {
        sprite_x++;  // Move para a direita
    } else if (accelX < 0) {
        sprite_x--;  // Move para a esquerda
    }
} **/

int main(){

    open_devmem();
    configura_acelerometro();
    bg_desable();

    labirinto_pacman();

    while (1)
    {
        mover_sprite(); // Atualiza a posição do sprite
        usleep(100000); // Delay para evitar movimentação muito rápida (100ms)
    } 

    //set_sprite(0,50,50,20, 1);
    //set_sprite(0,100,100,3,2);
    
}

        
      



