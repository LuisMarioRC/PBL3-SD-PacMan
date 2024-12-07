#include "mouse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>
#include <errno.h>

#define MOVEMENT_THRESHOLD 1 

static int fd_mouse;
static pthread_t thread_id;
static pthread_mutex_t lock;

extern int xsoma;
extern int ysoma;

// Função da thread para leitura do mouse
static void* read_mouse(void* arg) {
    unsigned char buf[3]; // Buffer para o pacote de 3 bytes
    int x, y;

    while (1) {
        ssize_t n = read(fd_mouse, buf, sizeof(buf));
        if (n == -1) {
            perror("Error reading");
            continue;
        } else if (n != sizeof(buf)) {
            fprintf(stderr, "Error: read %ld bytes, expecting %ld\n", n, sizeof(buf));
            continue;
        }

        // Capturar movimento em X e Y
        x = (signed char)buf[1];
        y = (signed char)buf[2];

        if (abs(x) >= MOVEMENT_THRESHOLD || abs(y) >= MOVEMENT_THRESHOLD) {
            pthread_mutex_lock(&lock);
            xsoma = x;
            ysoma = y;
            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}

// Inicializa o leitor de movimentos do mouse
int init_mouse_reader() {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        return -1;
    }

    fd_mouse = open("/dev/input/mice", O_RDONLY);
    if (fd_mouse == -1) {
        perror("Error opening /dev/input/mice");
        return -1;
    }

    if (pthread_create(&thread_id, NULL, read_mouse, NULL) != 0) {
        perror("Error creating thread");
        close(fd_mouse);
        return -1;
    }

    return 0;
}

// Finaliza o leitor de movimentos do mouse
void stop_mouse_reader() {
    pthread_cancel(thread_id);
    pthread_join(thread_id, NULL);
    close(fd_mouse);
    pthread_mutex_destroy(&lock);
}
