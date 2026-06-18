#include "protocol.h"
#include "types.h"
#include <unistd.h>

handshake_t handshakef(int fd){
  VarIntr(fd);//packet size
  handshake_t retval;
  retval.packetID = VarIntr(fd);
  retval.version = VarIntr(fd);
  retval.addr = Stringr(fd);
  read(fd,&retval.port,sizeof(retval.port));
  retval.nextstate = VarIntr(fd);
  return retval;
};

void server_status(int fd){
  std::string json_response = R"({
    "version": { "name": "C++ TCP 2026", "protocol": 47 },
    "players": { "max": 10, "online": 1, "sample": [ { "name": "Test", "id":
    "00000000-0000-0000-0000-000000000000" } ] },
    "description": { "text": "§eAhoj! §fTohle bezi na cistem TCP!" }
  })";

  std::vector<uint8_t> packet_data;
  VarIntw(packet_data, 0x00); // Packet ID pro Status Response je 0x00
  writeString(packet_data, json_response);

  std::vector<uint8_t> final_packet;
  VarIntw(final_packet, packet_data.size());
  final_packet.insert(final_packet.end(), packet_data.begin(), packet_data.end());

  write(fd, final_packet.data(), final_packet.size());
}

void disconnect(int fd){
// 1. Připravíme si JSON text zprávy
std::string disconnect_json = "{\"text\": \"§cTento server slouží pouze jako info panel!\\n§7Nelze se realně připojit.\"}";

// 2. Sestavíme tělo paketu [ID] [String]
std::vector<uint8_t> packet_body;
VarIntw(packet_body, 0x00); // LOGIN Disconnect má ID 0x00
writeString(packet_body, disconnect_json); // Naše pomocná funkce pro zápis Stringu

// 3. Sestavíme finální paket [Délka] [Tělo]
std::vector<uint8_t> final_packet;
VarIntw(final_packet, packet_body.size());
final_packet.insert(final_packet.end(), packet_body.begin(), packet_body.end());

// 4. Odešleme klientovi přímo do socketu
write(fd, final_packet.data(), final_packet.size());
}

void success(int fd){
std::vector<uint8_t> packet_body;
VarIntw(packet_body, 0x02);
writeString(packet_body,"00000000-0000-0000-0000-000000000000");
writeString(packet_body,"davsla12");
std::vector<uint8_t> final_packet;
VarIntw(final_packet,packet_body.size());
final_packet.insert(final_packet.end(), packet_body.begin(), packet_body.end());
write(fd, final_packet.data(), final_packet.size());
}
