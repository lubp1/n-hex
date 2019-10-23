#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "oo_nhex.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main (){
  Mapa* mapa = new Mapa();

  Corpo *c1 = new Corpo(0, -10, 15, 100); //nave
  Corpo *c2 = new Corpo(0, 10, rand()%45, rand()%190);
  Corpo *c3 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c4 = new Corpo(10, 0, rand()%45, rand()%190);
  Corpo *c5 = new Corpo(-10, 10, rand()%45, rand()%190);
  Corpo *c6 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c7 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c8 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c9 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c10 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c11 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c12 = new Corpo(-10, 10, rand()%45, rand()%190);
  Corpo *c13 = new Corpo(10, -10, rand()%45, rand()%190);
  Corpo *c14 = new Corpo(-10, 10, rand()%45, rand()%190);

  // O jogador comeca orbitando
  c1->set_orb(4);
  c1->set_rot('a');

  // Bolas inimigas
  c2->set_cor(2);
  c3->set_cor(2);
  c4->set_cor(2);
  c5->set_cor(2);
  c6->set_cor(2);
  c7->set_cor(2);
  c8->set_cor(2);
  c9->set_cor(2);
  c10->set_cor(2);
  c11->set_cor(2);

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
  l->add_corpo(c11);
  l->add_corpo(c12);
  l->add_corpo(c13);
  l->add_corpo(c14);

  Fisica *f = new Fisica(l, mapa);


  Tela *tela = new Tela(l, 20, 20, 20, 20, mapa);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;
  uint64_t ts0, ts1;

  int i = 0;

  T = get_now_ms();
  t1 = T;
  int tela_pequena = 0; // Marca se a tela eh menor que o suportado pelo jogo
  int ganhou = 0;

  while (1) {


    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;


    // Atualiza modelo
    if (f->update(deltaT, tela->getRows())) {
      break;
    }

    // Atualiza tela
    tela_pequena = tela->update();

    // Lê o teclado
    char c = teclado->getchar();

    if (c==' ') {
      f->impulso();
    }
    else if (c=='q' || c=='Q') {
      break;
    }

    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;
    if (c12->get_cor() == 1 || c13->get_cor() == 1 || c14->get_cor() == 1){
      ganhou = 1;
      break;
    }

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }
  tela->stop();

  if(ganhou) {
    printf("Você ganhou o jogo em %lu segundos\n", (get_now_ms()-T)/1000);
  }
  else if(!tela_pequena) {
    printf("Sobreviveu por %lu segundos\n", (get_now_ms()-T)/1000);
  }

  return 0;
}
