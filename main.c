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
#include "mouse.h"
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


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
extern int acess_btn();



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

int sprite_x = 92;  // Posição inicial X
int sprite_y = 48;  // Posição inicial Y

extern int xsoma;
extern int ysoma;

int ghost_x = 300; // Posição inicial X do ghost
int ghost_y = 300; // Posição inicial Y do ghost

int vidaPacman= 3;


int verificar_colisao_quina8x8() {
    // Verifica as 4 quinas (cantos) do sprite 8x8
    if (lab[(sprite_y / 8) - 5][(sprite_x / 8) - 8] == 1 ||  // Canto superior esquerdo
        lab[(sprite_y / 8) - 5][(sprite_x / 8) - 8 + 1] == 1 ||  // Canto superior direito
        lab[(sprite_y / 8) - 5 + 1][(sprite_x / 8) - 8] == 1 ||  // Canto inferior esquerdo
        lab[(sprite_y / 8) - 5 + 1][(sprite_x / 8) - 8 + 1] == 1) {  // Canto inferior direito
        return 1;  // Se alguma quina colidir com a parede (valor 1), retorna 1 (colidiu)
    }
    return 0;  // Se nenhuma quina colidir, retorna 0 (não colidiu)
}
void mover_pacman() {
    int direcao = get_direcao_movimento();  // Obtém a direção de movimento
    int posicao_spriteX = (sprite_x / 8) -8; // Posição do sprite no labirinto 
    int posicao_spriteY = (sprite_y / 8) -5;  // Posição do sprite no labirinto
    
    //printf("Posição x: %d ", posicao_spriteX);
    //printf("Posição y: %d\n", posicao_spriteY);
    //printf("lab: %d \n", lab[posicao_spriteY][posicao_spriteX]);

    int nova_posicao_x = sprite_x;
    int nova_posicao_y = sprite_y;

    // Verifica a direção e a posição para o movimento
    if (direcao == 1) { // Direita
        nova_posicao_x = sprite_x + 8;
        // Verifica se o caminho à direita é livre 
        //printf("PosiçãoX +1 = %d", lab[posicao_spriteY][posicao_spriteX +1]);
        if (lab[posicao_spriteY][posicao_spriteX +1] != 1) {
            sprite_x = nova_posicao_x;
            set_sprite(1, sprite_x, sprite_y, 19, 1);
        }
    } 
    else if (direcao == -1) { // Esquerda
        nova_posicao_x = sprite_x - 8;
        // Verifica se o caminho à esquerda é livre 
        //printf("PosiçãoX -1 = %d", lab[posicao_spriteY][posicao_spriteX -1]);
        if (lab[posicao_spriteY][posicao_spriteX - 3] != 1) {
            sprite_x = nova_posicao_x;
            set_sprite(1, sprite_x, sprite_y, 21, 1);
        }
    } 
    else if (direcao == 2) {  // Para cima
        nova_posicao_y = sprite_y - 8;  
        // Verifica se o caminho acima é livre 
        //printf("PosiçãoY -1 = %d", lab[posicao_spriteY - 1][posicao_spriteX ]);
        if (lab[posicao_spriteY - 1][posicao_spriteX] != 1) {
            sprite_y = nova_posicao_y;
            set_sprite(1, sprite_x, sprite_y, 22, 1);
        }
    } 
    else if (direcao == -2) { // Para baixo
        nova_posicao_y = sprite_y + 8;
        // Verifica se o caminho abaixo é livre 
        //printf("PosiçãoY -1 = %d", lab[posicao_spriteY + 2][posicao_spriteX ]);
        if (lab[posicao_spriteY + 3][posicao_spriteX] != 1) {
            sprite_y = nova_posicao_y;
            set_sprite(1, sprite_x, sprite_y, 23, 1);
        }
    }

}


int maca2_x = 92;  // Posição inicial x da maca2
int maca2_y = 442; // Posição fixa y da maca2 (linha no labirinto)

