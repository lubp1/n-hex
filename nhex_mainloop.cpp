#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "01-playback.hpp"

#include "oo_nhex.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main (){
  Audio::Sample *asample;
  asample = new Audio::Sample();
  asample->load("assets/blip.dat");

  Audio::Player *player;
  player = new Audio::Player();
  player->init();

  Corpo *c1 = new Corpo(10, 10, 10, 10);
  Corpo *c2 = new Corpo(10, 10, 25, 20);

  ListaDeCorpos *l = new ListaDeCorpos();
  l->add_corpo(c1);
  l->add_corpo(c2);


  Fisica *f = new Fisica(l, 15, 3);

  Tela *tela = new Tela(l, 20, 20, 20, 20);
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
  player->play(asample);

  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    // Atualiza modelo
    f->update(deltaT, tela->getRows());

    // Atualiza tela
    tela->update();

    // Lê o teclado
    char c = teclado->getchar();

    if (c=='w') {
       // Reproduz som
      asample->set_position(0);
    //  f->choque(-15);
    }
    else if(c == 's'){
       // Reproduz som
      asample->set_position(0);
     // f->choque(15);
    }
    else if (c=='q') {
      break;
    }

    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }

  player->stop();       // Termina o audio
  tela->stop();
  teclado->stop();
  return 0;
}
