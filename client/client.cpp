
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "oo_client.hpp"
#include <chrono>
#include <thread>
#include <ncurses.h>


using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main() {

  Corpo *c1 = new Corpo(0, 0, 0, 0);
  Corpo *c2 = new Corpo(0, 0, 0, 0);
  Corpo *c3 = new Corpo(0, 0, 0, 0);
  Corpo *c4 = new Corpo(0, 0, 0, 0);
  Corpo *c5 = new Corpo(0, 0, 0, 0);
  Corpo *c6 = new Corpo(0, 0, 0, 0);
  Corpo *c7 = new Corpo(0, 0, 0, 0);
  Corpo *c8 = new Corpo(0, 0, 0, 0);
  Corpo *c9 = new Corpo(0, 0, 0, 0);
  Corpo *c10 = new Corpo(0, 0, 0, 0);
  /*
  Corpo *c11 = new Corpo(0, 0, 0, 0);
  Corpo *c12 = new Corpo(0, 0, 0, 0);
  Corpo *c13 = new Corpo(0, 0, 0, 0);
  Corpo *c14 = new Corpo(0, 0, 0, 0);
  Corpo *c15 = new Corpo(0, 0, 0, 0);
  */

  ListaDeCorpos *l = new ListaDeCorpos();

  l->add_corpo(c1);
  l->add_corpo(c2);
  l->add_corpo(c3);
  l->add_corpo(c4);
  l->add_corpo(c5);
  l->add_corpo(c6);
  l->add_corpo(c7);
  l->add_corpo(c8);
  l->add_corpo(c9);
  l->add_corpo(c10);
  /*
  l->add_corpo(c11);
  l->add_corpo(c12);
  l->add_corpo(c13);
  l->add_corpo(c14);
  l->add_corpo(c15);
  */

  Cliente *cliente = new Cliente();
  if (cliente->initClient()) {
    printf("Nao foi possivel conectar ao servidor\n");
    return 1;
  }

  Mapa* mapa = new Mapa();

  Tela *tela = new Tela(l, 20, 20, 20, 20, mapa);
  tela->init();

  /*
  Teclado *teclado = new Teclado();
  teclado->init();
  */

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;
  uint64_t ts0, ts1;
  char c;

  T = get_now_ms();
  t1 = T;
  int tela_pequena = 0; // Marca se a tela eh menor que o suportado pelo jogo
  int ganhou = 0;

  // Criando thread para receber a lista de corpos do servidor
  std::thread newthread(threadCorpos, cliente, l);
  (cliente->corpos_thread).swap(newthread);
  // Criando thread para enviar comandos para o servidor
  std::thread newthread2(threadEnviaComandos, cliente);
  (cliente->kb_thread).swap(newthread2);

  while (1) {
      
    if(!cliente->getRodando()) {
      break;
    }


    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }


  //teclado->stop();
  tela->stop();

  if(cliente->getGanhou() == 'g') {
    printf("Parabéns, você ganhou!\n");
  } else if(cliente->getGanhou() == 'p') {
    printf("Perdeu!\n");
  } else {
    printf("Você saiu do jogo.\n");
  }
  cliente->endClient();



  return 0;
}
