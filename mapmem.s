.global open_devmem
.type open_devmem, %function
.global close_devmen
.type close_devmen, %function
.global bg_block
.type bg_block, %function
.global poligono
.type poligono, %function
.global check_fifo
.type check_fifo, %function
.global bg_color
.type bg_color, %function
.global set_sprite
.type set_sprite, %function
.global acess_btn
.type acess_btn, %function
.global acess_display
.type acess_display, %function
.global wsm
.type wsm, %function
.global save_sprite
.type save_sprite, %function

@ Constantes
.equ data_a, 0x80
.equ data_b, 0x70
.equ wrreg, 0xc0

open_devmem:
    @ Abrir /dev/mem

    @reservando na pilha os resgitradores que vai usar
    sub sp, sp, #28
    str r1, [sp, #24]
    str r2, [sp, #20]
    str r3, [sp, #16]
    str r4, [sp, #12]
    str r5, [sp, #8]
    str r7, [sp, #4]
    str r0, [sp, #0]


    ldr r0, =devmem
    mov r1, #2              @ Leitura e escrita
    mov r2, #0              @ Modo de arquivo
    mov r7, #5              @ syscall: open
    svc 0                   @ Chamada de sistema

    mov r4, r0              @ fd
    mov r0, #0              @ Endereço base
    mov r1, #4096           @ Tamanho da página
    mov r2, #3              @ Leitura e escrita
    mov r3, #1              @ Mapeamento compartilhado
    ldr r5, =fpga_bridge   @ Endereço do bridge FPGA
    ldr r5, [r5]            @ Offset base
    mov r7, #192            @ syscall: mmap2
    svc 0                  @ Chamada de sistema

    ldr r1, =mapped_address  @ Salva o endereço mapeado
    str r0, [r1]

    @carregando os registradores que foram salvos na pilha
    ldr r1, [sp, #24]
    ldr r2, [sp, #20]
    ldr r3, [sp, #16]
    ldr r4, [sp, #12]
    ldr r5, [sp, #8]
    ldr r7, [sp, #4]
    ldr r0, [sp, #0]

    add sp, sp, #28

    bx lr

@-----------------------------------------------------------------------
close_devmen:
    @ Fechar /dev/mem

    @reservando na pilha os resgitradores que vai usar
    sub sp, sp, #12
    str r0, [sp, #8]
    str r1, [sp, #4]
    str r7, [sp, #0]

    ldr r0, =mapped_address  @ Endereço mapeado
    ldr r0, [r0]
    mov r1, #4096 @ tamanho da página mapeada
    mov r7, #91   @ system call: munmap
    svc 0
    
    ldr r0, =devmem  @ /dev/mem
    ldr r0, [r0]
    mov r7, #6   @ system call: close
    svc 0
    
    ldr r0, [sp, #8]
    ldr r1, [sp, #4]
    ldr r7, [sp, #0]

    add sp, sp, #12

    bx lr

@------------------------------------------------------------------------
bg_block:
    @ r0-> parametro posição  ( (linhas * 80) +  coluna)
    @ r1-> parametro cor

    @reservando na pilha os resgitradores que vai usar
    sub sp, sp, #28  @reservando 28 bytes na pilha
    str lr, [sp, #24]
    str r1, [sp, #20]
    str r2, [sp, #16]
    str r4, [sp, #12]
    str r5, [sp, #8]
    str r6, [sp, #4]
    str r0, [sp, #0]


    ldr r4, =mapped_address  @ Endereço mapeado
    ldr r4, [r4]

    @DATAA
    mov r2, #2      @opcode
    lsl r0, #4       @deslocamento
    add r2, r2, r0    @opcode + posição

    mov r5, #0
    mov r6, #1

    str r5, [r4, #0xc0]   @desabilitando a escrita no wrreg
    str r2, [r4, #0x80]   @passando o opcode e a posição para dataA
    str r1, [r4, #0x70]   @passando a cor para dataB
    bl check_fifo         @verifica se o fifo está cheio entes de ativar o wrreg
    str r6, [r4, #0xc0]   @ativando a escrita no wrreg
    str r5, [r4, #0xc0]   @desativando a escrita no wrreg

    @carregando os registradores que foram salvos na pilha
    ldr lr, [sp, #24]
    ldr r1, [sp, #20]
    ldr r2, [sp, #16]
    ldr r4, [sp, #12]
    ldr r5, [sp, #8]
    ldr r6, [sp, #4]
    ldr r0, [sp, #0]

    add sp, sp, #24  @liberando a pilha

    bx lr

@------------------------------------------------------------------------
check_fifo: @função que verifica se o fifo está cheio
  sub sp, sp, #8    @reservando 8 bytes na pilha
  str r0, [sp, #4]
  str r1, [sp, #0]
  
  ldr r0, =mapped_address   @ Endereço mapeado
  ldr r0, [r0]
checking:                   @label para verificar se o fifo está cheio
  ldr r1, [r0, #0xb0]       @lendo o registrador de status do fifo
  CMP r1, #1                @comparando o status do fifo com 1
  beq checking              @se o fifo estiver cheio, volta para o label checking

  ldr r0, [sp, #4]
  ldr r1, [sp, #0]
  add sp, sp, #8 
  
  bx lr

@--------------------------------------------------------------------------

poligono:  @função que desenha um poligono
  @ r0 -> cor    (rgb)
  @ r1 -> tamanho (0 a 15)
  @ r2 -> forma   (0 -> quadrado, 1 -> triângulo)
  @ r3 -> endereço  (0 a 15)
  @ r4 -> posiçãoX  
  @ r5 -> posiçãoy

    sub sp, sp, #40       @reservando 40 bytes na pilha
    str lr, [sp, #36]
    str r11, [sp, #32] 
    str r7, [sp, #28] 
    str r6, [sp, #24] 
    str r5, [sp, #20]
    str r4, [sp, #16]
    str r3, [sp, #12] 
    str r2, [sp, #8]
    str r1, [sp, #4]
    str r0, [sp, #0]
    ldr r4, [sp, #40]
    ldr r5, [sp, #44]

  ldr r11, =mapped_address        @ Endereço mapeado
  ldr r11, [r11]

  @desabilitando a escrição no wrreg
  mov r9, #0
  str r9, [r11, #wrreg]

  @DATAA
  mov r8, #3  @opcode
  lsl r3, #4
  add r3, r3, r8    @opcode + endereço

  str r3, [r11, #data_a]    @passando o opcode e o endereço para dataA

  @DATAB
  mov r6, r4 @x
  mov r7, r5 @y

  @deslocamento
  lsl r2, #31
  lsl r0, #22
  lsl r1, #18
  lsl r6, #9      
  
  @calculando o data B
  add r2, r2, r0
  add r2, r2, r1
  add r2, r2, r6
  add r2, r2, r7

  str r2, [r11, #data_b]    @passando o dataB

  mov r9, #1                @habilitando a escrita no wrreg 
  str r9, [r11, #wrreg]
  mov r9, #0                @desabilitando a escrita no wrreg
  str r9, [r11, #wrreg]

    ldr lr, [sp, #36]
    ldr r11, [sp, #32] 
    ldr r7, [sp, #28] 
    ldr r6, [sp, #24] 
    ldr r9, [sp, #20]
    ldr r8, [sp, #16]
    ldr r3, [sp, #12] 
    ldr r2, [sp, #8]
    ldr r1, [sp, #4]
    ldr r0, [sp, #0]

    add sp, sp, #40        @liberando a pilha

  bx lr

@---------------------------------------------------------------------
bg_color:   @função que seta a cor de fundo de background
    @ r0 : cor

    sub sp, sp, #16       @reservando 16 bytes na pilha
    str lr, [sp, #12]
    str r4, [sp, #8]
    str r1, [sp, #4]
    str r0, [sp, #0]


    ldr r4, =mapped_address         @ Endereço mapeado
    ldr r4, [r4]

    mov r1, r0                      @ Carrega a cor no reg
    str r1, [r4, #0x70]               @ Passa a cor para o registrador dataB

    mov r1, #0                          @ Configura opcode WBR + registrador 0 (background)
    str r1, [r4, #0x80]               @ Passa o opcode e o registrador para dataA

    mov r1, #1                         @ Sinal de início de escrita no WRREG
    bl check_fifo                      @checa de a pilha está cheia
    str r1, [r4, #0xc0]                @ Ativa a escrita no registrador WRREG
    mov r1, #0                          @ Desativa o sinal de escrita
    str r1, [r4, #0xc0]                @ Confirma a finalização da escrita

    ldr lr, [sp, #12]
    ldr r4, [sp, #8]
    ldr r1, [sp, #4]
    ldr r0, [sp, #0]
    add sp, sp, #16                   @liberando a pilha

    bx lr                             @retorna 

@----------------------------------------------------------------
set_sprite:          @função que seta o sprite

@ r0, sp (habilita o sprite) 1 bit
@ r1, pos_x 10 bits (só da pra passar até o r3 como parâmetro)
@ r2, pos_y 10 bits
@ r3, offset (escolha do sprite na memoria) 9 bits
@ sp -> registrador (layer de exibição) 5 bits quanto mais próximo de 0 mais alto
   
  sub sp, sp, #28
  str lr, [sp, #24]
  str r0, [sp, #20]
  str r1, [sp, #16]
  str r2, [sp, #12]
  str r3, [sp, #8]
  str r4, [sp, #4]
  str r5, [sp, #0]

  ldr r4, =mapped_address
  ldr r4, [r4]
  ldr r5, [sp, #28] @ recupera o 5 parâmetro (reg)
  
  lsl r0, r0, #10 @ adicionando pos_x
  add r0, r0, r1
  lsl r0, r0, #10 @ adicionando pos_y
  add r0, r0, r2
  lsl r0, r0, #9  @ adicionando o offset
  add r0, r0, r3
  str r0, [r4, #data_b]
  
  lsl r0, r5, #4 @ adicionando upcode
  str r0, [r4, #data_a]
  
  bl check_fifo

  mov r0, #1
  str r0, [r4, #wrreg]
  mov r0, #0
  str r0, [r4, #wrreg]
  
  ldr lr, [sp, #24]
  ldr r0, [sp, #20]
  ldr r1, [sp, #16]
  ldr r2, [sp, #12]
  ldr r3, [sp, #8]
  ldr r4, [sp, #4]
  str r5, [sp, #0]
  add sp, sp, #28

  bx lr
@--------------------------------------------------------------------------
acess_btn:    @função que acessa os botões

    ldr r11, =mapped_address    @ Endereço mapeado
    ldr r11, [r11]

    ldr r0, [r11, #0x0]  @endereço base botões

    bx lr

@-------------------------------------------------------------------------
acess_display:    @função que acessa o display

    ldr r11, =mapped_address    @ Endereço mapeado
    ldr r11, [r11]

    ldr r0, [r11, #0x10]      @endereço base display 1
    ldr r0, [r11, #0x20]      @endereço base display 2
    ldr r0, [r11, #0x30]      @endereço base display 3
    ldr r0, [r11, #0x40]      @endereço base display 4
    ldr r0, [r11, #0x50]      @endereço base display 5
    ldr r0, [r11, #0x60]      @endereço base display 6

    bx lr


@--------------------------------------------------------------------------

@ Altera um pixel de um sprite na memória
@ Argumentos:
@ r0 -> Endereço a ser acessado
@ r1 -> Valor BGR
wsm:

  sub sp, sp, #16
  str lr, [sp, #12]
  str r2, [sp, #8]
  str r1, [sp, #4]
  str r0, [sp, #0]
  
  ldr r2, =mapped_address
  ldr r2, [r2]

  lsl r0, r0, #4
  add r0, r0, #0b0001
  str r0, [r2, #data_a]

  str r1, [r2, #data_b]

  bl check_fifo

  mov r1, #1
  str r1, [r2, #wrreg]
  mov r1, #0
  str r1, [r2, #wrreg]

  ldr lr, [sp, #12]
  ldr r2, [sp, #8]
  ldr r1, [sp, #4]
  ldr r0, [sp, #0]
  add sp,  sp, #16

  bx lr
@@ wsm v2
@ argumentos:
@ r0 -> Vetor com 400 posições indicando a cor de cada pixel;
@ r1 -> Offset (em qual espaço da memória o sprite vai ficar)
save_sprite:
  sub sp, sp, #36
  str r0, [sp, #0]
  str r1, [sp, #4]
  str r2, [sp, #8]
  str r3, [sp, #12]
  str r4, [sp, #16]
  str r5, [sp, #20]
  str r6, [sp, #24]
  str r7, [sp, #28]
  str lr, [sp, #32]


  mov r6, r0 @ salvando o endereço do vetor para liberar o r0
  @mov r7, r1 @ salvando o offset para liberar o r1

  ldr r3, =max_values
  ldr r3, [r3, #0] @ r3 = 400

  mul r7, r1, r3 @ primeiro indice 
  add r3, r7, r3 @ ultimo indice

  @ mov r4, #0 @ contador do for
  mov r4, r7
  mov r5, #0 @ indice do vetor
save_spr_for:
  cmp r3, r4
  beq end_save_spr

  mov r0, r4 @ guarda em r0 o endereço onde determiado pixel deve ser salvo
  ldr r1, [r6, r5] @ guarda em r1 a cor que determinado pixel deve ter
  
  bl wsm @ escreve o pixel na memória de sprites

  add r4, #1
  add r5, #2
  b save_spr_for

end_save_spr:
  ldr r0, [sp, #0]
  ldr r1, [sp, #4]
  ldr r2, [sp, #8]
  ldr r3, [sp, #12]
  ldr r4, [sp, #16]
  ldr r5, [sp, #20]
  ldr r6, [sp, #24]
  ldr r7, [sp, #28]
  ldr lr, [sp, #32]
  add sp, sp, #36

  bx lr

  
.section .data
mapped_address: .space 4
devmem: .asciz "/dev/mem"
fpga_bridge: .word 0xff200             @ Endereço do bridge FPGA
max_values: .word 400, 4800
