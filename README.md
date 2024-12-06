<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=f5d442&height=200&section=header&text=Jogo%20PacMan&fontSize=65&fontAlignY=38&desc=Projeto+de+Sistemas+Digitais&descAlignY=51&descAlign=62"/>

[![Typing SVG](https://readme-typing-svg.herokuapp.com/?color=f5d442&size=35&center=true&vCenter=true&width=1000&lines=Projeto+de+Sistemas+Digitais;Desenvolvimento+em+plataforma+de+hardware)](https:git.io/typing-svg)

---

## **Contexto do Projeto**

O objetivo deste projeto é desenvolver um jogo inspirado no clássico Pac-Man para a plataforma DE1-SoC, com um diferencial: o jogo será multiplayer, onde um jogador controla o Pac-Man e outro controla o Fantasma. Além disso, será criada uma biblioteca gráfica em Assembly para manipulação de vídeo, permitindo a exibição do jogo diretamente em um monitor conectado à interface VGA, sem o uso de bibliotecas de vídeo externas. O restante do código será escrito em C.

O Pac-Man será controlado por um acelerômetro integrado, cuja programação será feita em C, exigindo a implementação direta da captura e interpretação dos dados de inclinação. O Fantasma, por outro lado, será controlado pelo mouse, permitindo que o jogador o mova livremente pelo mapa. 

O objetivo do Pac-Man é coletar três maçãs para vencer o Fantasma. Já o Fantasma deve eliminar as três vidas do Pac-Man para sair vitorioso.

---

![img](midias/imagem1.jpg)

---

## **Objetivos de Aprendizagem**  
Ao final deste projeto, o/a discente será capaz de:  

1. **Aplicar conhecimentos de interação hardware-software;**  
2. **Compreender o mapeamento de memória em uma arquitetura ARM;**  
3. **Utilizar a interface de conexão entre HPS e FPGA da DE1-SoC;**  
4. **Programar em assembly para um processador com arquitetura ARM;**  
5. **Entender políticas de gerenciamento de sistema operacional Linux em arquitetura ARM;**  
6. **Compreender os princípios básicos da arquitetura da plataforma DE1-SoC;**  
7. **Utilizar lógica de threads para lidar com múltiplos processos simultâneos;**  
8. **Implementar e integrar periféricos externos, como o mouse, na comunicação com o sistema.**  

---

## **Requisitos**  

O problema a ser desenvolvido no Kit FPGA DE1-SoC deve atender às seguintes restrições:  

1. **O código do jogo deve ser escrito em linguagem C, e a biblioteca do Processador Gráfico deve ser implementada em Assembly;**  
2. **O sistema só poderá utilizar os componentes disponíveis na placa DE1-SoC;**  
3. **A biblioteca gráfica deve conter todas as funções necessárias para o desenvolvimento do jogo e incluir no mínimo um novo sprite armazenado na memória, que será utilizado durante o jogo;**  
4. **Os dois jogadores devem jogar simultaneamente, utilizando obrigatoriamente o acelerômetro e, adicionalmente, o mouse para controle;**  
6. **As informações do jogo (placar, vidas, etc.) devem ser exibidas na tela;**  
7. **O jogo deve permitir interações do usuário por meio dos botões da DE1-SoC, incluindo as seguintes funcionalidades:**
   - Pausa do jogo;
   - Retorno ao jogo;
   - Reinício do jogo;
   - Encerramento do jogo.   
8. **Pelo menos dois elementos passivos do jogo (não controlados pelos jogadores) deverão se mover de forma autônoma;**  
9. **Jogos de tabuleiro e/ou turno (como xadrez, damas, etc.), Tetris, jogo da forca e jogo da velha estão vetados. Além disso, cada grupo deve desenvolver um jogo único, diferente dos demais grupos.**  

---
## **Sumário**

<div id="sumario">
        <ul>
        <li><a href="#sft_ut"> Softwares Utilizados </a></li>
        <li><a href="#kit_placa"> Kit de Desenvolvimento DE1-SoC</a></li>
        <li><a href="#acl345"> Acelerômetro ADXL345</a></li>
        <li><a href="#lib_as"> Biblioteca Gráfica em Assembly<a><li>
        <li><a href="#gamerules"> Funcionamento do jogo</a></li>
        <li><a href="#test"> Testes</a></li>
        <li><a href="#makefile"> Como executar o Jogo</a></li>
        <li><a href="#resultado"> Resultados alcançados</a></li>
        <li><a href="#conclusion"> Conclusão</a></li>
        <li><a href="#aln"> Alunos</a></li>
        <li><a href="#crd"> Créditos</a></li>
        </ul>
</div>

---

<div id="sft_ut"></div>

## **Softwares Utilizados**

<code><img width="40px" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" title="C"/></code>
<code><img width="40px" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linux/linux-original.svg" title="LINUX"/></code>
<code><img width="40px" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/ubuntu/ubuntu-original.svg" title="UBUNTU"/></code>
<code><img width="40px" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/vscode/vscode-original.svg" title="VISUAL STUDIO CODE"/></code>

### GNU/Linux
Por fim, o kit de desenvolvimento DE1-SoC possui uma distribuição do Linux embarcado instalada, possibilitando a comunicação com o kit bem como a execução dos códigos criados através de conexão remota. Isso oferece uma gama de possibilidades para a elaboração do problema: a disposição dos diretórios do sistema e a possibilidade de compilar códigos na linguagem de programação requisitada de forma fácil com o compilador gcc embarcado no kit de desenvolvimento foram fundamentais.

### VS Code
O Visual Studio Code (VS Code) é uma ferramenta popular e altamente funcional utilizada em uma variedade de projetos de desenvolvimento de software. O uso dele foi necessário para agilizar o desenvolvimento, permitindo editar, depurar e gerenciar o código de maneira simplificada e eficaz.

### C
A linguagem C é amplamente utilizada em projetos devido à sua eficiência e versatilidade. Com sua sintaxe direta e controle próximo sobre o hardware, o C permite desenvolver programas robustos e rápidos, especialmente em sistemas embarcados, drivers de dispositivos e software de baixo nível. No contexto deste projeto, a utilização da linguagem C foi um dos requisitos necessarios a serem cumpridos.

### Assembly
O Assembly é uma linguagem de programação de baixo nível que está intimamente ligada à arquitetura do processador. Diferente de linguagens de alto nível como C ou Python, que são mais abstratas, o Assembly oferece um controle direto sobre o hardware, permitindo que o programador interaja com os registradores, memória e outras características do processador de forma explícita.

<div id="kit_placa"></div>

## **Kit de Desenvolvimento DE1-SoC**

O kit de desenvolvimento DE1-SoC é uma plataforma de prototipagem projetada para o desenvolvimento de sistemas embarcados e aplicações em FPGA (Field-Programmable Gate Array). Este kit é equipado com um FPGA da Altera (atualmente Intel), que permite a implementação de circuitos digitais personalizados. Abaixo tem os componentes principais dessa placa.

![alt text](image.png)

---
<div id="acl345"></div>

## **Acelerômetro ADXL345**
O ADXL345 é um acelerômetro de baixa potência e alta precisão, capaz de medir acelerações em três eixos (X, Y, Z). Ele é comumente utilizado em aplicações como medição de inclinação, detecção de movimento, controle de gestos, além de monitoramento de atividades em dispositivos portáteis. Seu uso se destaca pela facilidade de comunicação via interfaces I2C ou SPI e pela capacidade de operar em diferentes faixas de aceleração, como ±2g, ±4g, ±8g e ±16g.

### Uso do ADXL345 no Projeto

No projeto, o ADXL345 foi utilizado para capturar as acelerações nos eixos **X** e **Y**, que são convertidas em movimentos dentro de um sistema de controle. Dependendo da aceleração lida nos eixos, o programa interpreta direções e envia comandos como "esquerda", "direita", "para cima" ou "para baixo", proporcionando uma interface intuitiva baseada na inclinação.


### **Etapas do Uso**

1. **Configuração Inicial**  
   Antes de realizar a leitura dos dados, o acelerômetro foi configurado para operar com uma faixa de medição de **±2g**, adequada para capturar movimentos suaves de inclinação. A configuração foi realizada por meio da interface I2C, seguindo estas etapas:  
   - **Configuração dos Registradores:** Ajustes nos registradores de controle, como o **DATA_FORMAT** (para definir a faixa de ±2g) e o **POWER_CTL** (para ativar o dispositivo).  
   - **Definição da Taxa de Amostragem:** Configuramos o ADXL345 para operar a uma taxa de **100 Hz**, ajustada no registrador **BW_RATE**, garantindo uma resposta ágil aos movimentos.

2. **Leitura dos Dados**  
   - A leitura das acelerações é feita em dois bytes para cada eixo:  
     - O registrador **DATAX0** contém os 8 bits menos significativos e o **DATAX1** contém os 8 bits mais significativos para o eixo X.  
     - Da mesma forma, os registradores **DATAY0** e **DATAY1** são utilizados para o eixo Y.  
   - Os dois bytes de cada eixo são combinados para formar valores de 16 bits, que representam a aceleração bruta em cada direção.  
   - Essas leituras são feitas pelas funções `ler_aceleracao_x()` e `ler_aceleracao_y()`.

3. **Calibração**  
   - Para garantir leituras precisas, foi realizada uma calibração inicial. Durante esse processo, o sensor foi mantido em repouso e múltiplas leituras foram feitas para calcular offsets para os eixos X e Y.  
   - Esses offsets são subtraídos das leituras subsequentes para corrigir desvios naturais dos sensores.

4. **Conversão para Gravidade (G)**  
   - A aceleração bruta lida é convertida para a unidade de gravidade (**g**) usando a relação entre o valor bruto e a resolução configurada.  
   - Para a faixa de ±2g, a conversão foi realizada dividindo o valor bruto por **256**, obtendo a aceleração em "g". A resolução de **4 mg/LSB** foi utilizada para esse cálculo.

5. **Interpretação e Movimento**  
   - O sistema interpreta os valores de aceleração para determinar movimentos nos eixos:  
     - **Eixo X:** Valores positivos indicam movimento para a **direita**, enquanto valores negativos indicam movimento para a **esquerda**.  
     - **Eixo Y:** Valores positivos indicam movimento **para baixo**, enquanto valores negativos indicam movimento **para cima**.  
   - Um limiar foi definido (±50) para filtrar pequenos ruídos e considerar apenas movimentos significativos.  
   - Essa lógica é implementada na função `get_direcao_movimento()`, que retorna valores correspondentes às direções interpretadas.

---

### **Resultados**
A integração do ADXL345 no projeto permitiu capturar movimentos em dois eixos, **X** e **Y**, expandindo significativamente as possibilidades de controle. A leitura simultânea de ambos os eixos proporcionou maior precisão e flexibilidade, possibilitando comandos bidimensionais baseados na inclinação do dispositivo. Com isso, o sistema traduz movimentos físicos intuitivos em ações digitais rápidas e responsivas, garantindo uma interação fluida e eficiente.

---
<div id="lib_as"></div>

## **Biblioteca Gráfica em Assembly**
A biblioteca gráfica em Assembly apresentada foi desenvolvida para interagir diretamente com o hardware de um FPGA, utilizando a interface /dev/mem para mapear e acessar os registradores de controle do dispositivo. O objetivo principal dessa biblioteca é fornecer um conjunto de funções para manipulação da tela, incluindo a configuração de cores de fundo, desenho de formas geométricas, e controle de sprites. Cada função é cuidadosamente projetada para acessar registradores específicos de controle do FPGA, garantindo a manipulação eficiente dos recursos gráficos.

As funções são implementadas com chamadas diretas ao sistema operacional para abrir e mapear o arquivo especial /dev/mem, que permite o acesso direto à memória do dispositivo. A biblioteca utiliza operações de baixo nível para escrever e ler dados dos registradores, controlando as operações gráficas, como a cor de fundo, a renderização de polígonos e sprites, além de verificar o status do FIFO para garantir a sincronização das operações.

A seguir, será apresentada uma descrição detalhada das funções dessa biblioteca, explicando sua finalidade e a forma como manipulam o hardware para gerar as saídas gráficas desejadas. Essas funções são essenciais para qualquer aplicação gráfica que precise de um controle direto sobre o hardware, permitindo o desenvolvimento de interfaces personalizadas ou sistemas embarcados com recursos gráficos.

1. **Branch "open_devmem"**<br>
Esta função abre o dispositivo `/dev/mem`, que fornece acesso direto à memória do sistema para leitura e escrita. Ela salva os registradores na pilha para restaurá-los ao final. Em seguida, configura chamadas de sistema para leitura/escrita (syscall open) e usa `mmap2` para mapear uma região específica da memória, conectando-se ao endereço base da FPGA. O endereço mapeado é armazenado em `mapped_address`, e os registradores salvos são restaurados ao final da função.

2. **Branch "close_devmem"**<br>
Esta função desmonta e fecha o dispositivo `/dev/mem`. Ela libera a região de memória mapeada anteriormente usando `munmap`, para evitar vazamentos de memória. Em seguida, realiza o fechamento do dispositivo `/dev/mem`, restaurando os registradores da pilha antes de retornar.

3. **Branch "bg_block"**<br>
Esta função define uma posição específica na tela com uma determinada cor, criando um bloco no plano de fundo. A posição é passada como parâmetro (`r0`) e a cor como `r1`. A função manipula os registradores `wrreg`, `data_a` e `data_b` para escrever a cor do bloco na memória. Antes de habilitar a escrita, a função chama `check_fifo` para verificar se há espaço no FIFO, garantindo que a operação de escrita não cause um overflow.

4. **Branch "check_fifo"**<br>
Esta função verifica se há espaço no buffer FIFO (estrutura de dados do tipo fila) antes de cada escrita. Ela lê o registrador de status do FIFO em um loop até que haja espaço disponível, garantindo que novas escritas só ocorram quando o FIFO estiver apto.

5. **Branch "poligono"**<br>
Responsável por desenhar polígonos, como quadrados e triângulos, na tela com uma cor e posição específicas. Recebe como parâmetros a cor (`r0`), o tamanho (`r1`), o tipo de forma (`r2`), o registrador de destino (`r3`), a posição X (`r4`) e a posição Y (`r5`). Configura os registradores `DATAA` e `DATAB` para desenhar o polígono, ajustando os valores conforme o layout esperado pelo hardware. A função verifica o espaço no FIFO antes de escrever nos registradores da FPGA e, ao final, restaura os registradores e retorna ao ponto de execução original.

6. **Branch "bg_color"**<br>
Configura a cor de fundo da tela. A cor é passada como parâmetro em `r0`. A função acessa o endereço mapeado da FPGA, configurando o registrador `data_b` com a cor desejada. Em seguida, usa `data_a` para enviar o código de operação (opcode) para selecionar o registrador de fundo (background). A função chama `check_fifo` para garantir espaço no FIFO antes da escrita, definindo a cor de fundo com segurança.

7. **Branch "set_sprite"**<br>
Posiciona um sprite na tela e configura suas propriedades. Recebe parâmetros como posição X (`px`), posição Y (`py`), offset (identifica o sprite), `sp` (ativa/desativa o sprite) e o registrador de destino. Constrói os dados necessários para `DATAA` e `DATAB`, combinando o opcode, as coordenadas de posição e atributos do sprite. Assim como em outras operações, a escrita no hardware é controlada com `wrreg`.

8. **Branch "save_sprite"**<br>
Salva os dados de um sprite em uma região específica da memória. Recebe como parâmetros uma matriz de dados de sprite (`unsigned short sprite[400]`) e o endereço de destino. A função copia os dados da matriz para a memória mapeada no FPGA, garantindo que os gráficos do sprite sejam corretamente armazenados para uso posterior.

9. **Branch "acess_btn"**<br>
Acessa diretamente o estado dos botões conectados ao sistema. Lê o registrador correspondente e carrega o valor em `r0`, indicando o estado atual dos botões (pressionado ou não). Retorna o valor para que o software possa identificar os botões acionados.

10. **Branch "acess_display"**<br>
Lê o estado dos displays numéricos conectados ao sistema (seis displays no total). Acessa sequencialmente os registradores a partir do endereço base e carrega os valores no registrador `r0`. Isso permite que o software obtenha informações ou realize diagnósticos visuais com os displays.

**Mais alguns detalhes:**<br>
Todas essa funções foram feitas seguindo o passo a passo para a configuração dos barramentos DATA A e DATA B que está no TCC de @Gabriel B. Alves (Desenvolvimento de uma Arquitetura Baseada em
Sprites para criação de Jogos 2D em Ambientes
Reconfiguráveis utilizando dispositivos FPGA). Disponível em: https://drive.google.com/file/d/1MlIlpB9TSnoPGEMkocr36EH9-CFz8psO/view


---
<div id="gamerules"></div>

## **Funcionamento do Jogo - Pacman vs Ghost**

1. **Estrutura Básica do Jogo**:<br>
O jogo é baseado em uma competição entre dois jogadores, onde um controla o **Pacman** e o outro controla o **Ghost**. O objetivo do **Pacman** é comer 3 maçãs para ganhar, enquanto o **Ghost** tenta impedir o Pacman de completar a missão, fazendo com que ele perca suas 3 vidas. O **Pacman** é controlado pelo **acelerômetro**, enquanto o **Ghost** é controlado pelo **mouse**. O jogo termina quando o **Pacman** come todas as maçãs ou perde todas as suas vidas.

2. **Movimentação dos Personagens**:<br>
   - O **Pacman** se move livremente em todas as direções (eixo X e Y), controlado pelo **acelerômetro**. O movimento do **Pacman** é determinado pela inclinação do dispositivo: quando o jogador inclina para a direita, o Pacman se move para a direita; quando inclina para a esquerda, ele se move para a esquerda. Além disso, o Pacman possui **variações de sprites** que mudam conforme a direção do movimento, proporcionando uma resposta visual clara e dinâmica para cada movimentação (direita, esquerda, cima ou baixo).
   - O **Ghost** é controlado pelo **mouse**, seguindo a posição do ponteiro na tela. O movimento do **Ghost** é diretamente associado ao movimento do mouse do jogador.

3. **Objetivos e Mecânica de Jogo**:<br>
   - O objetivo do **Pacman** é coletar 3 maçãs espalhadas pelo cenário. Cada maçã que o **Pacman** come aumenta sua pontuação, e ao pegar todas as 3 maçãs, o **Pacman** vence o jogo.
   - O **Ghost** tem a missão de acabar com as 3 vidas do **Pacman**. Se o **Ghost** encostar no **Pacman**, ele perde uma vida. O jogo termina se o **Pacman** perder todas as suas vidas ou completar sua missão de pegar as 3 maçãs.

4. **Itens Passivos - Maçã Envenenada e Uva**:<br>
   Além das maçãs normais, o cenário inclui dois itens passivos que podem afetar diretamente a jogabilidade:
   - **Maçã Envenenada**: Esta maçã tem a capacidade de **retirar uma vida do Pacman** se ele colidir com ela. Ela não se move aleatoriamente, mas é posicionada em uma **área específica** do labirinto, definida pelo design do jogo. Ao colidir com a maçã envenenada, o **Pacman** perde uma vida, e um efeito visual (como uma mudança de cor ou piscar) é gerado para alertar o jogador.
   - **Uva**: A **uva** é um item benéfico que **recupera uma vida do Pacman** quando ele colide com ela. Semelhante à maçã envenenada, a **uva** não é gerada aleatoriamente, mas sim posicionada em uma **área específica** do labirinto. Quando o **Pacman** colide com a uva, ele recupera uma vida, oferecendo uma chance extra de continuar no jogo.

5. **Sprites e Itens**:<br>
Foram adicionados **sprites novos** para representar os personagens e itens do jogo:
   - **Pacman**: Um sprite circular amarelo que se move conforme a inclinação do acelerômetro. O sprite do **Pacman** muda dependendo da direção do movimento (direita, esquerda, cima, baixo).
   - **Ghost**: Um sprite colorido que se move de acordo com o movimento do mouse.
   - **Maçãs**: Sprites representando maçãs, que aparecem em posições específicas do labirinto para serem coletadas pelo **Pacman**.
   - **Maçã Envenenada**: Um sprite distinto de maçã com uma aparência diferente (talvez com cores como verde ou roxo), que representa o item que pode tirar uma vida do **Pacman**.
   - **Uva**: Um sprite de uva, representando o item que recupera vidas para o **Pacman**.

6. **Posicionamento de Itens**:<br>
A **maçã envenenada** e a **uva** são colocadas em **regiões específicas** do labirinto, que são definidas no design do jogo. Essas regiões são zonas estratégicas no mapa, que o jogador precisa acessar para pegar os itens. A função **GenerateSpecialItem** é responsável por posicionar a **maçã envenenada** e a **uva** nessas regiões pré-definidas do labirinto, garantindo que eles não apareçam no mesmo local do **Pacman** ou do **Ghost**.

7. **Detecção de Colisões**:<br>
A função **CheckCollision** verifica se o **Pacman** e o **Ghost** estão na mesma posição. Se o **Pacman** colidir com o **Ghost**, ele perde uma vida. Além disso, a função **CheckApple** verifica se o **Pacman** colidiu com uma maçã normal ou com a maçã envenenada. Se isso acontecer, a maçã é removida do cenário e a pontuação do **Pacman** é aumentada. A função **CheckSpecialItem** verifica se o **Pacman** colidiu com a **uva** ou a **maçã envenenada**, aplicando as regras de aumento ou perda de vidas conforme necessário.

8. **Controle de Vidas e Game Over**:<br>
O **Pacman** começa com 3 vidas. Sempre que o **Ghost** colide com o **Pacman**, ele perde uma vida. Se o **Pacman** colidir com a **maçã envenenada**, ele também perderá uma vida. Quando o **Pacman** perde todas as suas vidas, o jogo exibe a mensagem de **Game Over** e termina. Caso o **Pacman** consiga comer as 3 maçãs ou recupere vidas com a **uva**, ele vence o jogo.

9. **Desempenho e Atualização de Tela**:<br>
A tela é atualizada continuamente com base nos movimentos dos personagens. O **Pacman** é redesenhado sempre que o dispositivo é inclinado, e o **Ghost** é reposicionado com base no movimento do mouse. As maçãs, a **maçã envenenada** e a **uva** também são atualizadas para refletir o progresso do jogo.

10. **Pausar e Continuar o Jogo**:<br>
O jogador pode pausar o jogo a qualquer momento pressionando o botão de pausa. Durante a pausa, o jogo fica suspenso, e uma mensagem de pausa é exibida na tela. O jogo pode ser retomado quando o jogador pressionar novamente o botão de pausa.

11. **Gerenciamento de Memória e Recursos**:<br>
A memória dos **sprites** dos personagens e itens é alocada dinamicamente para garantir que os gráficos sejam renderizados corretamente durante o jogo. A função **DeleteSprite** libera a memória dos sprites quando não forem mais necessários, evitando vazamento de memória.

---
<div id="test"></div>

## **Testes** 
Alguns gif's abaixo descreve os testes feito durante o desenvolvimento.

### Interação com elementos
![Pac](midias/video1.gif)
---
### Reinício
![Reinício](midias/video2.gif)
---
### Maçã Envenenada 
![maca](midias/video3.gif)
---

<div id="makefile"></div> 

## **Como executar o Jogo**

### Makefile
Este Makefile fornece uma maneira simples e eficaz de compilar e executar seu projeto de Tetris, garantindo que as dependências necessárias sejam incluídas e que o ambiente esteja limpo antes da execução. A utilização de sudo indica que a aplicação pode interagir diretamente com o hardware, o que é comum em projetos que envolvem FPGAs e sistemas embarcados.

![Makefile](imagens/makefile.png)

1. all:<br>
Este é o alvo padrão do Makefile. Quando você executa make sem especificar um alvo, o Makefile executa o alvo all. Neste caso, não há dependências definidas, então ele sempre executará as instruções subsequentes.

2. clear:<br>
Este comando é utilizado para limpar a tela do terminal antes de compilar e executar o programa. Isso ajuda a manter a saída do terminal organizada e facilita a visualização dos resultados.

3. gcc -w -Wall mouse.c main.c -g -o main mapmem.s -lintelfpgaup -lpthread -std=c99<br>
Este comando utiliza o GCC (GNU Compiler Collection) para compilar o código-fonte do jogo:<br>

•gcc: Chama o compilador GCC (GNU Compiler Collection) para compilar o código.<br>
•-w: Suprime todos os warnings (mensagens de alerta) do compilador.<br>
•-Wall: Habilita a exibição de todos os warnings do compilador, que pode ajudar a identificar problemas potenciais no código.<br>
main.c: Arquivo de código-fonte C que será compilado.<br>
•-g: Adiciona informações de depuração (útil para debugging) ao binário gerado.<br>
•-o main: Especifica o nome do arquivo de saída, que será o executável chamado main.<br>
mapmem.s: Um arquivo de código Assembly que também será incluído no processo de compilação.<br>
•-lintelfpgaup: Linka a biblioteca libintelfpgaup, provavelmente uma biblioteca para interagir com um hardware específico.<br>
•-lpthread: Linka a biblioteca pthread, necessária para programação com threads em C.<br>
•-std=c99: Define que o código C será compilado de acordo com o padrão C99
4. sudo ./main: <br>
Após a compilação bem-sucedida, este comando executa o programa resultante (tetris). O uso de sudo sugere que o programa pode precisar de permissões elevadas para acessar recursos específicos do sistema ou hardware, como acessar o FPGA no kit DE1-SoC.

### Execução do Makefile

Ao implementar o makifile, facilitou-se ativamente na execução do jogo. Segue um passo a passo de como é feito:
1. O usuário deve conectar ao kit de desenvolvimento De1-Soc;
2. Entrar no diretório onde está o projeto;
3. Escrever no terminal a palavra chave "make";
4. Digitar a senha do usuário do pc;

Desse modo, o projeto vai iniciar a execução.

---
<div id="resultado"></div> 

## **Resultados Alcançados**
Após a implementação do projeto, todos os resultados estabelecidos foram alcançados com sucesso, proporcionando aprendizado e satisfação para os desenvolvedores. Além disso, é possível, durante a execução, pausar o jogo a qualquer momento e, posteriormente, retomar a jogabilidade após o comando de retomada. A funcionalidade de pausa e retomada foi implementada para garantir que o jogador tenha controle total sobre o fluxo do jogo, criando uma experiência mais interativa.

---
<div id="conclusion"></div> 

## **Conclusão**
Em suma, o projeto foi executado com sucesso. Apesar dos desafios iniciais relacionados à integração do mouse, o jogo demonstrou ser funcional e atendeu aos requisitos estabelecidos. Além disso, essa experiência foi crucial para aprofundar o entendimento sobre o kit de desenvolvimento, GNU/Linux embarcado e a interação entre hardware e software. Esse conhecimento adquirido certamente enriquecerá futuros projetos a serem desenvolvidos no kit DE1-SoC.

---
<div id="aln"></div>

## **Alunos(as)**

<table align='center'>
<tr> 
  <td align="center">
    <a href="https://github.com/LuisMarioRC">
      <img src="https://avatars.githubusercontent.com/u/142133059?v=4" width="100px;" alt=""/>
    </a>
    <br /><sub><b><a href="https://github.com/LuisMarioRC">Luis Mario</a></b></sub><br />👨‍💻
  </td>
  <td align="center">
    <a href="https://github.com/laizagordiano">
      <img src="https://avatars.githubusercontent.com/u/132793645?v=4" width="100px;" alt=""/>
    </a>
    <br /><sub><b><a href="https://github.com/laizagordiano">Laiza Gordiano</a></b></sub><br />👨‍💻
  </td>
  <td align="center">
    <a href="https://github.com/Ltiicia">
      <img src="https://avatars.githubusercontent.com/u/124190885?v=44" width="100px;" alt=""/>
    </a>
    <br /><sub><b><a href="https://github.com/Ltiicia">Letícia Gonçalves</a></b></sub><br />👨‍💻
  </td>
</tr>

</table>

---
<div id="crd"></div>

### **Créditos**
Este projeto foi desenvolvido na disciplina TEC499 - MI de Sistemas Digitais (Semestre 2024.2) do curso de Engenharia de Computação da Universidade Estadual de Feira de Santana - UEFS.

---

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=f5d442&height=175&section=footer"/>
