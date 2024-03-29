#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "oo_server.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std::chrono;
using json = nlohmann::json;

// Classe Corpo

Corpo::Corpo() {
}


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
void Corpo::set_orb(int orb){
  this->orb = orb;
}
int Corpo::get_orb(){
  return this->orb;
}
void Corpo::set_rot(int rot){
  this->rot = rot;
}
int Corpo::get_rot(){
  return this->rot;
}
void Corpo::set_cor(int cor){
  this->cor = cor;
}
int Corpo::get_cor(){
  return this->cor;
}
int Corpo::get_pos_orb(){
  return this->pos_orb;
}
void Corpo::set_pos_orb(int pos_orb){
  this->pos_orb = pos_orb;
}
int Corpo::get_jogador(){
  return this->jogador;
}
void Corpo::set_jogador(int jogador){
  this->jogador = jogador;
}
// Funcao para serializar um corpo
std::string Corpo::serialize(){
  json j;
  j["velY"] = (int) this->velY;
  j["velX"] = (int) this->velX;
  j["posY"] = (int) this->posY;
  j["posX"] = (int) this->posX;
  j["orb"] = this->orb;
  j["rot"] = this->rot;
  j["pos_orb"] = this->pos_orb;
  j["cor"] = this->cor;
  j["jogador"] = this->jogador;

  return j.dump();
}
// Funcao para desserializar um corpo e copiar seus atributos
void Corpo::unserialize(std::string corpo_serializado){
  json j;
  j = json::parse(corpo_serializado);
  this->velY = j.at("velY");
  this->velX = j.at("velX");
  this->posY = j.at("posY");
  this->posX = j.at("posX");
  this->orb = j.at("orb");
  this->rot = j.at("rot");
  this->pos_orb = j.at("pos_orb");
  this->cor = j.at("cor");
  this->jogador = j.at("jogador");
}


// Classe ListaDeCorpos

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

// Funcao para copiar outra lista de corpos
void ListaDeCorpos::copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo((*corpos)[k]->get_velX(), (*corpos)[k]->get_velY(), (*corpos)[k]->get_posX(), (*corpos)[k]->get_posY());
    this->add_corpo(c);
  }

}

// Funcao para adicionar um corpo a lista
void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}
std::string ListaDeCorpos::serialize() {
  json j;
  for(int i = 0; i < corpos->size(); i++) {
    j[i] = corpos->at(i)->serialize();
  }
  return j.dump();
}
void ListaDeCorpos::unserialize(std::string lista_serializada) {
  json j;
  j = json::parse(lista_serializada);
  for(int i = 0; i<corpos->size(); i++) {
    corpos->at(i)->unserialize(j.at(i));
  }
}


// Classe Mapa

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


  // posicoes em X dos centros dos hexagonos
  this->listaX_Hex[0] = this->listaX_Hex[1] = this->listaX_Hex[2] = 13;
  this->listaX_Hex[3] = this->listaX_Hex[4] = 25;
  this->listaX_Hex[5] = this->listaX_Hex[6] = this->listaX_Hex[7] = 38;

  // posicoes em Y dos centros hexagonos
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

// Funcao que retorna o indice do hexagono mais proximo da posicao dada
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

// Funcao que acha em qual orbita de um hexagono um corpo esta
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

// Funcao que indica se o corpo vai orbitar no sentido horario ('h') ou anti horario ('a') do hexagono
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


// Classe fisica

Fisica::Fisica(ListaDeCorpos *ldc, Mapa* mapa) {
  this->lista = ldc;
  this->mapa = mapa;
}

