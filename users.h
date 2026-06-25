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
int Users_add(User user);
int Users_upd(User user,int fd);
void Users_rem(int fd);

#endif
