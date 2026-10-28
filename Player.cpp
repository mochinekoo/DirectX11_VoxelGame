#include "Player.h"
#include "MochinekoEngine/DX2DManager.h"
#include "MochinekoEngine/InputManager.h"
#include "MessageManager.h"
#include <string>
#include "MochinekoEngine/ModelManager.h"
#include "MapManager.h"
#include "MochinekoEngine/CameraManager.h"
#include "MochinekoEngine/ImGUI/imgui.h"

using namespace DirectX;
using namespace MapManager;

namespace {
	const float CMD_BOX_HEIGHT = 50;
}

Player::Player()
	: BaseObject("Player") {
	cmdBox_ = nullptr;
	isCmdOpen_ = false;
	isJumping_ = false;
}

Player::~Player() {
}

void Player::Init() {
	cmdBox_ = new Box(1280, CMD_BOX_HEIGHT, { 0, 0, 0, 0.5 });
	cmdBox_->SetLocation({ 0, 720 - CMD_BOX_HEIGHT, 0 });
	cmdBox_->Init();

	isThree_ = false;

	modelFBX_ = new FBX("playerTest.fbx");
	modelFBX_->Init();
	SetTransform(modelFBX_->GetTransform());

	SetLocation({ 0, 0.0, 0 });

	// デフォルトでは1人称
	XMFLOAT3 playerLoc = transform_.location_;
	Camera* camera = CameraManager::GetCurrentCamera();
	camera->postion_ = { playerLoc.x, playerLoc.y + 2, playerLoc.z + 2 };
	camera->target_ = { playerLoc.x, playerLoc.y, playerLoc.z + 5 };

	SetTag("mochineko");
}

void Player::Update() {
	if (transform_.location_.y < MAX_WORLD_DEPTH) {
		transform_.location_ = { 0.0f, 5.0f, 0.0f };
		transform_.velocity_ = { 0.0f, 0.0f, 0.0f };
	}

	modelFBX_->SetTransform(transform_);
	modelFBX_->UpdateTransform();
	modelFBX_->Update();

	UpdateMoveKey();
	UpdateOtherKey();
}