// Funcao que atualiza as posicoes dos corpos
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
      if(((*c)[i])->get_posY() <= MIN_Y || ((*c)[i])->get_posY() >= MAX_Y){
        new_velY =  (-1)*((*c)[i])->get_velY();
      }


      new_posX = (*c)[i]->get_posX() + (int)deltaT * new_velX/1000;
      new_posY = (*c)[i]->get_posY() + (int)deltaT * new_velY/1000;
    }

    // Orbita 1
    else if((*c)[i]->get_orb() == 1){
      // Avanca uma posicao no hexagono
      if((*c)[i]->get_rot() == 'h'){
        (*c)[i]->set_pos_orb( (int)((*c)[i]->get_pos_orb() + 1)%2);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb( ((int)(*c)[i]->get_pos_orb() - 1)%2);
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
        (*c)[i]->set_pos_orb((int) ((*c)[i]->get_pos_orb() + 1)%8);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb((int) ((*c)[i]->get_pos_orb() - 1)%8);
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
        (*c)[i]->set_pos_orb((int) ((*c)[i]->get_pos_orb() + 1)%12);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb((int) ((*c)[i]->get_pos_orb() - 1)%12);
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
        (*c)[i]->set_pos_orb((int) ((*c)[i]->get_pos_orb() + 1)%18);
      }
      else if((*c)[i]->get_rot() == 'a'){
        (*c)[i]->set_pos_orb((int) ((*c)[i]->get_pos_orb() - 1)%18);
        if ((*c)[i]->get_pos_orb() == -1) {
          (*c)[i]->set_pos_orb(17);
        }
      }
      new_posX =  this->mapa->get_listaX()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb4X()[((*c)[i])->get_pos_orb()];

      new_posY =  this->mapa->get_listaY()[this->mapa->buscaHex((*c)[i]->get_posX(), (*c)[i]->get_posY())] + \
                  this->mapa->get_orb4Y()[((*c)[i])->get_pos_orb()];
    }

    if(new_posX == 1) {
      new_posX++;
    }
    else if(new_posX == 50) {
      new_posX--;
    }
    if(new_posY == 1) {
      new_posY++;
    }
    else if(new_posY == 200) {
      new_posY--;
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
            if((*c)[i]->get_jogador()) { // Se eh jogador
              if(!(*c)[j]->get_cor()) { // Se for um corpo neutro
                (*c)[j]->set_cor((*c)[i]->get_cor());
              }
              else if ((*c)[j]->get_cor() != (*c)[i]->get_cor() && !(*c)[j]->get_jogador()) { // Se colidiu com uma bola inimiga
                return i + 1;
              }
            }
            if((*c)[j]->get_jogador()) { // Se eh jogador
              if(!(*c)[i]->get_cor()) { // Se for um corpo neutro
                (*c)[i]->set_cor((*c)[j]->get_cor());
              }
              else if ((*c)[i]->get_cor() != (*c)[j]->get_cor() && !(*c)[i]->get_jogador()) { // Se colidiu com uma bola inimiga
                return j + 1;
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

// Funcao para quando eh dado um impulso em um corpo, que pode fazer ele entrar ou sair da orbita, caso esteja em um hexagono,
// ou pular algumas posicoes,caso nao esteja em nenhum hexagono
void Fisica::impulso(int id) {

  std::vector<Corpo *> *c = this->lista->get_corpos();

  int posX = (int)((*c)[id])->get_posX();
  int posY = (int)((*c)[id])->get_posY();

  if(((*c)[id])->get_orb() != 0){
    // Se estiver orbitando
    // Sai da orbita
    int deltaX, deltaY, newVelX, newVelY;
    if ((*c)[id]->get_rot() == 'h') {
      if((*c)[id]->get_orb() == 1){
        deltaX = this->mapa->get_orb1X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb1X()[((*c)[id])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb1Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb1Y()[((*c)[id])->get_pos_orb() - 1];
      }
      else if((*c)[id]->get_orb() == 2){
        deltaX = this->mapa->get_orb2X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb2X()[((*c)[id])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb2Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb2Y()[((*c)[id])->get_pos_orb() - 1];
      }
      else if((*c)[id]->get_orb() == 3){
        deltaX = this->mapa->get_orb3X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb3X()[((*c)[id])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb3Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb3Y()[((*c)[id])->get_pos_orb() - 1];
      }
      else if((*c)[id]->get_orb() == 4){
        deltaX = this->mapa->get_orb4X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb4X()[((*c)[id])->get_pos_orb() - 1];
        deltaY = this->mapa->get_orb4Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb4Y()[((*c)[id])->get_pos_orb() - 1];
      }
    }
    else if ((*c)[id]->get_rot() == 'a') {
      if((*c)[id]->get_orb() == 1){
        deltaX = this->mapa->get_orb1X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb1X()[((*c)[id])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb1Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb1Y()[((*c)[id])->get_pos_orb() + 1];
      }
      else if((*c)[id]->get_orb() == 2){
        deltaX = this->mapa->get_orb2X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb2X()[((*c)[id])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb2Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb2Y()[((*c)[id])->get_pos_orb() + 1];
      }
      else if((*c)[id]->get_orb() == 3){
        deltaX = this->mapa->get_orb3X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb3X()[((*c)[id])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb3Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb3Y()[((*c)[id])->get_pos_orb() + 1];
      }
      else if((*c)[id]->get_orb() == 4){
        deltaX = this->mapa->get_orb4X()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb4X()[((*c)[id])->get_pos_orb() + 1];
        deltaY = this->mapa->get_orb4Y()[((*c)[id])->get_pos_orb()] - this->mapa->get_orb4Y()[((*c)[id])->get_pos_orb() + 1];
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

    ((*c)[id])->update(newVelX,newVelY,(*c)[id]->get_posX(),(*c)[id]->get_posY());
    ((*c)[id])->set_orb(0);
    return;
  }

  //Se nao estiver orbitando
  int hex_number = this->mapa->buscaHex(posX, posY);
  char orbita = this->mapa->orbita(posX, posY, hex_number, *(c->data()));

  // Se estiver fora de um hexagono e longe das bordas do mapa
  if(posX > 5 && posX < 45 && posY > 5 && posY < 195){
    float velX =  ((*c)[id])->get_velX();
    float velY =  ((*c)[id])->get_velY();
    //Avanca vel/4 posicoes
    ((*c)[id])->update(velX, velY, posX+velX/4, posY+velY/4);
  }

  char rotacao = this->mapa->rotacao(posX, posY, (int)((*c)[id])->get_velX(), (int)((*c)[id])->get_velY(), orbita, hex_number);

  (*c)[id]->set_orb(orbita);
  (*c)[id]->set_rot(rotacao);
}


// Funcao que roda em uma segunda thread para ouvir cada cliente
void threadServidor(Servidor* server, int id) {
  while (server->getRodando()) {
    char keybuffer;
    if (server->getConexaoUsada(id) == 1) {
      if (recv(server->getConnection(id), &keybuffer, 1, 0)) {
        server->setBuffer(keybuffer, id); // Atualizando buffer
        if (keybuffer == 'q') {
          printf("Jogador %d saiu.\n", id);
          send(server->getConnection(id), "q", 2, 0);
          server->setConnection(0, id);
          server->setConexaoUsada(0,id);
          server->removeJogador();
        }
      } else {
        server->setBuffer(0, id);
      }
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(50));
  }
  printf("Saindo da thread de ouvir clientes.\n");
  return;
}

// Thread que espera novas conexoes
void threadEsperaServidor(Servidor* server) {
  int conn_fd;
  while(server->getJogadores() < MAX_PLAYERS) {
    conn_fd = accept(server->getSocket(), (struct sockaddr*)&server->client, &server->client_size);
    if (conn_fd != -1) {
      for(int i = 0; i < MAX_PLAYERS; i++) {
        if(!server->getConexaoUsada(i)) {
          printf("Adicionando cliente %d.\n", i);
          server->setConnection(conn_fd,i);
          server->setConexaoUsada(1,i);
          server->novoJogador();
          printf("%d jogadores\n", server->getJogadores());
          break;
        }
      }
    }
    int acumulador = 0;
    for(int i = 0; i < MAX_PLAYERS; i++) {
      acumulador += server->getJogadorVivo(i);
    }
    if (acumulador == 2*server->getJogadores() && server->getJogadores() > 0) {
      printf("Todos os jogadores prontos, iniciando o jogo.\n");
      return;
    }
  }
  printf("Quantidade máxima de jogadores, iniciando jogo.\n");
  return;
}

// Thread para serializar e enviar corpos para os clientes
void threadEnviaCorpos(Servidor* server, ListaDeCorpos* l) {
  while(server->getRodando()) {
    std::string message = l->serialize();
    for(int i = 0; i < MAX_PLAYERS; i++) {
      if(server->getConexaoUsada(i)) {
        char mensagem[10000];
        if (server->getJogadores() == 1) { // Se for o unico jogador, ele ganhou
          printf("Jogador %d ganhou.\n", i);
          send(server->getConnection(i), "g", 2, 0);
          server->setConnection(0, i);
          server->setConexaoUsada(0,i);
          server->removeJogador();
          server->setRodando(0);
        } else if (server->getJogadorVivo(i) == -1) { // Se o jogador perdeu
          printf("Jogador %d morreu.\n", i);
          send(server->getConnection(i), "p", 2, 0);
          server->setConnection(0, i);
          server->setConexaoUsada(0,i);
          server->removeJogador();
        } else {
         strcpy(mensagem,message.c_str());
          if (send(server->getConnection(i), &mensagem, 10000, 0) == -1) { // Se nao foi possivel mandar a mensagem (Cliente desconectou)
            server->setConnection(0, i);
            server->setConexaoUsada(0,i);
            server->removeJogador();
          }
        }
      }
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }
  printf("Saindo da thread de serializar corpos.\n");
  return;
}

// Classe Servidor

Servidor::Servidor() {
  this->jogadores = 0;
  this->input_buffer = (char *)calloc(MAX_PLAYERS, sizeof(char));
}

void Servidor::initServer() {
  
  this->client_size = (socklen_t)sizeof(this->client);
  for (int i=0; i<MAX_PLAYERS; i++) {
    this->conexao_usada[i] = 0;
    this->jogador_vivo[i] = 0;
  }

  this->socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  (this->myself).sin_family = AF_INET;
  (this->myself).sin_port = htons(3001);
  char ip[16];
  printf("Insira o seu ip:");
  scanf("%s", ip);
  inet_aton(ip, &((this->myself).sin_addr));
  
  if (bind(this->socket_fd, (struct sockaddr*)&(this->myself), sizeof(this->myself)) != 0) {
    return;
  }

  listen(this->socket_fd, 2);

  this->rodando = 1;
  printf("Servidor iniciado!\n");
}


void Servidor::endServer() {
  printf("Terminando servidor.\n");
  this->rodando = 0;


  // Esperando threads
  for(int i = 0; i < MAX_PLAYERS; i++) {
    (this->kb_thread[i]).join();
  }
  (this->model_thread).join();

  for (int i=0; i<MAX_PLAYERS; i++) {
    if (this->conexao_usada[i]) {
      this->conexao_usada[i] = 0;
      close(this->connection_fd[i]);
    }
  }

  free(this->input_buffer);
}


void Servidor::novoJogador() {
  this->jogadores++;
}
void Servidor::removeJogador() {
  this->jogadores--;
}
int Servidor::getJogadores() {
  return this->jogadores;
}
void Servidor::setBuffer(char buffer, int pos) {
  this->input_buffer[pos] = buffer;
}
char Servidor::getBuffer(int pos) {
  char c = this->input_buffer[pos];
  this->input_buffer[pos] = 0;
  return c;
}
void Servidor::setJogadorVivo(int vivo, int pos) {
  this->jogador_vivo[pos] = vivo;
}
int Servidor::getJogadorVivo(int pos) {
  return this->jogador_vivo[pos];
}
void Servidor::setRodando(int rodando) {
  this->rodando = rodando;
}
int Servidor::getRodando() {
  return this->rodando;
}
void Servidor::setConnection(int connection, int pos) {
  this->connection_fd[pos] = connection;
}
int Servidor::getConnection(int pos) {
  return this->connection_fd[pos];
}
void Servidor::setConexaoUsada(int conexao, int pos) {
  this->conexao_usada[pos] = conexao;
}
int Servidor::getConexaoUsada(int pos) {
  return this->conexao_usada[pos];
}
void Servidor::setSocket(int socket) {
  this->socket_fd = socket;
}
int Servidor::getSocket() {
  return this->socket_fd;
}
