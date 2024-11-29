#include "mouse_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

// Variáveis globais
static pthread_t thread_id;   // ID da thread
static pthread_mutex_t lock;  // Mutex para sincronização
static int fd_mouse = -1;     // Descritor do dispositivo do mouse
static int xsoma = 0, ysoma = 0;  // Acumuladores para movimento do mouse
static time_t last_move_time;     // Tempo do último movimento registrado

#define MOVEMENT_THRESHOLD 1
#define INACTIVITY_TIMEOUT 1  // Tempo de inatividade em segundos

// Thread para leitura contínua do mouse
void* read_mouse(void* arg) {
    unsigned char buf[3];
    int x, y;

    while (1) {
        ssize_t n = read(fd_mouse, buf, sizeof(buf));
        if (n != sizeof(buf)) continue;

        x = (signed char)buf[1];
        y = (signed char)buf[2];

        if (abs(x) >= MOVEMENT_THRESHOLD || abs(y) >= MOVEMENT_THRESHOLD) {
            pthread_mutex_lock(&lock);
            xsoma += x;
            ysoma += y;
            last_move_time = time(NULL);  // Atualiza o tempo do último movimento
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

// Inicializa a leitura do mouse
int init_mouse_reader(void) {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        return -1;
    }

    fd_mouse = open("/dev/input/mice", O_RDONLY);
    if (fd_mouse == -1) {
        perror("Error opening /dev/input/mice");
        return -1;
    }

    last_move_time = time(NULL);

    if (pthread_create(&thread_id, NULL, read_mouse, NULL) != 0) {
        perror("Error creating thread");
        close(fd_mouse);
        return -1;
    }

    return 0;
}

// Retorna a direção do movimento do mouse
const char* get_mouse_direction(void) {
    const char* direction = "No Movement";

    pthread_mutex_lock(&lock);

    if (difftime(time(NULL), last_move_time) >= INACTIVITY_TIMEOUT) {
        xsoma = 0;
        ysoma = 0;
    }

    if (xsoma > 0) direction = "Right";
    else if (xsoma < 0) direction = "Left";
    else if (ysoma > 0) direction = "Up";
    else if (ysoma < 0) direction = "Down";

    xsoma = 0;
    ysoma = 0;

    pthread_mutex_unlock(&lock);

    return direction;
}

// Finaliza a leitura do mouse
void close_mouse_reader(void) {
    pthread_cancel(thread_id);
    pthread_join(thread_id, NULL);
    close(fd_mouse);
    pthread_mutex_destroy(&lock);
}
