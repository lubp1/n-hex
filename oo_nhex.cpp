#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "01-playback.hpp"
#include "oo_nhex.hpp"
#include <cstdlib>
#include <ncurses.h>

using namespace std::chrono;

Corpo::Corpo(float vX, float vY, float posX, float posY) {
  this->velX = vX;
  this->velY = vY;
  this->posX = posX;
  this->posY = posY;
}

void Corpo::update(float new_velX, float new_velY, float new_posX, float new_posY) {
  this->velX = new_velX;
  this->velY = new_velY;
  this->posX = new_posX;
  this->posY = new_posY;
}

float Corpo::get_velY() {
  return this->velY;
}

float Corpo::get_velX() {
  return this->velX;
}

float Corpo::get_posY() {
  return this->posY;
}

float Corpo::get_posX() {
  return this->posX;
}

void Corpo::set_orb(char orb){
  this->orb = orb;
}

char Corpo::get_orb(){
  return this->orb;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo((*corpos)[k]->get_velX(), (*corpos)[k]->get_velY(), (*corpos)[k]->get_posX(), (*corpos)[k]->get_posY());
    this->add_corpo(c);
  }

}


void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}

Fisica::Fisica(ListaDeCorpos *ldc, int k, int b) {
  this->lista = ldc;
  this->k = k;
  this->b = b;
}

void Fisica::update(float deltaT, int tamTela) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    float new_velX, new_velY, new_posX, new_posY;

    if(((*c)[i])->get_posX() <= MIN_X || ((*c)[i])->get_posX() >= MAX_X){
      new_velX =  (-1)*((*c)[i])->get_velX();
    }
    else{
      new_velX = ((*c)[i])->get_velX();
    }
    if(((*c)[i])->get_posY() <= MIN_Y || ((*c)[i])->get_posY() >= MAX_Y){
      new_velY =  (-1)*((*c)[i])->get_velY();
    }
    else{
      new_velY = ((*c)[i])->get_velY();
    }

    new_posX = (*c)[i]->get_posX() + (int)deltaT * new_velX/1000;
    new_posY = (*c)[i]->get_posY() + (int)deltaT * new_velY/1000;

    (*c)[i]->update(new_velX, new_velY, new_posX,  new_posY);
  }
}


void Fisica::impulso() {
  
  std::vector<Corpo *> *c = this->lista->get_corpos();

  int posX = (int)((*c)[0])->get_posX();
  int posY = (int)((*c)[0])->get_posY();
  
  int hex_number = buscaHex(posX, posY);

  printw("X= %d", posX);
  printw("\nY= %d", posY);

}

int Mapa::buscaHex(int posX, int posY){
  int new_dist=0;
  int min_dist=200;
  int hex_number;

  for(int i=0; i< sizeof(this->listaX_Hex)/(sizeof(int)); i++){
      new_dist = abs(listaX_Hex[i] - posX) + abs(listaY_Hex[i] - posY); 

      if(new_dist < min_dist){
        min_dist = new_dist;
        hex_number = i;
      }
  }
    printw("\ndist= %d", min_dist);
  return hex_number;
}


Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */


  this->row = 50;
  this->col = 200;
}

int Tela::getRows(){
  return this->row;
}

int Tela::getCols(){
  return this->col;
}

void Tela::update() {
  move(13, 34);                                                     //TESTE
  printw("o");

  int x_pos, y_pos;
  getmaxyx(stdscr, this->row, this->col);

  if(this->row < MAX_X || this->col < MAX_Y){
    move(row/2, col/2);
    printw("Aumente a tela e reinicie o jogo");
  }

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  //Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++){

    x_pos = (int) ((*corpos_old)[k]->get_posX());
    y_pos = (int) ((*corpos_old)[k]->get_posY());

    move(x_pos, y_pos);   /* Move cursor to position */
   // echochar(' ');  /* Prints character, advances a position */
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++) {

    x_pos = (int) ((*corpos)[k]->get_posX());
    y_pos = (int) ((*corpos)[k]->get_posY());

    move(x_pos, y_pos);   /* Move cursor to position */
    echochar('*');  /* Prints character, advances a position */

    // Atualiza corpos antigos
    (*corpos_old)[k]->update((*corpos)[k]->get_velX(), (*corpos)[k]->get_velY(), (*corpos)[k]->get_posX(), (*corpos)[k]->get_posY());
  }

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}


/*
class Teclado {
  private:
    char ultima_captura;
    int rodando;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
}

*/

void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
  // Inicializa ncurses
  raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);
}

void Teclado::stop() {
  this->rodando = 0;
  (this->kb_thread).join();
}

char Teclado::getchar() {
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}
