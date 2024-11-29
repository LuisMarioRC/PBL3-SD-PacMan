#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "globals.h"

/**
 * Definições de endereços base
 */
#define HPS_PHYS_BASE 0xFF000000
#define HPS_SPAN 0x01000000
#define SYSMGR_BASE 0xFFD08000
#define I2C0_BASE 0xFFC04000 

/**
 * Definições de registradores do módulo SYSMGR
 */
#define SYSMGR_I2C0USEFPGA (SYSMGR_BASE + 0x704)
#define SYSMGR_GENERALIO7 (SYSMGR_BASE + 0x49C)
#define SYSMGR_GENERALIO8 (SYSMGR_BASE + 0x4A0)

/**
 * Definições de registradores do módulo I2C
 */
#define I2C0_CON (I2C0_BASE + 0x00)
#define I2C0_TAR (I2C0_BASE + 0x04)
#define I2C0_DATA_CMD (I2C0_BASE + 0x10)
#define I2C0_FS_SCL_HCNT (I2C0_BASE + 0x1C)
#define I2C0_FS_SCL_LCNT (I2C0_BASE + 0x20)
#define I2C0_ENABLE (I2C0_BASE + 0x6C)
#define I2C0_RXFLR (I2C0_BASE + 0x78)
#define I2C0_TXFLR (I2C0_BASE + 0x74)
#define I2C0_CLR_INTR (I2C0_BASE + 0x40)

/**
 * Definições de registradores do acelerômetro ADXL345
 */
#define ADXL345_ADDR 0x53
#define ADXL345_DEVID 0x00
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_INT_ENABLE 0x2E
#define ADXL345_INT_MAP 0x2F
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_BW_RATE 0x2C

#define AMOSTRAS_CALIBRACAO 100

/**
 * Variáveis globais
 */
volatile uint32_t *base_hps;
int mg_por_lsb = 4;
int offset_x = 0;
int offset_y = 0;
int fd;

/**
 * Funções auxiliares
 */
void escrever_registro(uint32_t endereco, uint32_t valor) {
   *(volatile uint32_t*)(base_hps + (endereco - HPS_PHYS_BASE) / 4) = valor;
}

uint32_t ler_registro(uint32_t endereco) {
   return *(volatile uint32_t*)(base_hps + (endereco - HPS_PHYS_BASE) / 4);
}

/**
 * Inicialização do módulo I2C
 */
void inicializar_i2c() {
   escrever_registro(SYSMGR_I2C0USEFPGA, 0);
   escrever_registro(SYSMGR_GENERALIO7, 1);
   escrever_registro(SYSMGR_GENERALIO8, 1);

   escrever_registro(I2C0_ENABLE, 0x0);
   escrever_registro(I2C0_CON, 0x65);
   escrever_registro(I2C0_TAR, ADXL345_ADDR);
   escrever_registro(I2C0_FS_SCL_HCNT, 90);
   escrever_registro(I2C0_FS_SCL_LCNT, 160);

   escrever_registro(I2C0_ENABLE, 0x1);
   usleep(10000);
}

/**
 * Funções de leitura e escrita no ADXL345
 */
void ADXL345_REG_READ(uint8_t address, uint8_t *value) {
   escrever_registro(I2C0_DATA_CMD, address + 0x400);
   escrever_registro(I2C0_DATA_CMD, 0x100);
   while (ler_registro(I2C0_RXFLR) == 0) {}
   *value = ler_registro(I2C0_DATA_CMD) & 0xFF;
}

void ADXL345_REG_WRITE(uint8_t address, uint8_t value) {
   escrever_registro(I2C0_DATA_CMD, address + 0x400);
   escrever_registro(I2C0_DATA_CMD, value);
}

uint8_t ler_i2c(uint8_t endereco_reg) {
   uint8_t valor;
   ADXL345_REG_READ(endereco_reg, &valor);
   return valor;
}

void escrever_i2c(uint8_t endereco_reg, uint8_t valor) {
   ADXL345_REG_WRITE(endereco_reg, valor);
}

/**
 * Inicialização e calibração do acelerômetro
 */
void inicializar_adxl345() {
   escrever_i2c(ADXL345_POWER_CTL, 0x00);
   usleep(10000);
   escrever_i2c(ADXL345_POWER_CTL, 0x08);
   escrever_i2c(ADXL345_DATA_FORMAT, 0x00);
   escrever_i2c(ADXL345_BW_RATE, 0x0A);
   escrever_i2c(ADXL345_INT_ENABLE, 0x80);
   escrever_i2c(ADXL345_INT_MAP, 0x00);
}

void calibrar_acelerometro(int16_t *offset_x, int16_t *offset_y) {
   int32_t soma_x = 0, soma_y = 0;
   int16_t x, y;

   for (int i = 0; i < AMOSTRAS_CALIBRACAO; i++) {
       x = (ler_i2c(ADXL345_DATAX1) << 8) | ler_i2c(ADXL345_DATAX0);
       y = (ler_i2c(ADXL345_DATAY1) << 8) | ler_i2c(ADXL345_DATAY0);
       soma_x += x;
       soma_y += y;
       usleep(10000);
   }

   *offset_x = soma_x / AMOSTRAS_CALIBRACAO;
   *offset_y = soma_y / AMOSTRAS_CALIBRACAO;

   printf("Calibração completa. Offset X=%d, Y=%d\n", *offset_x, *offset_y);
}

int configura_acelerometro() {
   fd = open("/dev/mem", O_RDWR | O_SYNC);
   if (fd < 0) {
       printf("Erro ao abrir /dev/mem\n");
       return -1;
   }

   base_hps = mmap(NULL, HPS_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HPS_PHYS_BASE);
   if (base_hps == MAP_FAILED) {
       printf("Erro ao mapear memória\n");
       close(fd);
       return -1;
   }

   inicializar_i2c();
   inicializar_adxl345();

   printf("Calibrando acelerômetro...\n");
   calibrar_acelerometro(&offset_x, &offset_y);
   return 0;
}

/**
 * Leituras ajustadas
 */
void ler_aceleracao_x_ajustada(float *x_g) {
   int16_t x_bruto = (ler_i2c(ADXL345_DATAX1) << 8) | ler_i2c(ADXL345_DATAX0);
   *x_g = (x_bruto - offset_x) * (mg_por_lsb / 1000.0);
}

void ler_aceleracao_y_ajustada(float *y_g) {
   int16_t y_bruto = (ler_i2c(ADXL345_DATAY1) << 8) | ler_i2c(ADXL345_DATAY0);
   *y_g = (y_bruto - offset_y) * (mg_por_lsb / 1000.0);
}

int desmapear() {
   munmap((void*)base_hps, HPS_SPAN);
   close(fd);
   return 0;
}

/**
 * Main para teste
 */
int main() {
   if (configura_acelerometro() == 0) {
       printf("Acelerômetro configurado com sucesso!\n");

       float x, y;
       while (1) {
           ler_aceleracao_x_ajustada(&x);
           ler_aceleracao_y_ajustada(&y);
           printf("Aceleração ajustada: X=%.2f g, Y=%.2f g\n", x, y);
           usleep(100000);
       }
   }

   desmapear();
   return 0;
}