void mover_maca2_labirinto() {
    static int direcao = 1;  // Inicialmente, a maca2 se move para a direita


    int nova_pos_x = maca2_x + (direcao * 8);
    int posicao_maca2X = nova_pos_x / 8;  // Converte para índice da matriz (assumindo células de 8px)
    int posicao_maca2Y = maca2_y / 8;    // Posição fixa na matriz (linha do labirinto)

    // Verifica colisão antes de mover
    if (direcao == 1 && lab[posicao_maca2Y][posicao_maca2X] != 1) {
        // Movendo para a direita
        maca2_x = nova_pos_x;
    } else if (direcao == -1 && lab[posicao_maca2Y][posicao_maca2X] != 1) {
        // Movendo para a esquerda
        maca2_x = nova_pos_x;
    } else {
        // Colisão detectada, inverte a direção
        direcao *= -1;
    }

    // Verifica limites e ajusta direção
    if (maca2_x >= 390) {
        maca2_x = 390;
        direcao = -1;  // Muda para a esquerda
    } else if (maca2_x <= 92) {
        maca2_x = 92;
        direcao = 1;   // Muda para a direita
    }

    // Atualiza a posição gráfica da maca2
    set_sprite(1, maca2_x, maca2_y, 27, 20);



    
    
}

int morango_x = 230;  // Posição inicial x do morango
int morango_y = 225;  // Posição fixa y do morango (linha no labirinto)

void mover_morango_labirinto() {
    static int direcao = 1;  // Inicialmente, a maca2 se move para a direita


    int nova_pos_x = morango_x + (direcao * 8);
    int posicao_morango_x = nova_pos_x / 8;  // Converte para índice da matriz (assumindo células de 8px)
    int posicao_morango_y = morango_y / 8;    // Posição fixa na matriz (linha do labirinto)

    // Verifica colisão antes de mover
    if (direcao == 1 && lab[posicao_morango_y][posicao_morango_x] != 1) {
        // Movendo para a direita
        morango_x = nova_pos_x;
    } else if (direcao == -1 && lab[posicao_morango_y][posicao_morango_x] != 1) {
        // Movendo para a esquerda
        morango_x = nova_pos_x;
    } else {
        // Colisão detectada, inverte a direção
        direcao *= -1;
    }

    // Verifica limites e ajusta direção
    if (morango_x >= 400) {
        morango_x = 400;
        direcao = -1;  // Muda para a esquerda
    } else if (morango_x <= 220) {
        morango_x = 230;
        direcao = 1;   // Muda para a direita
    }

    // Atualiza a posição gráfica da maca2
    set_sprite(1, morango_x, morango_y, 28, 21);



    
    
}





// Função que será chamada no loop principal para mover o sprite automaticamente
/*void loop() {
    mover_sprite_labirinto();  // Faz o sprite passear pelo labirinto
    // Outras funções do jogo, como renderizar o labirinto, etc.
}*/

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
               //bg_block(posicao, BRANCO);  // Cor do caminho
                //set_sprite(20,i,j,1, 1);
            }
            // Espaços vazios (0) - onde o Pac-Man pode coletar pontos
            else if (lab[i][j] == 0) {
                bg_block(posicao, PRETO);  // Cor dos pontos
            }
            
        }
    }
}


/**
 * Função para desabilitar o background
 * A função desabilita o background
 * @return void
 */
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
 * Função para mover o sprite no labirinto
 * A função move o sprite no labirinto
 * @return void
 */
int get_direcao_mouse() {
    pthread_mutex_lock(&lock);
    int x = xsoma;
    int y = ysoma;
    pthread_mutex_unlock(&lock);

    int sensibilidade = 10;  // Aumenta a sensibilidade

    if (xsoma > 2 * sensibilidade) {
        return 1;
    } else if (xsoma < -2 * sensibilidade) {
        return -1;
    } else if (ysoma > 2 * sensibilidade) {
        return 2;
    } else if (ysoma < -2  * sensibilidade) {
        return -2;
    }
}

