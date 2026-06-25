#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // Knihovna pro internetové protokoly (IP/TCP)
#include <arpa/inet.h>
#include <poll.h>
#include <thread>

#include "types.h"
#include "protocol.h"
#include "network.h"
#include "game.h"

int main() {
  const int PORT = 25565;

  Network_Init();
  std::thread Network_th(Network_Run);
  std::thread Game_th(Game_Run);

  Network_th.join();
  Game_th.join();

  return 0;
}
