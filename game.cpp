#include "game.h"
#include "users.h"

#include <chrono>
#include <thread>
#include <iostream>

std::mutex Eventb_mtx;
bool Eventb = true;

std::mutex Event1_mtx;
std::queue<Packet> Event1;

std::mutex Event2_mtx;
std::queue<Packet> Event2;

std::queue<Packet>* Events;

std::chrono::milliseconds TICK_TIME(50);

int Game_AddEvent(Packet* packet){

  return 0;
}

void Game_tick(){
  Eventb_mtx.lock();
  Eventb = !Eventb;
  Eventb_mtx.unlock();
  if(Eventb){
    Event1_mtx.lock();
    Events = &Event1;
  }
  else{
    Event2_mtx.lock();
    Events = &Event2;
  }

  while(!Events->empty()){
    Packet event = Events->front();
    Events->pop();
    User user;
    Users_getfd(&user,event.fd);
    switch(user.state){

    }
  }

  if(Eventb){
    Event1_mtx.unlock();
  }
  else{
    Event2_mtx.unlock();
  }
}

void Game_Run(){
    while (true) { // nebo nějaká globální proměnná running
        // Zaznamenáme čas na začátku ticku
        auto tick_start = std::chrono::steady_clock::now();

        // Spustíme samotnou herní logiku
        Game_tick();

        // Zaznamenáme čas po skončení logiky
        auto tick_end = std::chrono::steady_clock::now();

        // Spočítáme, jak dlouho ta logika reálně trvala
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(tick_end - tick_start);

        // Pokud logika trvala méně než 50 ms, dopočítáme zbytek a vlákno uspíme
        if (elapsed_time < TICK_TIME) {
            auto sleep_time = TICK_TIME - elapsed_time;
            std::this_thread::sleep_for(sleep_time);
        } else {
            // Pokud logika trvala déle než 50 ms, server nestíhá (laguje)
            // V tom případě neuspáváme a hned pokračujeme dalším tickem
            std::cout << "Server laguje! Tick trval " << elapsed_time.count() << " ms místo 50 ms.\n";
        }
    }
}
