#include "Player.h"
#include "MochinekoEngine/DX2DManager.h"
#include "MochinekoEngine/InputManager.h"
#include "MessageManager.h"
#include <string>

namespace {
	const float CMD_BOX_HEIGHT = 50;
}

Player::Player()
	: BaseObject("Player") {
	cmdBox_ = nullptr;
	isCmdOpen_ = false;
}

Player::~Player() {
}

void Player::Init() {
	cmdBox_ = new Box(1280, CMD_BOX_HEIGHT, {0, 0, 0, 0.5});
	cmdBox_->SetLocation({0, 720 - CMD_BOX_HEIGHT, 0});
	cmdBox_->Init();

	SetTag("mochineko");
}

void Player::Update() {

	// UI描画
	if (cmdBox_ != nullptr) {
		if (!isCmdOpen_) {
			InputManager::DestoryKeyBuffer();
		}

		if (InputManager::CheckDownKey(DIK_ESCAPE)) {
			isCmdOpen_ = !isCmdOpen_;
		}
		if (InputManager::CheckDownKey(DIK_RETURN)) {
			std::string keyBuffer = InputManager::GetLowKeyBuffer();
			if (keyBuffer.starts_with("/")) {

			}
			else {
				MessageManager::AddMessage(this, keyBuffer);
			}
			isCmdOpen_ = false;
		}

		cmdBox_->UpdateTransform();
		cmdBox_->Update();
	}
}

void Player::Draw() {

	// UI描画
	if (cmdBox_ != nullptr) {
		if (!isCmdOpen_) return;

		std::string keyBuffer = InputManager::GetLowKeyBuffer();
		cmdBox_->Draw();
		const float drawX = 10;
		const float drawY = (720 / 2 - CMD_BOX_HEIGHT) + (CMD_BOX_HEIGHT / 2);
		DX2DManager::DrawFontText(drawX, drawY, keyBuffer, Color::Black());
	}
}

void Player::Release() {
}