/**
 * Função para mover o ghost
 * A função move o ghost no labirinto
 * @return void
 */
void mover_ghost() {
    int ghost_direcao = get_direcao_mouse();
    if (ghost_direcao == 0) {
        return; // Não faz nada se não há movimento
    }

    int posicao_ghostX = (ghost_x / 8) - 8; // Posição do ghost no labirinto
    int posicao_ghostY = (ghost_y / 8) - 5; // Posição do ghost no labirinto
    
    int nova_posicao_x = ghost_x;
    int nova_posicao_y = ghost_y;
    
    if (ghost_direcao == 1) { // Direita
        nova_posicao_x = ghost_x + 8;
        if (lab[posicao_ghostY][posicao_ghostX + 1] != 1) {
            ghost_x = nova_posicao_x;
        }
    } else if (ghost_direcao == -1) { // Esquerda
        nova_posicao_x = ghost_x - 8;
        if (lab[posicao_ghostY][posicao_ghostX - 1] != 1) {
            ghost_x = nova_posicao_x;
        }
    } else if (ghost_direcao == 2) { // Para cima
        nova_posicao_y = ghost_y - 8;
        if (lab[posicao_ghostY - 1][posicao_ghostX] != 1) {
            ghost_y = nova_posicao_y;
        }
    } else if (ghost_direcao == -2) { // Para baixo
        nova_posicao_y = ghost_y + 8;
        if (lab[posicao_ghostY + 1][posicao_ghostX] != 1) {
            ghost_y = nova_posicao_y;
        }
    }


    // Atualiza o sprite do ghost na nova posição
    set_sprite(1, ghost_x, ghost_y, 25, 2);
}



/**
 * Função para verificar colisão
 */

int checkcollision(int x1, int y1, int x2, int y2, int size_x1, int size_y1, int size_x2, int size_y2) {
    return x1 < (x2 + size_x2) && (x1 + size_x1) > x2 && y1 < (y2 + size_y2) && (y1 + size_y1) > y2;
}

/**
 * Função para verificar colisão com o ghost
 * A função verifica se houve colisão com o ghost
 * @return int - 0 se não houve colisão, 1 se houve colisão
 */
int verificar_colisao() {
    if (checkcollision(sprite_x, sprite_y, ghost_x, ghost_y, 20, 20, 20, 20)) {
        set_sprite(0, sprite_x, sprite_y, 22, 1);
        sprite_x = 92;
        sprite_y = 48;
        set_sprite(1, sprite_x, sprite_y, 22, 1);
        vidaPacman--;
        ghost_x = 300;
        ghost_y = 300;
        set_sprite(1, ghost_x, ghost_y, 25, 2);

        if (vidaPacman == 2) {
            set_sprite(0, 600, 300, 19, 10);
        } else if (vidaPacman == 1) {
            set_sprite(0, 600, 300, 19, 9);
        } else if (vidaPacman == 0) {
            set_sprite(0, 600, 300, 19, 8);
        }
    }
    return 0;
} 
/**
 * Função para verificar colisão com a maca
 * A função verifica se houve colisão com a maca
 *
 */
int verificar_colisao_pac_maca() {
    if (checkcollision(sprite_x, sprite_y, maca2_x, maca2_y, 20, 20, 20, 20)) {
        set_sprite(0, sprite_x, sprite_y, 22, 1);
        sprite_x = 92;
        sprite_y = 48;
        set_sprite(1, sprite_x, sprite_y, 22, 1);
        vidaPacman--;

        if (vidaPacman == 2) {
            set_sprite(0, 600, 300, 19, 10);
        } else if (vidaPacman == 1) {
            set_sprite(0, 600, 300, 19, 9);
        } else if (vidaPacman == 0) {
            set_sprite(0, 600, 300, 19, 8);
        }
    }
    return 0;
} 

