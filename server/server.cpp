#include "oo_server.hpp"

using namespace std::chrono;
using json = nlohmann::json;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main (){
  // Atualiza seed
  srand(get_now_ms());

  Mapa* mapa = new Mapa();

  Corpo *c1 = new Corpo(0, -10, 15, 100); //nave
  Corpo *c2 = new Corpo(10, -10, rand()%45, rand()%190);
  Corpo *c3 = new Corpo(10, 10, rand()%45, rand()%190);
  Corpo *c4 = new Corpo(10, 0, rand()%45, rand()%190);
  Corpo *c5 = new Corpo(-10, 10, rand()%45, rand()%190);
  /*
    Corpo *c6 = new Corpo(10, 10, rand()%45, rand()%190);
    Corpo *c7 = new Corpo(10, 10, rand()%45, rand()%190);
    Corpo *c8 = new Corpo(10, 10, rand()%45, rand()%190);
    Corpo *c9 = new Corpo(10, 10, rand()%45, rand()%190);
    Corpo *c10 = new Corpo(10, 10, rand()%45, rand()%190);
    Corpo *c11 = new Corpo(10, 10, rand()%45, rand()%190);
    Corpo *c12 = new Corpo(-10, 10, rand()%45, rand()%190);
    Corpo *c13 = new Corpo(10, -10, rand()%45, rand()%190);
    Corpo *c14 = new Corpo(-10, 10, rand()%45, rand()%190);
    Corpo *c15 = new Corpo(-10, -10, rand()%45, rand()%190);
  */

  // O jogador comeca orbitando
  c1->set_orb(4);
  c1->set_rot('a');

  

  ListaDeCorpos *l = new ListaDeCorpos();

  l->add_corpo(c1);
  l->add_corpo(c2);
  l->add_corpo(c3);
  l->add_corpo(c4);
  l->add_corpo(c5);
  /*
    l->add_corpo(c6);
    l->add_corpo(c7);
    l->add_corpo(c8);
    l->add_corpo(c9);
    l->add_corpo(c10);
    l->add_corpo(c11);
    l->add_corpo(c12);
    l->add_corpo(c13);
    l->add_corpo(c14);
    l->add_corpo(c15);
  */


  Fisica *f = new Fisica(l, mapa);

  /*
  Tela *tela = new Tela(l, 20, 20, 20, 20, mapa);
  tela->init();
  */

  Servidor *servidor = new Servidor();
  servidor->initServer();
  // Thread para receber os comandos dos usuarios
  for(int i = 0; i< MAX_PLAYERS; i++) {
    std::thread newthread(threadServidor, servidor, i);
    (servidor->kb_thread[i]).swap(newthread);
  }
  // Thread que espera a conexão de usuarios
  std::thread newthread2(threadEsperaServidor, servidor);
  (servidor->wait_thread).swap(newthread2);
  // Thread que envia o model para os usuarios
  std::thread newthread3(threadEnviaCorpos, servidor, l);
  (servidor->model_thread).swap(newthread3);


  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;
  uint64_t ts0, ts1;

  int i = 0;





  while(1) {
    for (int i = 0; i < servidor->getJogadores(); i++) {
      char c = servidor->getBuffer(i);
      if (c=='s') {
        servidor->setJogadorVivo(2,i);
        printf("Jogador %d está pronto.\n", i);
      }
    }
    int acumulador = 0;
    for(int i = 0; i < MAX_PLAYERS; i++) {
      acumulador += servidor->getJogadorVivo(i);
    }
    if (acumulador == 2*servidor->getJogadores() && servidor->getJogadores() > 0) {
      for(int i = 0; i < MAX_PLAYERS; i++) {
        if(servidor->getJogadorVivo(i)) {
          // Colorindo jogadores e marcando como vivos
          servidor->setJogadorVivo(1, i);
          l->get_corpos()->at(i)->set_cor(i+1);
          l->get_corpos()->at(i)->set_jogador(1);
        }
      } 
      break;
    }
  }
  (servidor->wait_thread).join();

  T = get_now_ms();
  t1 = T;


  while (1) {


    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;


    // Atualiza modelo
    if (f->update(deltaT, 50)) {
      break;
    }

    /*
    // Atualiza tela
    tela_pequena = tela->update();
    */


    // Lê o teclado
    for (int i = 0; i < servidor->getJogadores(); i++) {
      char c = servidor->getBuffer(i);
      if (c==' ') {
        f->impulso(i);
      } else if (c=='q' || c=='Q') {
        printf("Recebido comando para terminar.\n");
        goto fim;
      } else if (!servidor->getRodando()) {
        printf("Todos os clientes desconectados.\n");
        goto fim;
      }
    }


    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }
fim:
  servidor->endServer();
  //tela->stop();
  printf("Servidor fechado.\n");
  return 0;
}
