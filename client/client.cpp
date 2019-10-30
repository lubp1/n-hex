
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "oo_client.hpp"
#include <chrono>
#include <thread>
#include <ncurses.h>


using namespace std::chrono;


int main() {
  int socket_fd;
  struct sockaddr_in target;

  initscr();
  Teclado *teclado = new Teclado();
  teclado->init();



    while (1) {
      socket_fd = socket(AF_INET, SOCK_STREAM, 0);


      target.sin_family = AF_INET;
      target.sin_port = htons(3001);
      inet_aton("128.168.43.204", &(target.sin_addr));
      if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
        return 0;
      }
      // Lê o teclado
      char c = teclado->getchar();


      /* Agora, meu socket funciona como um descritor de arquivo usual */
      send(socket_fd, &c, 1, 0);
      if(c == 'q') {
        break;
      }

      close(socket_fd);


      std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }

  endwin();
  teclado->stop();

  return 0;
}
