#include "types.h"
#include <sys/socket.h>
#include <unistd.h>


void VarIntw(std::vector<uint8_t>& buffer, int value) {
  uint32_t u_value = static_cast<uint32_t>(value);
  while (true) {
    if ((u_value & ~0x7F) == 0) {
      buffer.push_back(static_cast<uint8_t>(u_value));
      return;
    }
    buffer.push_back(static_cast<uint8_t>((u_value & 0x7F) | 0x80));
    u_value >>= 7;
  }
}

int VarIntr(int client_fd,int& retval) {
    uint32_t result = 0;
    int numRead = 0;
    uint8_t readByte;
    ssize_t bytes_received;

    do {
        // Čteme přesně 1 bajt přímo ze socketu
        bytes_received = read(client_fd, &readByte, 1);

        // Pokud klient zavřel spojení (0) nebo nastala chyba (-1)
        if (bytes_received <= 0) {
            return -1; // Vrátíme -1 jako signál chyby/odpojení
        }

        // Vezmeme spodních 7 bitů a posuneme je na správné místo
        uint32_t value = (readByte & 0x7F);
        result |= (value << (7 * numRead));

        numRead++;

        // Pojistka proti poškozeným datům (VarInt má max 5 bajtů)
        if (numRead > 5) {
            return -2;
        }

    } while ((readByte & 0x80) != 0); // Opakuj, pokud pokračovací bit je 1

    retval = static_cast<int>(result);
    return 0;
}

void writeString(std::vector<uint8_t>& buffer, const std::string& str) {
  VarIntw(buffer, str.length());
  buffer.insert(buffer.end(), str.begin(), str.end());
}

int Stringr(int client_fd,std::string& retval) {
    int size;
    if(VarIntr(client_fd,size)<=0)return -1;
    std::string tmpretval(size, '\0');
    ssize_t bytes_received = read(client_fd, &retval[0], size);
    if (bytes_received < size) {
        return -2;
    }
    retval = tmpretval;
    return 0;
}
