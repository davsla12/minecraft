#ifndef USERS_H
#define USERS_H

#include <vector>

#include "types.h"

struct User{
int fd;
sockaddr_in addr_IP;
std::string addr;
State = HANDSHAKE;
}

std::vector<User> Users;

#endif
