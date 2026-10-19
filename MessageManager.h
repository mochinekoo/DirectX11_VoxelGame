#pragma once
#include "Player.h"
#include <string>

namespace MessageManager {

	void Init();
	void Draw();

	void AddMessage(Player* player, std::string message);
}