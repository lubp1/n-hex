#ifndef OO_CLIENT_HPP
#define OO_CLIENT_HPP

#define  MAX_Y 200
#define  MIN_Y 2
#define  MAX_X 49
#define  MIN_X 2

#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../headers/json.hpp"


// Funcao que roda em outra thread para ler o teclado
void threadTeclado (char *keybuffer, int *control);

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


// Classe de corpos. Cada corpo possui sua velocidade e posicao nos eixos X e Y, alem de atributos de sua rotacao e cor
class Corpo {
  private:
  float velY;
  float velX;
  float posY;
  float posX;
  int orb; // Em qual nivel de orbita em torno do hexagono o corpo esta, entre 0 (nao em orbita) e de 1 a 4 (da orbita mais interna a mais externa, respectivamente)
  int rot; // O sentido da rotacao do corpo
  int pos_orb; // Posicao do corpo no vetor de orbitas
  int cor; // Cor do corpo, 0 eh um corpo neutro, 1 da cor do usuario e 2 inimigo
  int jogador; // 0 se nao eh um jogador, 1 se eh um jogador


  public:
  Corpo();
  Corpo(float vX, float vY, float posX, float posY);
  void update(float new_velX, float new_velY, float new_posX, float new_posY);
  float get_velY();
  float get_velX();
  float get_posY();
  float get_posX();
  void set_orb(int orb);
  int get_orb();
  void set_rot(int rot);
  int get_rot();
  void set_cor(int cor);
  int get_cor();
  void set_pos_orb(int pos_orb);
  int get_pos_orb();
  void set_jogador(int jogador);
  int get_jogador();
  std::string serialize();
  void unserialize(std::string corpo_serializado);
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
    std::string serialize();
    void unserialize(std::string lista_serializado);
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


class Cliente {
    private:
    int socket_fd;
    int connection_fd;
    struct sockaddr_in myself;
    char input_buffer;
    int rodando;
    int id;
    ListaDeCorpos* l;


  public:
    struct sockaddr_in client;
    socklen_t client_size;
    std::thread corpos_thread, kb_thread;

    Cliente();
    int initClient();
    void endClient();
    void setBuffer(char buffer);
    char getBuffer();
    void setRodando(int rodando);
    int  getRodando();
    void setConnection(int connection);
    int  getConnection();
    void setSocket(int socket);
    int  getSocket();
    void setClientSize(socklen_t client_size);
    socklen_t getClientSize();
};

void threadCorpos(Cliente* client, ListaDeCorpos* l);
void threadEnviaComandos(Cliente* client, Teclado* teclado);


#endif
