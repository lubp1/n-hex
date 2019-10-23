#ifndef OO_CLIENT_HPP
#define OO_CLIENT_HPP


#include <thread>


// Funcao que roda em outra thread para ler o teclado
void threadfun (char *keybuffer, int *control);

// Classe que le do teclado
class Teclado {
  private:
    char ultima_captura;
    int rodando;

    std::thread kb_thread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
};


#endif