// Variável global para controlar a quantidade de maçãs comidas
 int macaComida =0;
 int l1= 0;
 int l2= 0;
int l3= 0;
int l4= 0;

/**
 * Função para verificar colisão com a maca
 * A função verifica se houve colisão com a maca
 * @return int - 0 se não houve colisão, 1 se houve colisão
 */
int verificar_colisao_maca() {
    if (checkcollision(sprite_x, sprite_y, 92, 410, 20, 20, 20, 20)&& l1 == 0) {
        set_sprite(0, 92,410, 26,6);
        macaComida ++;
        l1++;


    }  else  if (checkcollision(sprite_x, sprite_y, 292, 200, 20, 20, 20, 20) && l2 == 0) {
        set_sprite(0, 292,200, 26,4);
        macaComida ++;
        l2++;

    }else  if (checkcollision(sprite_x, sprite_y, 280, 442, 20, 20, 20, 20) && l3 == 0) {
        set_sprite(0, 280,442, 26,7);
        macaComida ++;
        l3++;
    }else  if (checkcollision(sprite_x, sprite_y, 390, 48, 20, 20, 20, 20) && l4 == 0) {
        set_sprite(0, 390,48, 26,11);
        macaComida ++;
        l4++;
    }
    if (macaComida == 1){
        set_sprite(1, 500,250, 26,12);
    } else if (macaComida ==2) {
        set_sprite(1, 550,250, 26,13);
    } else if (macaComida ==3){
        set_sprite(1, 600,250, 26,14);
    }

    //printf("Maca comida: %d\n", macaComida);
    return 0;
}
// Variável global para controlar o estado do jogo
int gameOn= 1;
/**
 * Função para verificar se o jogo acabou
 * A função verifica se o jogo acabou
 * @return int - 1 se o jogo acabou, 0 se o jogo não acabou
 */
void fim_de_jogo(){
    if( macaComida == 3){
        bg_desable();
        set_sprite(0, 292,200, 26,4);
        set_sprite(0, 92,410, 26,6);
        set_sprite(0, 280,442, 26,7);
        set_sprite(0, 390,48, 26,11);
        set_sprite(0, ghost_x, ghost_y, 25, 2);
        set_sprite(0, 500,250, 26,12);
        set_sprite(0, 550,250, 26,13);
        set_sprite(0, 600,250, 26,14);
        set_sprite(0, 500,200, 19,8);
        set_sprite(0, 550,200, 19,9);
        set_sprite(0, 600,200, 19,10);
        set_sprite(0, sprite_x,sprite_y, 19,1);
        set_sprite(0, maca2_x, maca2_y, 27, 20);
        gameOn =0;
        set_sprite(1, 290,220, 19,7);
    }
    if( vidaPacman == 0){
        bg_desable();
        set_sprite(0, 292,200, 26,4);
        set_sprite(0, 92,410, 26,6);
        set_sprite(0, 280,442, 26,7);
        set_sprite(0, 390,48, 26,11);
        set_sprite(0, ghost_x, ghost_y, 25, 2);
        set_sprite(0, 500,250, 26,12);
        set_sprite(0, 550,250, 26,13);
        set_sprite(0, 600,250, 26,14);
        set_sprite(0, 500,200, 19,8);
        set_sprite(0, 550,200, 19,9);
        set_sprite(0, 600,200, 19,10);
        set_sprite(0, maca2_x, maca2_y, 27, 20);
        set_sprite(0, sprite_x,sprite_y, 19,1);
        gameOn =0;
        set_sprite(1, 290,220, 25,7);
    }
}




/**
 * Função para verificar qual botão foi pressionado
 * A função verifica qual botão foi pressionado
 * @return int - Número do botão pressionado
 */
int button() {
    switch (acess_btn()) {
    case 0b1110:
        return 1;  // Botão 1
    case 0b1101:
        return 2;  // Botão 2
    case 0b1011:
        return 3;  // Botão 3
    default:
        return NULL;
    }
}

