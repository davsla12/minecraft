#include "users.h"

#include <mutex>

std::mutex Users_mtx;
std::vector<User> Users;

int Users_getfd(User* retval,int fd){
  Users_mtx.lock();
  for(auto usr:Users){
    if(usr.fd == fd){
      *retval = usr;
      Users_mtx.unlock();
      return 0;
    }
  }
  Users_mtx.unlock();
  return -1;
}
