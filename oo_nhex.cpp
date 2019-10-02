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

Fisica::Fisica(ListaDeCorpos *ldc, Mapa* mapa) {
  this->lista = ldc;
  this->mapa = mapa;
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

  int hex_number = this->mapa->buscaHex(posX, posY);


}


Mapa::Mapa() {
  this->listaX_Hex = (int*)malloc(8*sizeof(int));
  this->listaY_Hex = (int*)malloc(8*sizeof(int));

  this->orb1X = (int*)malloc(2*sizeof(int));
  this->orb1Y = (int*)malloc(2*sizeof(int));
  this->orb2X = (int*)malloc(8*sizeof(int));
  this->orb2Y = (int*)malloc(8*sizeof(int));
  this->orb3X = (int*)malloc(12*sizeof(int));
  this->orb3Y = (int*)malloc(12*sizeof(int));
  this->orb4X = (int*)malloc(18*sizeof(int));
  this->orb4Y = (int*)malloc(18*sizeof(int));


  // posicoes em X dos hexagonos
  this->listaX_Hex[0] = this->listaX_Hex[1] = this->listaX_Hex[2] = 13;
  this->listaX_Hex[3] = this->listaX_Hex[4] = 25;
  this->listaX_Hex[5] = this->listaX_Hex[6] = this->listaX_Hex[7] = 38;

  // posicoes em Y dos hexagonos
  this->listaY_Hex[0] = this->listaY_Hex[5] = 34;
  this->listaY_Hex[1] = this->listaY_Hex[6] = 101;
  this->listaY_Hex[2] = this->listaY_Hex[7] = 167;
  this->listaY_Hex[3] = 57;
  this->listaY_Hex[4] = 144;


  // orbita mais interna
  this->orb1X[0] = this->orb1X[1] = 0;
  this->orb1Y[0] = -1;
  this->orb1Y[1] = 1;

  this->orb2X[0] = this->orb2X[4] = 0;
  this->orb2X[1] = this->orb2X[2] = this->orb2X[3] = 1;
  this->orb2X[5] = this->orb2X[6] = this->orb2X[7] = -1;
  this->orb2Y[0] = -3;
  this->orb2Y[1] = this->orb2Y[7] = -2;
  this->orb2Y[2] = this->orb2Y[6] = 0;
  this->orb2Y[3] = this->orb2Y[5] = 2;
  this->orb2Y[4] = 3;

  this->orb3X[0] = this->orb3X[6] = 0;
  this->orb3X[1] = this->orb3X[5] = 1;
  this->orb3X[2] = this->orb3X[3] = this->orb3X[4] = 2;
  this->orb3X[7] = this->orb3X[11] = -1;
  this->orb3X[8] = this->orb3X[9] = this->orb3X[10] = -2;
  this->orb3Y[0] = -5;
  this->orb3Y[1] = this->orb3Y[11] = -4;
  this->orb3Y[2] = this->orb3Y[10] = -2;
  this->orb3Y[3] = this->orb3Y[9] = 0;
  this->orb3Y[4] = this->orb3Y[8] = 2;
  this->orb3Y[5] = this->orb3Y[7] = 4;
  this->orb3Y[6] = 5;

  // Orbita mais externa
  this->orb4X[0] = this->orb4X[9] = 0;
  this->orb4X[1] = this->orb4X[8] = 1;
  this->orb4X[2] = this->orb4X[7] = 2;
  this->orb4X[3] = this->orb4X[4] = this->orb4X[5] = this->orb4X[6] = 3;
  this->orb4X[10] = this->orb4X[17] = -1;
  this->orb4X[11] = this->orb4X[16] = -2;
  this->orb4X[12] = this->orb4X[13] = this->orb4X[14] = this->orb4X[15] = -3;
  this->orb4Y[0] = -6;
  this->orb4Y[1] = this->orb4Y[17] = -5;
  this->orb4Y[2] = this->orb4Y[16] = -4;
  this->orb4Y[3] = this->orb4Y[15] = -3;
  this->orb4Y[4] = this->orb4Y[14] = -1;
  this->orb4Y[5] = this->orb4Y[13] = 1;
  this->orb4Y[6] = this->orb4Y[12] = 3;
  this->orb4Y[7] = this->orb4Y[11] = 4;
  this->orb4Y[8] = this->orb4Y[10] = 5;
  this->orb4Y[9] = 6;


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
  return hex_number;
}


Mapa::~Mapa() {
  free(this->listaX_Hex);
  free(this->listaY_Hex);

  free(this->orb1X);
  free(this->orb1Y);
  free(this->orb2X);
  free(this->orb2Y);
  free(this->orb3X);
  free(this->orb3Y);
  free(this->orb4X);
  free(this->orb4Y);
}


Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY, Mapa* mapa) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
  this->mapa = mapa;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */


  this->row = 50;
  this->col = 200;

  int i;
  for (i=0;i<201;i++) {
    move(0,i);
    echochar('-');
    move(50,  i);
    echochar('-');
  }
  for (i=0;i<51;i++) {
    move(i,0);
    echochar('|');
    move(i,201);
    echochar('|');
  }

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
    echochar(' ');  /* Prints character, advances a position */
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