// Variável global para controlar o estado de pausa
int isPaused = 0;

/**
 * Função para alternar o estado de pausa
 * A função alterna o estado de pausa
 */
void TogglePause() {
    isPaused = !isPaused;  // Alterna o estado de pausa
    if (isPaused) {
        printf("Jogo pausado\n");
    } else {
        printf("Jogo despausado\n");
    }
}

/**
 * Função para pausar o jogo
 * A função pausa o jogo
 */
void *PauseGame(void *arg) {
    int btn;
    while (1) {
        btn = button();
        if (btn == 2) {  // Pressione o botão 2 para alternar o estado de pausa
            TogglePause();
            usleep(200000);  // Evita múltiplos registros da mesma pressão
        }
        usleep(100000);  // Pequeno atraso para não sobrecarregar a CPU
    }
}

/**
 * Função para reiniciar o jogo
 * A função reinicia o jogo
 */
void reiniciar_jogo() {
    sprite_x = 92;  // Posição inicial X do Pac-Man
    sprite_y = 48;  // Posição inicial Y do Pac-Man
    ghost_x = 300;  // Posição inicial X do Ghost
    ghost_y = 300;  // Posição inicial Y do Ghost
    vidaPacman = 3; // Reinicia o número de vidas
    macaComida =0;

            bg_desable();
        set_sprite(0, 292,200, 26,4);
        set_sprite(0, 92,410, 26,6);
        set_sprite(0, 280,442, 26,7);
        set_sprite(0, 390,48, 26,11);
        set_sprite(0, ghost_x, ghost_y, 25, 2);
        set_sprite(0, 500,250, 26,12);
        set_sprite(0, 550,250, 26,13);
        set_sprite(0, 600,250, 26,14);
        set_sprite(0, 500,200, 19,8);
        set_sprite(0, 550,200, 19,9);
        set_sprite(0, 600,200, 19,10);
        set_sprite(0, maca2_x, maca2_y, 27, 20);
        set_sprite(0, sprite_x,sprite_y, 19,1);

    // Redesenha o labirinto
    labirinto_pacman();


         set_sprite(1, 500,200, 19,8);
    set_sprite(1, 550,200, 19,9);
    set_sprite(1, 600,200, 19,10);

     l1= 0;
 l2= 0;
 l3= 0;
 l4= 0;
    //----------------------------
    //maca
            set_sprite(0, 500,250, 26,12);

        set_sprite(0, 550,250, 26,13);

        set_sprite(0, 600,250, 26,14);
    
    //----------------

    set_sprite(1, 292,200, 26,4);
    set_sprite(1, 92,410, 26,6);
    set_sprite(1, 280,442, 26,7);
    set_sprite(1, 390,48, 26,11);
    set_sprite(1, ghost_x, ghost_y, 25, 2);

    // Posiciona os sprites nas posições iniciais
    set_sprite(1, sprite_x, sprite_y, 19, 1);  // Pac-Man
    set_sprite(2, ghost_x, ghost_y, 20, 1);    // Ghost

    printf("Jogo reiniciado!\n");
}

/**
 * Função para reiniciar o jogo
 * A função reinicia o jogo
 * @param arg - Argumento da função
 * @return NULL
 */
void *ReiniciarJogo(void *arg) {
    int btn;
    while (1) {
        btn = button();  // Verifica qual botão foi pressionado
        if (btn == 1) {  // Pressione o botão 1 para reiniciar o jogo
            pthread_mutex_lock(&lock);  // Trava o mutex para evitar condições de corrida
            reiniciar_jogo();           // Chama a função de reinício
            pthread_mutex_unlock(&lock);  // Destrava o mutex
            usleep(200000);  // Evita múltiplos registros da mesma pressão
        }
        usleep(100000);  // Pequeno atraso para não sobrecarregar a CPU
    }
}

