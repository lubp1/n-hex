
#ifndef OO_NHEX_HPP
#define OO_NHEX_HPP

#include <thread>
#define  MAX_Y 200
#define  MIN_Y 2
#define  MAX_X 50
#define  MIN_X 2

class Corpo {
  private:
  float velY;
  float velX;
  float posY;
  float posX;
  char orb;

  public:
  Corpo(float vX, float vY, float posX, float posY);
  void update(float new_velX, float new_velY, float new_posX, float new_posY);
  float get_velY();
  float get_velX();
  float get_posY();
  float get_posX();
  void set_orb(char orb);
  char get_orb();
};

class ListaDeCorpos {
 private:
    std::vector<Corpo*> *corpos;

  public:
    ListaDeCorpos();
    void hard_copy(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    std::vector<Corpo*> *get_corpos();
};

class Fisica {
  private:
    ListaDeCorpos *lista;
    int k, b;

  public:
    Fisica(ListaDeCorpos *ldc, int k, int b);
    void add_corpo(Corpo *c);
    void choque(int impulso);
    void update(float deltaT, int tamTela);
};

class Tela {
  private:
    ListaDeCorpos *lista, *lista_anterior;
    int maxI, maxJ;
    float maxX, maxY;
    int row, col;


  public:
    Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
    int getRows(void);
    int getCols(void);
};

void threadfun (char *keybuffer, int *control);

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
