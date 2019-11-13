# n-hex
Lucas Bertoloto e Lucas Morandi - RAs 201798 e 202031

Laboratório 6:
  A funcionalidade de teclado foi removida do jogo principal, e foi adicionado outro programa para o cliente. Agora, o cliente que usa o teclado, que é enviado ao servidor para que ele execute o resto do jogo.

  Como rodar:
  -no diretorio raiz, executar:
    make
    make cliente
  -Rodar o servidor com "./nhex" e o cliente com "./cliente"


O jogo é baseado no jogo 12orbs, disponivel em: https://play.google.com/store/apps/details?id=com.romanuhlig.twelveorbitsfree&hl=pt_BR.

Na etapa single player, o objetivo do jogo é capturar uma das bolas brancas, desviando das bolas de outras cores. O jogador controla o asterisco azul, e, ao encostar em uma bola branca, captura ela. Ao apertar espaço dentro de um hexagono, o jogador comeca a orbitar dentro dele, e, ao apertar fora do hexagono, ele dá um impulso para frente, pulando o que está no seu caminho. Se o jogador encostar em uma bola verde, ele perde o jogo.

Para rodar o jogo, basta rodar "make" no diretorio do programa, e depois "./nhex". O projeto está disponível em https://github.com/lubp1/n-hex

A programação foi baseada em MVC, sendo as classes Corpo, ListaDeCorpos e Mapa o Model, as classes Fisica e Teclado o Control, e a classe Tela o View.
