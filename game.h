#ifndef GAME_H
#define GAME_H

#include "protocol.h"

#include <queue>
#include <mutex>

void Game_Run();
int Game_AddEvent(Packet packet);

#endif