/**
 * Função para desativar o jogo
 * A função desativa o jogo
 * @param arg - Argumento da função
 * @return NULL
 */
void *DesativarJogo(void *arg) {
    int btn;
    while (1) {
        btn = button();  // Verifica qual botão foi pressionado
        if (btn == 3) {  // Pressione o botão 3 para desativar o jogo
                bg_desable();
        set_sprite(0, 292,200, 26,4);
        set_sprite(0, 92,410, 26,6);
        set_sprite(0, 280,442, 26,7);
        set_sprite(0, 390,48, 26,11);
        set_sprite(0, ghost_x, ghost_y, 25, 2);
        set_sprite(0, 500,250, 26,12);
        set_sprite(0, 550,250, 26,13);
        set_sprite(0, 600,250, 26,14);
        set_sprite(0, 500,200, 19,8);
        set_sprite(0, 550,200, 19,9);
        set_sprite(0, 600,200, 19,10);
        set_sprite(0, sprite_x,sprite_y, 19,1);
        set_sprite(0, maca2_x, maca2_y, 27, 20);
            pthread_mutex_lock(&lock);  // Trava o mutex para evitar condições de corrida
            gameOn = 0;                // Altera o estado do jogo para desativado
            pthread_mutex_unlock(&lock);  // Destrava o mutex
            printf("Jogo desativado!\n");
            break;  // Sai do loop para encerrar a thread
        }
        usleep(100000);  // Pequeno atraso para não sobrecarregar a CPU
    }
    return NULL;
}


int main(){

    open_devmem();
    configura_acelerometro();
    bg_desable();
    bg_color(PRETO);
    save_sprite(pac_man1, 19);
    save_sprite(pac_man4, 21);
    save_sprite(pac_man9, 22);
    save_sprite(pac_man8, 23);
    save_sprite(ghost, 25);
    save_sprite(maca, 26);
    save_sprite(maca2,27);
    save_sprite(barry,28);


    set_sprite(1, sprite_x,sprite_y, 19,1);

    pthread_t thread_reinicio;
    pthread_create(&thread_reinicio, NULL, ReiniciarJogo, NULL);

     pthread_t pauseThread;   // Thread de pausa

     pthread_create(&pauseThread, NULL, PauseGame, NULL);          // Cria a thread para alternar pausa


        pthread_t thread_desativar;
    pthread_create(&thread_desativar, NULL, DesativarJogo, NULL);
    //vidas
    set_sprite(1, 500,200, 19,8);
    set_sprite(1, 550,200, 19,9);
    set_sprite(1, 600,200, 19,10);
    //----------------------------
    //maca
            set_sprite(0, 500,250, 26,12);

        set_sprite(0, 550,250, 26,13);

        set_sprite(0, 600,250, 26,14);
    
    //----------------

    set_sprite(1, 292,200, 26,4);
    set_sprite(1, 92,410, 26,6);
    set_sprite(1, 280,442, 26,7);
    set_sprite(1, 390,48, 26,11);
    set_sprite(1, ghost_x, ghost_y, 25, 2);


    labirinto_pacman();
    if (init_mouse_reader() != 0) {
        fprintf(stderr, "Failed to initialize mouse reader\n");
        return 1;
    }

    while (gameOn)
    {
        if (isPaused) {
            usleep(100000);  // Reduz o uso da CPU enquanto o jogo está pausado
            continue;
        }
        verificar_colisao_pac_maca();
        mover_pacman(); // Atualiza a posição do sprite
        mover_ghost();
        mover_morango_labirinto();
        mover_maca2_labirinto();
        verificar_colisao_maca(); 
        fim_de_jogo();
            printf("Vida Pacman: %d\n", vidaPacman );

        verificar_colisao();
        usleep(100000); // Delay para evitar movimentação muito rápida (100ms)
    } 


    stop_mouse_reader();    
    pthread_cancel(pauseThread);
    pthread_join(pauseThread, NULL);
    return 0;
    
}

        
    


