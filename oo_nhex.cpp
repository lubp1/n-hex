#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "oo_nhex.hpp"
#include <cstdlib>
#include <ncurses.h>

using namespace std::chrono;

Corpo::Corpo(float vX, float vY, float posX, float posY) {
  this->velX = vX;
  this->velY = vY;
  this->posX = posX;
  this->posY = posY;
  this->orb = 0;
  this->rot = 0;
  this->cor = 0;
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
void Corpo::set_rot(char rot){
  this->rot = rot;
}
char Corpo::get_rot(){
  return this->rot;
}
void Corpo::set_cor(char cor){
  this->cor = cor;
}

char Corpo::get_cor(){
  return this->cor;
}
int Corpo::get_pos_orb(){
  return this->pos_orb;
}
void Corpo::set_pos_orb(int pos_orb){
  this->pos_orb = pos_orb;
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
  this->listaY_Hex[3] = 63;
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


int* Mapa::get_listaX() {
  return this->listaX_Hex;
}
int* Mapa::get_listaY() {
  return this->listaY_Hex;
}
int* Mapa::get_orb1X(){
  return this->orb1X;
}
int* Mapa::get_orb2X(){
  return this->orb2X;
}
int* Mapa::get_orb3X(){
  return this->orb3X;
}
int* Mapa::get_orb4X(){
  return this->orb4X;
}
int* Mapa::get_orb1Y(){
  return this->orb1Y;
}
int* Mapa::get_orb2Y(){
  return this->orb2Y;
}
int* Mapa::get_orb3Y(){
  return this->orb3Y;
}
int* Mapa::get_orb4Y(){
  return this->orb4Y;
}

int Mapa::buscaHex(int posX, int posY){
  int new_dist=0;
  int min_dist=1000000;
  int hex_number = 0;

  for(int i=0; i<8; i++){
      new_dist = (listaX_Hex[i] - posX)*(listaX_Hex[i] - posX) + (listaY_Hex[i] - posY)*(listaY_Hex[i] - posY);

      if(new_dist < min_dist){
        min_dist = new_dist;
        hex_number = i;
      }
  }
  return hex_number;
}

char Mapa::orbita(int x, int y, int hex, Corpo *c) {
  int i;
  // Calculo posicoes relativas
  x =(int)(x - this->listaX_Hex[hex]);
  y =(int)(y - this->listaY_Hex[hex]);

  for (i=0;i<18;i++) {
    //Orbita 1
    if (i < 2 && x == this->orb1X[i] && y == this->orb1Y[i]) {
      c->set_pos_orb(i);
      return 1;
    }
    //Orbita 2
    if (i < 8 && x == this->orb2X[i] && y == this->orb2Y[i]) {
      c->set_pos_orb(i);
      return 2;
    }
    //Orbita 3
    if (i < 12 && x == this->orb3X[i] && y == this->orb3Y[i]) {
      c->set_pos_orb(i);
      return 3;
    }
    //Orbita 4
    if (i < 18 && x == this->orb4X[i] && y == this->orb4Y[i]) {
      c->set_pos_orb(i);
      return 4;
    }
  }

  int raio = abs(x)+abs(y);
  if(raio <= 7){
     int indice_pos, dist, min_dist = 7;
      // Esta em uma das posicoes vazias do hexagono
      if(raio == 7 || raio == 5 || (abs(x)==3 && abs(y)==0 )){
        // Orbita 4
        for(int j=0; j<18; j++){
          // Percorre o vetor de pos da orbita 4 e checa a pos mais proxima
          dist = (x - orb4X[j]) + (y - orb4Y[j]);

          if(dist < min_dist){
            min_dist = dist;
            indice_pos = j;
          }
        }
        c->set_pos_orb(indice_pos);
        return 4;
      }
      if(raio == 4 || (abs(x)==2 && abs(y)==1)){
        // Orbita 3
        for(int j=0; j<12; j++){
          // Percorre o vetor de pos da orbita 3 e checa a pos mais proxima
          dist = (x - orb3X[j]) + (y - orb3Y[j]);

          if(dist < min_dist){
            min_dist = dist;
            indice_pos = j;
          }
        }
        c->set_pos_orb(indice_pos);
        return 3;
      }
      if(raio == 2){
        // Orbita 4
        for(int j=0; j<8; j++){
          // Percorre o vetor de pos da orbita 2 e checa a pos mais proxima
          dist = (x - orb2X[j]) + (y - orb2Y[j]);

          if(dist < min_dist){
            min_dist = dist;
            indice_pos = j;
          }
        }
        c->set_pos_orb(indice_pos);
        return 2;
      }
    }

  return 0;
}

char Mapa::rotacao(int x, int y, int vx, int vy, char orb, int hex) {
  char sentido;
  // calcula sentido de rotacao
  sentido = vy * (x - this->listaX_Hex[hex]);
      // >= 0   -    horario
  if(sentido >= 0)
    sentido = 'h';        // horario
  else if(sentido < 0)
    sentido = 'a';        // anti-horario

  return sentido;
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


Fisica::Fisica(ListaDeCorpos *ldc, Mapa* mapa) {
  this->lista = ldc;
  this->mapa = mapa;
}

int Fisica::update(float deltaT, int tamTela) {
  // Atualiza parametros dos corpos
  std::vector<Corpo *> *c = this->lista->get_corpos();
  // Atualiza posicoes dos corpos
  for (int i = 0; i < (*c).size(); i++) {
    float new_velX = ((*c)[i])->get_velX();
    float new_velY = ((*c)[i])->get_velY();
    float new_posX, new_posY;

    // Se nao estiver orbitando
    if((*c)[i]->get_orb() == 0){
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


      for (int j = 0; j < (*c).size(); j++) {
        if(i != j && (*c)[i]->get_posX() == (*c)[j]->get_posX() && (*c)[i]->get_posY() == (*c)[j]->get_posY()) {
          (*c)[i]->update(-(*c)[i]->get_velX(), -(*c)[i]->get_velY(), (*c)[i]->get_posX(),  (*c)[i]->get_posY());
          (*c)[j]->update(-(*c)[j]->get_velX(), -(*c)[j]->get_velY(), (*c)[j]->get_posX(),  (*c)[j]->get_posY());
        }
      }

      new_posX = (*c)[i]->get_posX() + (int)deltaT * new_velX/1000;
      new_posY = (*c)[i]->get_posY() + (int)deltaT * new_velY/1000;
    }

    // Orbita 1
    else if((*c)[i]->get_orb() == 1){
      // Avanca uma posicao no hexagono
      if((*c)[i]->get_rot() == 'h'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() + 1)%2);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb( (*c)[i]->get_pos_orb() - 1);
        if ((*c)[i]->get_pos_orb() == -1) {
          (*c)[i]->set_pos_orb(1);
        }
      }
      new_posX =  this->mapa->get_listaX()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb1X()[((*c)[i])->get_pos_orb()];

      new_posY =  this->mapa->get_listaY()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb1Y()[((*c)[i])->get_pos_orb()];
    }
    // Orbita 2
    else if((*c)[i]->get_orb() == 2){
      // Avanca uma posicao no hexagono
      if((*c)[i]->get_rot() == 'h'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() + 1)%8);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() - 1)%8);
        if ((*c)[i]->get_pos_orb() == -1) {
          (*c)[i]->set_pos_orb(7);
        }
      }
      new_posX =  this->mapa->get_listaX()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb2X()[((*c)[i])->get_pos_orb()];

      new_posY =  this->mapa->get_listaY()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb2Y()[((*c)[i])->get_pos_orb()];
    }
    // Orbita 3
    else if((*c)[i]->get_orb() == 3){
      // Avanca uma posicao no hexagono
      if((*c)[i]->get_rot() == 'h'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() + 1)%12);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() - 1)%12);
        if ((*c)[i]->get_pos_orb() == -1) {
          (*c)[i]->set_pos_orb(11);
        }
      }
      new_posX =  this->mapa->get_listaX()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb3X()[((*c)[i])->get_pos_orb()];

      new_posY =  this->mapa->get_listaY()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb3Y()[((*c)[i])->get_pos_orb()];
    }
    // Orbita 4
    else if((*c)[i]->get_orb() == 4){
      // Avanca uma posicao no hexagono
      if((*c)[i]->get_rot() == 'h'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() + 1)%18);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb( ((*c)[i]->get_pos_orb() - 1)%18);
        if ((*c)[i]->get_pos_orb() == -1) {
          (*c)[i]->set_pos_orb(17);
        }
      }
      new_posX =  this->mapa->get_listaX()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb4X()[((*c)[i])->get_pos_orb()];

      new_posY =  this->mapa->get_listaY()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb4Y()[((*c)[i])->get_pos_orb()];
    }


    (*c)[i]->update(new_velX, new_velY, new_posX,  new_posY);

  }

  // Checa Colisoes
  for (int i = 0; i < (*c).size(); i++) {
    for (int j=i+1; j < (*c).size(); j++) {
      if ( ((*c)[i]->get_posX() > ((*c)[j]->get_posX()-1 ))&&((*c)[i]->get_posX() < ((*c)[j]->get_posX()+1 ))
         &&((*c)[i]->get_posY() > ((*c)[j]->get_posY()-1 ))&&((*c)[i]->get_posY() < ((*c)[j]->get_posY()+1))){
            // Colisao detectada
            float velXi = (*c)[i]->get_velX();
            float velYi = (*c)[i]->get_velY();
            float velXj = (*c)[j]->get_velX();
            float velYj = (*c)[j]->get_velY();
            if(!i) {
              if(!(*c)[j]->get_cor()) {
                (*c)[j]->set_cor(1);
              }
              else if ((*c)[j]->get_cor() != 1) {
                return 1;
              }
            }
            if(!j) {
              if(!(*c)[i]->get_cor()) {
                (*c)[i]->set_cor(1);
              }
              else if ((*c)[i]->get_cor() != 1) {
                return 1;
              }
            }
            // Troca as velocidades dos dois corpos
            (*c)[i]->update(velXj, velYj, (*c)[i]->get_posX(), (*c)[i]->get_posY());
            (*c)[j]->update(velXi, velYi, (*c)[j]->get_posX(), (*c)[j]->get_posY());
      }
    }
  }
  return 0;
}

