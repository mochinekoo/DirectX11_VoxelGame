#include "Player.h"
#include "MochinekoEngine/DX2DManager.h"
#include "MochinekoEngine/InputManager.h"
#include "MessageManager.h"
#include <string>
#include "MochinekoEngine/ModelManager.h"
#include "MochinekoEngine/CameraManager.h"

using namespace DirectX;

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

	isThree_ = false;

	modelFBX_ = new FBX("playerTest.fbx");
	modelFBX_->Init();
	SetTransform(modelFBX_->GetTransform());

	SetLocation({0, 0.0, 0});

	// デフォルトでは1人称
	XMFLOAT3 playerLoc = transform_.location_;
	Camera* camera = CameraManager::GetCurrentCamera();
	camera->postion_ = {playerLoc.x, playerLoc.y + 2, playerLoc.z + 2};
	camera->target_ = {playerLoc.x, playerLoc.y, playerLoc.z + 5 };

	SetTag("mochineko");
}

void Player::Update() {
	modelFBX_->SetTransform(transform_);
	modelFBX_->UpdateTransform();
	modelFBX_->Update();

	const float speed = 1.0f;
	float deltaTime = MochinekoEngine::GetDeltaTime();

	Camera* camera = CameraManager::GetCurrentCamera();
	XMFLOAT3 playerLoc = transform_.location_;

	XMVECTOR forward = XMVectorSet(0, 0, 1, 0);
	XMMATRIX rotation = XMMatrixRotationY(transform_.rotation_.y);
	XMVECTOR direction = XMVector3Normalize(XMVector3TransformNormal(forward, rotation));
	XMVECTOR playerLocVec = XMLoadFloat3(&transform_.location_);
	XMVECTOR cameraLocVec = XMLoadFloat3(&camera->postion_);
	XMVECTOR cameraTargetVec = XMLoadFloat3(&camera->target_);

	XMVECTOR right = XMVectorSet(1, 0, 0, 0);
	XMVECTOR rightDirection = XMVector3Normalize(XMVector3TransformNormal(right, rotation));

	if (InputManager::CheckDownKey(DIK_F3)) {

	}


	if (InputManager::CheckDownKey(DIK_F5)) {
		isThree_ = !isThree_;

		if (isThree_) {
			// 3人称
			cameraLocVec = { playerLoc.x, playerLoc.y + 5, playerLoc.z - 5 };
			cameraTargetVec = { playerLoc.x, playerLoc.y, playerLoc.z };
		}
		else {
			// 1人称
			cameraLocVec = { playerLoc.x, playerLoc.y + 2, playerLoc.z + 2 };
			cameraTargetVec = { playerLoc.x, playerLoc.y, playerLoc.z + 5 };
		}
	}

	if (InputManager::CheckPushKey(DIK_W)) {
		playerLocVec += XMVectorScale(direction, speed * deltaTime);
		cameraLocVec += XMVectorScale(direction, speed * deltaTime);
		cameraTargetVec += XMVectorScale(direction, speed * deltaTime);
	}
	if (InputManager::CheckPushKey(DIK_S)) {
		playerLocVec -= XMVectorScale(direction, speed * deltaTime);
		cameraLocVec -= XMVectorScale(direction, speed * deltaTime);
		cameraTargetVec -= XMVectorScale(direction, speed * deltaTime);
	}
	if (InputManager::CheckPushKey(DIK_A)) {
		playerLocVec -= XMVectorScale(rightDirection, speed * deltaTime);
		cameraLocVec -= XMVectorScale(rightDirection, speed * deltaTime);
		cameraTargetVec -= XMVectorScale(rightDirection, speed * deltaTime);
	}
	if (InputManager::CheckPushKey(DIK_D)) {
		playerLocVec += XMVectorScale(rightDirection, speed * deltaTime);
		cameraLocVec += XMVectorScale(rightDirection, speed * deltaTime);
		cameraTargetVec += XMVectorScale(rightDirection, speed * deltaTime);
	}

	if (InputManager::CheckPushKey(DIK_LEFT)) {
		transform_.rotation_.y -= XMConvertToRadians(1.0f);
	}
	if (InputManager::CheckPushKey(DIK_RIGHT)) {
		transform_.rotation_.y += XMConvertToRadians(1.0f);
	}

	XMStoreFloat3(&transform_.location_, playerLocVec);
	XMStoreFloat3(&camera->postion_, cameraLocVec);
	XMStoreFloat3(&camera->target_, cameraTargetVec);

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
	modelFBX_->Draw();
	modelFBX_->DrawImGUI();

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
