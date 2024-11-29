#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Estrutura para eventos do mouse
struct input_event ev;
pthread_mutex_t lock; // Mutex para sincronização
int fd_mouse;         // Descritor do dispositivo do mouse
int xsoma = 0, ysoma = 0; // Acumuladores para movimento do mouse

#define MOVEMENT_THRESHOLD 1 
#define INACTIVITY_TIMEOUT 1  // Tempo de inatividade em segundos para resetar os valores

time_t last_move_time;  // Tempo do último movimento registrado

// Função para abrir o dispositivo físico
static int open_physical(int fd) {
    if (fd == -1) {
        if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf("ERROR: could not open \"/dev/mem\"...\n");
            return -1;
        }
    }
    return fd;
}

// Função para mapear memória física
static void* map_physical(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Mapear endereços físicos para virtuais
    virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return NULL;
    }
    return virtual_base;
}

// Thread para leitura do mouse
void* read_mouse(void* arg) {
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

        // Aplicar limiar para ignorar valores muito pequenos
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
/*

int main() {
    pthread_t thread_id;

    // Inicializar mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        return EXIT_FAILURE;
    }

    // Abrir dispositivo do mouse
    fd_mouse = open("/dev/input/mice", O_RDONLY);
    if (fd_mouse == -1) {
        perror("Error opening /dev/input/mice");
        return EXIT_FAILURE;
    }

    // Obter o tempo inicial de movimento
    last_move_time = time(NULL);

    // Criar a thread para leitura do mouse
    if (pthread_create(&thread_id, NULL, read_mouse, NULL) != 0) {
        perror("Error creating thread");
        close(fd_mouse);
        return EXIT_FAILURE;
    }

    // Loop principal para exibir valores acumulados
    printf("Monitoring mouse movements. Press Ctrl+C to exit.\n");
    while (1) {
        usleep(100000);

        pthread_mutex_lock(&lock);

        // Verifica a inatividade e zera os valores se necessário
        if (difftime(time(NULL), last_move_time) >= INACTIVITY_TIMEOUT) {
            xsoma = 0;
            ysoma = 0;
            printf("No movement detected for %d seconds. Resetting coordinates.\n", INACTIVITY_TIMEOUT);
        }

        // Exibe os valores acumulados
        printf("Mouse X Accumulated: %d, Y Accumulated: %d\n", xsoma, ysoma);

        pthread_mutex_unlock(&lock);
    }

    pthread_join(thread_id, NULL);
    close(fd_mouse);
    pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}
*/