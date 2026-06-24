#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <cstdint>

enum State{
HANDSHAKE,
STATUS,
LOGIN,
PLAY
};

void VarIntw(std::vector<uint8_t>& buffer, int value);
int VarIntr(int client_fd,int& retval);

void writeString(std::vector<uint8_t>& buffer, const std::string& str);
int Stringr(int client_fd,std::string& retval);

#endif
