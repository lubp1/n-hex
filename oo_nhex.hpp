
#ifndef OO_NHEX_HPP
#define OO_NHEX_HPP

#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "oo_nhex.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  MAX_Y 200
#define  MIN_Y 2
#define  MAX_X 49
#define  MIN_X 2


// Classe de corpos. Cada corpo possui sua velocidade e posicao nos eixos X e Y, alem de atributos de sua rotacao e cor
class Corpo {
  private:
  float velY;
  float velX;
  float posY;
  float posX;
  char orb; // Em qual nivel de orbita em torno do hexagono o corpo esta, entre 0 (nao em orbita) e de 1 a 4 (da orbita mais interna a mais externa, respectivamente)
  char rot; // O sentido da rotacao do corpo
  int pos_orb; // Posicao do corpo no vetor de orbitas
  char cor; // Cor do corpo, 0 eh um corpo neutro, 1 da cor do usuario e 2 inimigo

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
  void set_pos_orb(int pos_orb);
  int get_pos_orb();
};

// Lista com todos os corpos do programa
class ListaDeCorpos {
 private:
    std::vector<Corpo*> *corpos;

  public:
    ListaDeCorpos();
    void copy(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    std::vector<Corpo*> *get_corpos();
};

// Mapa do jogo, com a posicao dos hexagonos e suas orbitas
class Mapa {
  private:
  // Listas das posicoes dos centros dos hexagonos, em X e Y
  int* listaX_Hex;
  int* listaY_Hex;

  // Listas das posicoes em X e Y das bordas, em relacao ao centro do hexagono (referencia do sistema)
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

// Classe que controla a atualizacao das posicoes dos corpos, e as interacoes entre eles
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

// Classe que imprime o mapa e corpos na tela
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
    int update();
    int getRows(void);
    int getCols(void);
};


class Servidor {
  private:
    int socket_fd;
    int connection_fd;
    struct sockaddr_in myself;
    char input_buffer;
    int rodando;


  public:
    struct sockaddr_in client;
    socklen_t client_size;
    std::thread kb_thread;

    Servidor();
    void initServer();
    void endServer();
    void setBuffer(char buffer);
    char getBuffer();
    void setRodando(int rodando);
    int getRodando();
    void setConnection(int connection);
    int getConnection();
    void setSocket(int socket);
    int getSocket();
    void setClientSize(socklen_t client_size);
    socklen_t getClientSize();
};

void threadfun (Servidor* server);

#endif
