
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


  Teclado *teclado = new Teclado();
  teclado->init();

  Mapa* mapa = new Mapa();
  ListaDeCorpos *l = new ListaDeCorpos();

  Tela *tela = new Tela(l, 20, 20, 20, 20, mapa);
  tela->init();

  Cliente *cliente = new Cliente();
  cliente->initClient();

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

  while (1) {
      
    if(c == ' ' || c == 'q'){
      printf("TESTE\n");
      /* Agora, meu socket funciona como um descritor de arquivo usual */
      send(cliente->getSocket(), &c, 1, 0);
      send(cliente->getSocket(), 0, 1, 0);
      if(c == 'q') {
        break;
      }
    }

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }

  cliente->endClient();
  tela->stop();


  if(ganhou) {
    printf("Você ganhou o jogo em %lu segundos\n", (get_now_ms()-T)/1000);
  }
  else if(!tela_pequena) {
    printf("Sobreviveu por %lu segundos\n", (get_now_ms()-T)/1000);
  }

  return 0;
  teclado->stop();

  return 0;
}
