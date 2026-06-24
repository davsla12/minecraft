#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <cstdint>


#include "mapping.h"

struct Packet{
  int id;
  int fd;
  void* data = NULL;
};
int packet_r(int fd);

struct handshake_t{
  int version;
  std::string addr;
  unsigned short port;
  int nextstate;
};
int packet_handshake(int fd,handshake_t* retval);

#endif
