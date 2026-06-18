#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <cstdint>

#include "mapping.h"

struct packet{
  int id;
  std::vector<uint8_t> data;
};

struct handshake_t{
  int packetID;
  int version;
  std::string addr;
  unsigned short port;
  int nextstate;
};
handshake_t handshakef(int fd);

void server_status(int fd);
void disconnect(int fd);
void success(int fd);
#endif
