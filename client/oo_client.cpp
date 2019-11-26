#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "oo_client.hpp"
#include <cstdlib>
#include <ncurses.h>

using namespace std::chrono;

// Funcao que roda em uma outra thread para capturar o teclado
void threadTeclado(char *keybuffer, int *control) {
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR)     (*keybuffer) = c;
    else            (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
  }
  return;
}

// Classe Teclado

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

// Iniciando a leitura do teclado
void Teclado::init() {
  // Inicializa ncurses
  raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  std::thread newthread(threadTeclado, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);
}

void Teclado::stop() {
  this->rodando = 0;
  (this->kb_thread).join();
}

// Funcao que le um caracter
char Teclado::getchar() {
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}



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
  j["velY"] = this->velY;
  j["velX"] = this->velX;
  j["posY"] = this->posY;
  j["posX"] = this->posX;
  j["orb"] = this->orb;
  j["rot"] = this->rot;
  j["pos_orb"] = this->pos_orb;
  j["cor"] = this->cor;
  j["jogador"] = this->jogador;

  return j.dump();
}

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
  int i;
  for(i=0;i<corpos->size();i++) {
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



// Classe Tela

Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY, Mapa* mapa) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
  this->mapa = mapa;
}

// Criando a tela, escolhendo cores e criando o mapa inicial
void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */

  // Escolhendo indice das cores dos corpos
  // Escolhendo indice das cores dos corpos
  start_color();
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_WHITE, COLOR_RED);
  init_pair(3, COLOR_WHITE, COLOR_YELLOW);
  init_pair(4, COLOR_WHITE, COLOR_CYAN);
  init_pair(5, COLOR_WHITE, COLOR_MAGENTA);

  // Tamanho do mapa
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
  move (51,10);
  printw("s - pronto para iniciar, espaco - impulso e entrar em orbita, q para sair do jogo");
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
    echochar('.');

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

// Funcao que atualiza a tela a cada iteracao, retorna 1 se a tela eh menor do que o necessario e 0 caso contrario
int Tela::update() {

  int x_pos, y_pos;
  getmaxyx(stdscr, this->row, this->col);

  // Se a tela eh pequena demais
  if(this->row < MAX_X || this->col < MAX_Y){
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

    if (abs(hexX[hex]-x_pos) < 9 && abs(hexY[hex]-y_pos) < 9) {
      //Centro do hexagono
      move(hexX[hex],hexY[hex]);
      echochar('.');

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
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++) {

    x_pos = (int) ((*corpos)[k]->get_posX());
    y_pos = (int) ((*corpos)[k]->get_posY());

    if ((*corpos)[k]->get_jogador()) { // Jogador
      move(x_pos, y_pos);
      attron(COLOR_PAIR((*corpos)[k]->get_cor()));
      echochar('*');
      attroff(COLOR_PAIR((*corpos)[k]->get_cor()));
    } else { // Outros corpos
      move(x_pos, y_pos);
      attron(COLOR_PAIR((*corpos)[k]->get_cor()));
      echochar('o');
      attroff(COLOR_PAIR((*corpos)[k]->get_cor()));
    }

    // Atualiza corpos antigos
    if (corpos->size() == corpos_old->size()) { // Se nao tem corpos novos
      (*corpos_old)[k]->update((*corpos)[k]->get_velX(), (*corpos)[k]->get_velY(), (*corpos)[k]->get_posX(), (*corpos)[k]->get_posY());
    } else { // Se tem corpos novos
      this->lista_anterior->copy(this->lista);
    }
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


// Funcao que roda em uma outra thread para receber os dados do servidor
void threadCorpos(Cliente* client, ListaDeCorpos* l) {
  char reply[10000];
  int msg_len;
  while(client->getRodando() == 1) {
    msg_len = recv(client->getSocket(), reply, 10000, MSG_DONTWAIT);
    if (msg_len > 10) {
      std::string data(reply);
      if (data.length() > 1)
        l->unserialize(data);
    }
  }
}

// Thread que envia os comandos do teclado
void threadEnviaComandos(Cliente* client, Teclado* teclado) {
  while(client->getRodando()) {
    char c = teclado->getchar();
    if(c == ' ' || c == 'q' || c == 's'){
      if((send(client->getSocket(), &c, 1, 0) == -1) || c == 'q') {
        printw("Pressione qualquer tecla para sair");
        client->setRodando(0);
      }
      send(client->getSocket(), 0, 1, 0);
    }
  }
  
}

// Classe Cliente

Cliente::Cliente() {

}

int Cliente::initClient() {
  struct sockaddr_in target;

  this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
   
  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  char ip[16];
  printf("Insira o ip do servidor:");
  scanf("%s", ip);
  inet_aton(ip, &(target.sin_addr));
  if(connect(this->socket_fd, (struct sockaddr*)&target, sizeof(target))) {
    return 1;
  }
  this->rodando = 1;
  return 0;
}

void Cliente::endClient() {
  this->rodando = 0;
  (this->corpos_thread).join();
  (this->kb_thread).join();
  close(this->socket_fd);
}


void Cliente::setBuffer(char buffer) {
  this->input_buffer = buffer;
}
char Cliente::getBuffer() {
  char c = this->input_buffer;
  this->input_buffer = 0;
  return c;
}
void Cliente::setRodando(int rodando) {
  this->rodando = rodando;
}
int Cliente::getRodando() {
  return this->rodando;
}
void Cliente::setConnection(int connection) {
  this->connection_fd = connection;
}
int Cliente::getConnection() {
  return this->connection_fd;
}
void Cliente::setSocket(int socket) {
  this->socket_fd = socket;
}
int Cliente::getSocket() {
  return this->socket_fd;
}



