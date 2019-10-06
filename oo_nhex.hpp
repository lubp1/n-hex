
#ifndef OO_NHEX_HPP
#define OO_NHEX_HPP

#include <thread>
#define  MAX_Y 200
#define  MIN_Y 2
#define  MAX_X 49
#define  MIN_X 2


class Corpo {
  private:
  float velY;
  float velX;
  float posY;
  float posX;
  char orb;
  char rot;
  int pos_orb;
  char cor;

  public:
  Corpo();
  Corpo(float vX, float vY, float posX, float posY);
  void update(float new_velX, float new_velY, float new_posX, float new_posY);
  float get_velY();
  float get_velX();
  float get_posY();
  float get_posX();
  void set_orb(char orb);
  char get_orb();
  void set_rot(char rot);
  char get_rot();
  void set_cor(char cor);
  char get_cor();
  int get_pos_orb();
  void set_pos_orb(int pos_orb);
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

class Mapa {
  private:
  int* listaX_Hex;
  int* listaY_Hex;

  int *orb1X, *orb1Y;
  int *orb2X, *orb2Y;
  int *orb3X, *orb3Y;
  int *orb4X, *orb4Y;

  public:
  Mapa();
  int buscaHex(int posX, int posY);
  char orbita(int x, int y, int hex, Corpo *c);
  char rotacao(int x, int y, int vx, int vy, char orb, int hex);
  int* get_listaX();
  int* get_listaY();
  int* get_orb1X();
  int* get_orb2X();
  int* get_orb3X();
  int* get_orb4X();
  int* get_orb1Y();
  int* get_orb2Y();
  int* get_orb3Y();
  int* get_orb4Y();
  ~Mapa();
};

class Fisica {
  private:
    ListaDeCorpos *lista;
    Mapa* mapa;


  public:
    Fisica(ListaDeCorpos *ldc, Mapa* mapa);
    void add_corpo(Corpo *c);
    void impulso();
    int update(float deltaT, int tamTela);
};

class Tela {
  private:
    ListaDeCorpos *lista, *lista_anterior;
    int maxI, maxJ;
    float maxX, maxY;
    int row, col;
    Mapa* mapa;


  public:
    Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY, Mapa* mapa);
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
