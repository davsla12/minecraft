#ifndef USERS_H
#define USERS_H

#include <vector>

#include "types.h"

struct User{
int fd;
std::string username;
std::string addr;
State state = HANDSHAKE;
};

int Users_getfd(User* retval,int fd);

#endif
