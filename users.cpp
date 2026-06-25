#include "users.h"

#include <mutex>

std::mutex Users_mtx;
std::vector<User> Users;

int Users_getfd(User* retval,int fd){
  Users_mtx.lock();
  for(auto usr:Users){
    if(usr.fd == fd){
      if(retval != NULL)*retval = usr;
      Users_mtx.unlock();
      return 0;
    }
  }
  Users_mtx.unlock();
  return -1;
}

int Users_upd(User user,int fd){
  Users_mtx.lock();
  for(auto& usr:Users){
    if(usr.fd == fd){
      usr = user;
      Users_mtx.unlock();
      return 0;
    }
  }
  Users_mtx.unlock();
  return -1;
}

int Users_add(User user){
  Users_mtx.lock();
  Users.push_back(user);
  Users_mtx.unlock();
  return 0;
}

void Users_rem(int fd){
  Users_mtx.lock();

  // Projedeme vector od začátku do konce přes index 'i'
  for (size_t i = 0; i < Users.size(); i++) {
    if (Users[i].fd == fd) {
      // Smažeme prvek na pozici 'i'
      Users.erase(Users.begin() + i);

      // Okamžitě vyskočíme ven, takže je jedno, že se vector zkrátil!
      Users_mtx.unlock();
      return;
    }
  }

  Users_mtx.unlock();
}
