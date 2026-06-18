#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // Knihovna pro internetové protokoly (IP/TCP)
#include <arpa/inet.h>

#include "types.h"
#include "protocol.h"

int main() {
  const int PORT = 25565;

  // 1. Vytvoření socketu typu AF_INET (IPv4 internetový protokol) a SOCK_STREAM (TCP)
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    std::cerr << "Chyba při vytváření socketu!" << std::endl;
    return 1;
  }

  // Nastavení socketu, aby se port uvolnil ihned po vypnutí serveru (zabrání chybě "Address already in use")
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // 2. Příprava adresy struktury sockaddr_in (internetová adresa)
  struct sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;                  // Protokolová rodina IPv4
  addr.sin_addr.s_addr = INADDR_ANY;          // Naslouchat na všech dostupných síťových rozhraních
  addr.sin_port = htons(PORT);               // Převod portu na Network Byte Order (Big Endian)

  // 3. Bind - přiřazení socketu k IP adrese a portu
  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    std::cerr << "Chyba při bindování na port " << PORT << "!" << std::endl;
    close(server_fd);
    return 1;
  }

  // 4. Listen - začátek naslouchání pro příchozí TCP spojení
  if (listen(server_fd, 5) == -1) {
    std::cerr << "Chyba při listen!" << std::endl;
    close(server_fd);
    return 1;
  }

  std::cout << "Status server spusten pres sys/socket na TCP portu: " << PORT << "\nCekam na Minecraft klienta..." << std::endl;

  while (true) {
    // 5. Accept - přijetí nového TCP spojení
    sockaddr_in client_addr{};            // Příprava struktury pro data klienta
    socklen_t client_len = sizeof(client_addr); // Inicializace velikosti

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        std::cerr << "Chyba při accept!" << std::endl;
        continue;
    }

    handshake_t first = handshakef(client_fd);
    std::cout << "nekdo pingnul z " << first.addr << ":" << first.port << std::endl;
    // 1. Převedení IP adresy na řetězec (string/char*)
    char* ip_string = inet_ntoa(client_addr.sin_addr);

    // 2. Převedení portu na běžné číslo (int)
    int port = ntohs(client_addr.sin_port);

    // 3. Výpis do konzole
    std::cout << "Klient se připojil z IP: " << ip_string << " na portu: " << port << std::endl;

    switch(first.nextstate){

      case 1:{
          VarIntr(client_fd);//packet size
          VarIntr(client_fd);//packet id

          server_status(client_fd);
        // ... (tvoje stávající čtení prvního bufferu a odeslání JSONu) ...

        // --- 4. KROK: Odeslání REÁLNÉHO PONGU ---

        // Vytvoříme buffer pro odpověď
        std::vector<uint8_t> pong_packet;
        VarIntw(pong_packet, 0x01); // Packet ID pro Pong je 0x01

        // Pokusíme se vytáhnout reálný timestamp z receive_bufferu.
        // Ping paket má na konci 8 bajtů dat. Vezmeme posledních 8 bajtů z toho, co jsme přečetli.
        int bytes_read =  8;
        VarIntr(client_fd);
        VarIntr(client_fd);
        char receive_buffer[8];
        read(client_fd,&receive_buffer,8);

        if (bytes_read >= 8) {
            // Zkopírujeme posledních 8 bajtů z přijatých dat do našeho Pong paketu
            size_t timestamp_start = bytes_read - 8;
            for (size_t i = 0; i < 8; ++i) {
                pong_packet.push_back(receive_buffer[timestamp_start + i]);
            }
        } else {
            // Záloha: Pokud by data náhodou nepřišla celá, dáme tam nuly, aby server nespadl
            for (int i = 0; i < 8; i++) pong_packet.push_back(0x00);
        }

        // Zabalíme s délkou (Délka bude 9, protože ID = 1 bajt + 8 bajtů data)
        std::vector<uint8_t> final_pong;
        VarIntw(final_pong, pong_packet.size());
        final_pong.insert(final_pong.end(), pong_packet.begin(), pong_packet.end());

        // Odešleme klientovi
        write(client_fd, final_pong.data(), final_pong.size());

        std::cout << "Poslán status s reálným pingem z bufferu!" << std::endl;

        // Uzavření spojení
        close(client_fd);
        break;
}
    case 2:
    // --- Větev pro next_state == 2 (LOGIN) ---
std::cout << "Hráč se zkouší připojit. Posílám Disconnect..." << std::endl;

disconnect(client_fd);
//success(client_fd);
// 5. Okamžitě zavřeme spojení
close(client_fd);
    }
  }

    close(server_fd);
    return 0;
}
