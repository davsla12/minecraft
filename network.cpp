#include "network.h"
#include "users.h"
#include "mapping.h"

#include <iostream>
#include <cstring>

#define PORT 25565

int server_fd;

void Network_Init(){
  // 1. Vytvoření socketu typu AF_INET (IPv4 internetový protokol) a SOCK_STREAM (TCP)
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    std::cerr << "Chyba při vytváření socketu!" << std::endl;
    exit(1);
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
    exit(1);
  }

  // 4. Listen - začátek naslouchání pro příchozí TCP spojení
  if (listen(server_fd, 5) == -1) {
    std::cerr << "Chyba při listen!" << std::endl;
    close(server_fd);
    exit(1);
  }

  std::cout << "Status server spusten pres sys/socket na TCP portu: " << PORT << "\nCekam na Minecraft klienta..." << std::endl;

}

void Network_Run(){
  std::vector<pollfd> fd_s;
  pollfd server_pollfd;
  server_pollfd.fd = server_fd;
  server_pollfd.events = POLLIN; // Sledujeme příchozí připojení
  server_pollfd.revents = 0;
  fd_s.push_back(server_pollfd);
    int fd_r;
    char buffer[1024];
  while ((fd_r = poll(fd_s.data(), fd_s.size(), 100)) >= 0) {
      if (fd_r == 0) {
          // Timeout - nikdo nic neposílá, můžeš pokračovat dál
          continue;
      }

      // Procházíme všechny sledované deskriptory od konce na začátek
      // (od konce proto, abychom mohli bezpečně mazat odpojené klienty)
      for (size_t i = 0; i < fd_s.size(); ++i) {

          // Zkontrolujeme, zda na tomto socketu nastala událost POLLIN
          if (fd_s[i].revents & POLLIN) {

              // PŘÍPAD A: Událost na serverovém socketu -> Nový klient se připojuje
              if (fd_s[i].fd == server_fd) {
                  int client_fd = accept(server_fd, NULL, NULL);
                  if (client_fd >= 0) {
                      std::cout << "Nový klient připojen! FD: " << client_fd << std::endl;

                      // Přidáme nového klienta do vektoru pro příští volání poll()
                      pollfd client_pollfd;
                      client_pollfd.fd = client_fd;
                      client_pollfd.events = POLLIN; // Sledujeme data od klienta
                      client_pollfd.revents = 0;
                      fd_s.push_back(client_pollfd);
                  }
              }

              // PŘÍPAD B: Událost na klientském socketu -> Klient posílá data
              else {
                  int bytes_received = recv(fd_s[i].fd, buffer, sizeof(buffer) - 1, 0);

                  if (bytes_received <= 0) {
                      // Klient se odpojil nebo nastala chyba
                      std::cout << "Klient na FD " << fd_s[i].fd << " se odpojil." << std::endl;
                      close(fd_s[i].fd);

                      // Vymažeme klienta z vektoru
                      fd_s.erase(fd_s.begin() + i);
                      --i; // Upravíme index, protože jsme smazali prvek
                  } else {
                      // Úspěšně přijatá data
                      buffer[bytes_received] = '\0';
                      //std::cout << "Klient [" << fd_s[i].fd << "] píše: " << buffer << std::endl;
                      packet_r(fd_s[i].fd);
                      // Zde můžeš klientovi odpovědět pomocí send()
                      // send(fd_s[i].fd, "OK\n", 3, 0);
                  }
              }
          }
      }
  }

  close(server_fd);
}
