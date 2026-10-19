#include "MessageManager.h"
#include "Player.h"
#include <string>
#include "MochinekoEngine/MathUtil.h"
#include "MochinekoEngine/DX2DManager.h"

struct MessageData {
	Player* player_ = nullptr;
	std::string message_ = "";
	bool isCmd_ = false;
};

namespace {
	std::vector<MessageData> messageDataList_;
}

void MessageManager::Init() {
	messageDataList_.clear();
}

void MessageManager::Draw() {
	const int drawCount = 5;
	static bool lastDraw = false;
	static int beforeSize = messageDataList_.size();
	int afterSize = messageDataList_.size();
	bool sizeChanged = beforeSize != afterSize;

	if (sizeChanged) {
		lastDraw = true;
	}

	//DX2DManager::DrawFontText(10, 10, "aa", {1.0f, 0.0f, 0.0f, 0.5f});
	static float count = drawCount;
	if (lastDraw) {
		auto& messageData = messageDataList_[afterSize - 1];
		Player* player = messageData.player_;
		std::string playerName = player->GetTag();
		float rate = 1.0 - (count / drawCount);

		float opacity = MathUtil::Lerp(1.0, 0.0, rate, MathUtil::easeInCubic);
		if (messageData.isCmd_ || messageData.message_.empty() || count <= 0) {
			lastDraw = false;
			count = drawCount;
		}
		DX2DManager::DrawFontText(10, 10, "<" + playerName + ">:" + messageData.message_, {1.0f, 0.0f, 0.0f, opacity});

		count -= MochinekoEngine::GetDeltaTime();
	}

	beforeSize = afterSize;
}

void MessageManager::AddMessage(Player* player, std::string message) {
	MessageData messageData = MessageData();
	messageData.player_ = player;
	messageData.isCmd_ = message.starts_with("/");
	if (messageData.isCmd_) {
		messageData.message_ = message.erase(0, 1);
	}
	else {
		messageData.message_ = message;
	}
	messageDataList_.push_back(messageData);
}
