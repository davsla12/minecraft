#include "protocol.h"
#include "types.h"
#include "users.h"
#include "game.h"

#include <unistd.h>
#include <iostream>
#include <cstring>

int packet_r(int fd){
  int size;
  User user;
  Packet packet;
  packet.fd=fd;
  Users_getfd(&user,fd);
  if(VarIntr(fd,size)<0)return -1;
  if(VarIntr(fd,packet.id)<0)return -1;
  std::cout << "id:" << packet.id << std::endl;
  if(Users_getfd(&user,fd)<0)return -2;
  switch(user.state){
    case HANDSHAKE:
      switch(packet.id){
        case HANDSHAKING_TOSERVER_SET_PROTOCOL:
          packet.data = malloc(sizeof(handshake_t));
          if(packet_handshake(fd,(handshake_t*)packet.data)<0)goto exit;
          if (((handshake_t*)packet.data)->nextstate == 1){
            user.state = STATUS;
          }
          else if(((handshake_t*)packet.data)->nextstate == 2){
            user.state = LOGIN;
          }
          else goto exit;
          std::cout << "NextState:" << user.state << std::endl;
        Game_AddEvent(packet);

          break;
      }
      break;
        case STATUS:
          switch(packet.id){

          }

    //default:
      //goto exit;
  }

  Users_upd(user,user.fd);

  return 0;
  exit:
  free(packet.data);
  return -1;
}

int packet_handshake(int fd,handshake_t* retval){
  if(VarIntr(fd,retval->version)<0)return -1;
  if(Stringr(fd,retval->addr)<0)return -1;
  read(fd,&retval->port,2);
  if(VarIntr(fd,retval->nextstate)<0)return -1;
  return 0;
}

Packet packet_server_info(int fd) {
    Packet retval;
    retval.fd = fd;
    retval.id = STATUS_TOCLIENT_SERVER_INFO; // 0x00

    std::string json_motd = "{\"version\":{\"name\":\"1.8.9\",\"protocol\":47},\"players\":{\"max\":100,\"online\":0},\"description\":{\"text\":\"C++ Minecraft Server\"}}";

    // Vytvoříme buffer POUZE pro specifická data tohoto packetu
    std::vector<uint8_t> payload;

    // Zapíšeme délku JSON stringu a samotný JSON string
    VarIntw(payload, json_motd.length());
    payload.insert(payload.end(), json_motd.begin(), json_motd.end());

    // Alokujeme a zkopírujeme POUZE payload
    retval.size = payload.size();
    retval.data = malloc(retval.size);
    std::memcpy(retval.data, payload.data(), retval.size);

    return retval;
}







