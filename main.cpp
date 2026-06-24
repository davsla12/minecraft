#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // Knihovna pro internetové protokoly (IP/TCP)
#include <arpa/inet.h>
#include <poll.h>

#include "types.h"
#include "protocol.h"
#include "network.h"

int main() {
  const int PORT = 25565;

  Network_Init();
  Network_Run();

  return 0;
}