void Player::Draw() {
	modelFBX_->Draw();

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

void Player::DrawImGUI() {
	auto pLoc = transform_.location_;
	BaseObject::DrawImGUI();

	ImGui::Begin("Player");
	ImGui::Text("PlayerLocHit: %d", IsHitBlock(pLoc.x, pLoc.y, pLoc.z));
	ImGui::Separator();
	ImGui::Text("Collision size");
	ImGui::SliderFloat("Height", &PLAYER_BODYHEIGHT, 0.1f, 10.0f, "%.2f");
	ImGui::SliderFloat("Half Width", &PLAYER_HALFWIDTH, 0.1f, 2.0f, "%.2f");
	ImGui::Text("Bottom: (%.2f, %.2f, %.2f)", pLoc.x, pLoc.y, pLoc.z);
	ImGui::Text("Top: (%.2f, %.2f, %.2f)", pLoc.x, pLoc.y + PLAYER_BODYHEIGHT, pLoc.z);
	ImGui::Text("%.2f x %.2f x %.2f", PLAYER_HALFWIDTH * 2.0f, PLAYER_BODYHEIGHT, PLAYER_HALFWIDTH * 2.0f);
	ImGui::Text("Block size: %.2f", DRAW_BLOCK_SIZE);
	ImGui::End();
}

void Player::UpdateMoveKey() {
	///
	/// 外部
	/// 
	float deltaTime = MochinekoEngine::GetDeltaTime();
	Camera* camera = CameraManager::GetCurrentCamera();

	///
	/// 変換
	/// 
	XMVECTOR playerLocVec = XMLoadFloat3(&transform_.location_);
	XMVECTOR cameraLocVec = XMLoadFloat3(&camera->postion_);
	XMVECTOR cameraTargetVec = XMLoadFloat3(&camera->target_);
	XMMATRIX rotationYMat = XMMatrixRotationY(transform_.rotation_.y);

	///
	/// 座標
	/// 
	XMVECTOR nextPlayerLocVec = playerLocVec;

	///
	/// 移動
	/// 
	XMVECTOR frontDirection = XMVector3Normalize(XMVector3TransformNormal(PLAYER_FRONT, rotationYMat));
	XMVECTOR frontMoveLoc = XMVectorScale(frontDirection, PLAYER_SPEED * deltaTime);
	XMVECTOR rightDirection = XMVector3Normalize(XMVector3TransformNormal(PLAYER_RIGHT, rotationYMat));
	XMVECTOR rightMoveLoc = XMVectorScale(rightDirection, PLAYER_SPEED * deltaTime);

	/// 移動
	if (InputManager::CheckPushKey(DIK_W)) {
		nextPlayerLocVec += frontMoveLoc;
		cameraLocVec += frontMoveLoc;
		cameraTargetVec += frontMoveLoc;
	}
	if (InputManager::CheckPushKey(DIK_S)) {
		nextPlayerLocVec -= frontMoveLoc;
		cameraLocVec -= frontMoveLoc;
		cameraTargetVec -= frontMoveLoc;
	}
	if (InputManager::CheckPushKey(DIK_A)) {
		nextPlayerLocVec -= rightMoveLoc;
		cameraLocVec -= rightMoveLoc;
		cameraTargetVec -= rightMoveLoc;
	}
	if (InputManager::CheckPushKey(DIK_D)) {
		nextPlayerLocVec += rightMoveLoc;
		cameraLocVec += rightMoveLoc;
		cameraTargetVec += rightMoveLoc;
	}


	/// ジャンプ
	if (InputManager::CheckDownKey(DIK_SPACE)) {
		isJumping_ = true;
		transform_.velocity_.y = PLAYER_JUMPPOWER;
	}
	if (isJumping_) {
		transform_.velocity_.y -= PLAYER_GRAVITY;
	}

	///
	/// 移動後の処理
	/// 
	float playerX = XMVectorGetX(playerLocVec), playerY = XMVectorGetY(playerLocVec), playerZ = XMVectorGetZ(playerLocVec);
	XMVECTOR subtractLocVec = XMVectorSubtract(nextPlayerLocVec, playerLocVec);
	float nextPlayerX = playerX + XMVectorGetX(subtractLocVec);
	float nextPlayerZ = playerZ + XMVectorGetZ(subtractLocVec);

	/// X軸とZ軸
	if (!IsHitBody(nextPlayerX, playerY, playerZ))
		playerX = nextPlayerX;
	if (!IsHitBody(playerX, playerY, nextPlayerZ))
		playerZ = nextPlayerZ;

	/// ジャンプ
	float nextPlayerY = playerY + transform_.velocity_.y;
	if (transform_.velocity_.y > 0) {
		if (IsHitBody(playerX, nextPlayerY, playerZ)) {
			transform_.velocity_.y = 0.0f;
			nextPlayerY = playerY;
		}
	}
	else {
		if (IsHitGround(playerX, nextPlayerY, playerZ)) {
			int iy = (int)floorf(nextPlayerY / DRAW_BLOCK_SIZE);
			nextPlayerY = ceil(iy * DRAW_BLOCK_SIZE);
			transform_.velocity_.y = 0;
			isJumping_ = false;
		}
		else {
			isJumping_ = true;
		}
	}
	playerY = nextPlayerY;


	/// カメラ更新
	if (!isThree_) {
		cameraLocVec = { playerX, playerY + 2, playerZ + 2 };
		cameraTargetVec = { playerX, playerY, playerZ + 5 };
	}
	else {
		cameraLocVec = { playerX, playerY + 8, playerZ - 5 };
		cameraTargetVec = { playerX, playerY, playerZ };
	}

	///
	/// 回転
	/// 

	if (InputManager::CheckPushKey(DIK_LEFT)) {
		transform_.rotation_.y -= PLAYER_ROTATION_SPEED;
	}
	if (InputManager::CheckPushKey(DIK_RIGHT)) {
		transform_.rotation_.y += PLAYER_ROTATION_SPEED;
	}

	playerLocVec = XMVectorSet(playerX, playerY, playerZ, 1.0f);
	XMStoreFloat3(&transform_.location_, playerLocVec);
	XMStoreFloat3(&camera->postion_, cameraLocVec);
	XMStoreFloat3(&camera->target_, cameraTargetVec);
}

void Player::UpdateOtherKey() {
	///
	/// カメラの視点切り替え
	/// 
	if (InputManager::CheckDownKey(DIK_F5)) {
		isThree_ = !isThree_;
	}

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

bool Player::IsHitBody(float x, float y, float z) {
	for (float dx : { -PLAYER_HALFWIDTH, PLAYER_HALFWIDTH }) {
		for (float dz : { -PLAYER_HALFWIDTH, PLAYER_HALFWIDTH }) {
			if (MapManager::IsHitBlock(x + dx, y + PLAYER_BODYHEIGHT * 0.5, z + dz)) return true;
			if (MapManager::IsHitBlock(x + dx, y + PLAYER_BODYHEIGHT, z + dz)) { 
				return true; 
			}
		}
	}
	return false;
}

bool Player::IsHitGround(float x, float y, float z) {
	for (float dx : { -PLAYER_HALFWIDTH, PLAYER_HALFWIDTH }) {
		for (float dz : { -PLAYER_HALFWIDTH, PLAYER_HALFWIDTH }) {
			if (MapManager::IsHitBlock(x + dx, y - 0.01f, z + dz)) return true;
		}
	}
	return false;
}