void Fisica::impulso() {

  std::vector<Corpo *> *c = this->lista->get_corpos();

  int posX = (int)((*c)[0])->get_posX();
  int posY = (int)((*c)[0])->get_posY();

  if(((*c)[0])->get_orb() != 0){
    // Se estiver orbitando
    // Sai da orbita
    int deltaX, deltaY, newVelX, newVelY;
    if ((*c)[0]->get_rot() == 'h') {
      if((*c)[0]->get_orb() == 1){
        deltaX = this->mapa->get_orb1X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb1X()[((*c)[0])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb1Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb1Y()[((*c)[0])->get_pos_orb() - 1];
      }
      else if((*c)[0]->get_orb() == 2){
        deltaX = this->mapa->get_orb2X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb2X()[((*c)[0])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb2Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb2Y()[((*c)[0])->get_pos_orb() - 1];
      }
      else if((*c)[0]->get_orb() == 3){
        deltaX = this->mapa->get_orb3X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb3X()[((*c)[0])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb3Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb3Y()[((*c)[0])->get_pos_orb() - 1];
      }
      else if((*c)[0]->get_orb() == 4){
        deltaX = this->mapa->get_orb4X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb4X()[((*c)[0])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb4Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb4Y()[((*c)[0])->get_pos_orb() - 1];
      }
    }
    else if ((*c)[0]->get_rot() == 'a') {
      if((*c)[0]->get_orb() == 1){
        deltaX = this->mapa->get_orb1X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb1X()[((*c)[0])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb1Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb1Y()[((*c)[0])->get_pos_orb() + 1];
      }
      else if((*c)[0]->get_orb() == 2){
        deltaX = this->mapa->get_orb2X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb2X()[((*c)[0])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb2Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb2Y()[((*c)[0])->get_pos_orb() + 1];
      }
      else if((*c)[0]->get_orb() == 3){
        deltaX = this->mapa->get_orb3X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb3X()[((*c)[0])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb3Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb3Y()[((*c)[0])->get_pos_orb() + 1];
      }
      else if((*c)[0]->get_orb() == 4){
        deltaX = this->mapa->get_orb4X()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb4X()[((*c)[0])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb4Y()[((*c)[0])->get_pos_orb()] - this->mapa->get_orb4Y()[((*c)[0])->get_pos_orb() + 1];
      }
    }
    if (deltaX >= 0) {
      newVelX = 10;
    } else {
      newVelX = -10;
    }

    if (deltaY >= 0) {
      newVelY = 10;
    } else {
      newVelY = -10;
    }

    ((*c)[0])->update(newVelX,newVelY,(*c)[0]->get_posX(),(*c)[0]->get_posY());
    ((*c)[0])->set_orb(0);
    return;
  }
  //Se nao estiver orbitando

  int hex_number = this->mapa->buscaHex(posX, posY);
  char orbita = this->mapa->orbita(posX, posY, hex_number, *(c->data()));

  // Se estiver fora de um hexagono
  if(orbita == 0){
    float velX =  ((*c)[0])->get_velX();
    float velY =  ((*c)[0])->get_velY();
    //Avanca vel/4 posicoes
    ((*c)[0])->update(velX, velY, posX+velX/4, posY+velY/4);
  }

  char rotacao = this->mapa->rotacao(posX, posY, (int)((*c)[0])->get_velX(), (int)((*c)[0])->get_velY(), orbita, hex_number);

  (*c)[0]->set_orb(orbita);
  (*c)[0]->set_rot(rotacao);
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


  start_color();
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_BLUE, COLOR_WHITE);
  init_pair(2, COLOR_GREEN, COLOR_WHITE);

  this->row = 50;
  this->col = 200;

  // Limpando a tela
  erase();


  // Imprimindo bordas
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

  // Imprimindo os hexagonos
  int* hexX = this->mapa->get_listaX();
  int* hexY = this->mapa->get_listaY();
  for (i=0;i<8; i++) {

    //Centro do hexagono
    move(hexX[i],hexY[i]);
    echochar('0');

    // Lados inclinados
    int j;
    for (j=0;j<4;j++) {
      move(hexX[i]-j,hexY[i]-8+j);
      echochar('/');
      move(hexX[i]-j,hexY[i]+8-j);
      echochar('\\');
      move(hexX[i]+1+j,hexY[i]-8+j);
      echochar('\\');
      move(hexX[i]+1+j,hexY[i]+8-j);
      echochar('/');
    }

    // Lados retos
    for(j=0;j<9;j++) {
      move(hexX[i]-4,hexY[i]-4+j);
      echochar('_');
      move(hexX[i]+4,hexY[i]-4+j);
      echochar('_');
    }
  }

}

int Tela::getRows(){
  return this->row;
}

int Tela::getCols(){
  return this->col;
}

int Tela::update() {

  int x_pos, y_pos;
  getmaxyx(stdscr, this->row, this->col);

  if(this->row < MAX_X || this->col < MAX_Y){
    // Limpando a tela
    erase();
    move(row/2, col/2);
    printw("Aumente a tela e reinicie o jogo, aperte 'q' para sair");
    std::this_thread::sleep_for (std::chrono::seconds(1));
    return 1;
  }

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  //Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++){

    x_pos = (int) ((*corpos_old)[k]->get_posX());
    y_pos = (int) ((*corpos_old)[k]->get_posY());


    move(x_pos, y_pos);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */

    // Reimprimindo o hexagono mais proximo, para caso ele tenha sido apagado
    int hex = this->mapa->buscaHex(x_pos, y_pos);
    int* hexX = this->mapa->get_listaX();
    int* hexY = this->mapa->get_listaY();

    //Centro do hexagono
    move(hexX[hex],hexY[hex]);
    echochar('0');

    // Lados inclinados
    int j;
    for (j=0;j<4;j++) {
      move(hexX[hex]-j,hexY[hex]-8+j);
      echochar('/');
      move(hexX[hex]-j,hexY[hex]+8-j);
      echochar('\\');
      move(hexX[hex]+1+j,hexY[hex]-8+j);
      echochar('\\');
      move(hexX[hex]+1+j,hexY[hex]+8-j);
      echochar('/');
    }

    // Lados retos
    for(j=0;j<9;j++) {
      move(hexX[hex]-4,hexY[hex]-4+j);
      echochar('_');
      move(hexX[hex]+4,hexY[hex]-4+j);
      echochar('_');
    }
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++) {

    x_pos = (int) ((*corpos)[k]->get_posX());
    y_pos = (int) ((*corpos)[k]->get_posY());

    if (!k) {
      move(x_pos, y_pos);   /* Move cursor to position */
      attron(COLOR_PAIR(1));
      echochar('*');  /* Prints character, advances a position */
      attroff(COLOR_PAIR(1));
    } else {
      move(x_pos, y_pos);   /* Move cursor to position */
      attron(COLOR_PAIR((*corpos)[k]->get_cor()));
      echochar('o');  /* Prints character, advances a position */
      attroff(COLOR_PAIR((*corpos)[k]->get_cor()));
    }

    // Atualiza corpos antigos
    (*corpos_old)[k]->update((*corpos)[k]->get_velX(), (*corpos)[k]->get_velY(), (*corpos)[k]->get_posX(), (*corpos)[k]->get_posY());
  }

  // Atualiza tela
  refresh();
  return 0;
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
