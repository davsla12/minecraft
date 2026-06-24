#ifndef NETWORK_H
#define NETWORK_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // Knihovna pro internetové protokoly (IP/TCP)
#include <arpa/inet.h>
#include <poll.h>

#include "protocol.h"

void Network_Init();
void Network_Run();

#endif
