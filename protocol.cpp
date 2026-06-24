#include "protocol.h"
#include "types.h"
#include "users.h"

#include <unistd.h>
#include <iostream>

int packet_r(int fd){
  int size;
  User user;
  Packet* packet = (Packet*)malloc(sizeof(Packet));
  packet->fd=fd;
  Users_getfd(&user,fd);
  if(VarIntr(fd,size)<0)return -1;
  if(VarIntr(fd,packet->id)<0)return -1;
  if(Users_getfd(&user,fd)<0)return -2;
  switch(user.state){
    case HANDSHAKE:
      switch(packet->id){
        case HANDSHAKING_TOSERVER_SET_PROTOCOL:
          packet->data = malloc(sizeof(handshake_t));
          if(packet_handshake(fd,(handshake_t*)packet->data)<0)goto exit;
          if (((handshake_t*)packet->data)->nextstate == 1){
            user.state = STATUS;
          }
          else if(((handshake_t*)packet->data)->nextstate == 2){
            user.state = LOGIN;
          }
          else goto exit;
          std::cout << user.state << std::endl;

          break;
      }break;
    case STATUS:
      switch(packet->id){
        case 0:
          break;
      }
  }

  return 0;
  exit:
  free(packet->data);
  free(packet);
  return -1;
}

int packet_handshake(int fd,handshake_t* retval){
  if(VarIntr(fd,retval->version)<0)return -1;
  if(Stringr(fd,retval->addr)<0)return -1;
  read(fd,&retval->port,2);
  if(VarIntr(fd,retval->nextstate)<0)return -1;
  return 0;
}
