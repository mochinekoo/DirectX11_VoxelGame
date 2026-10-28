#pragma once
#include "MochinekoEngine/BaseObject.h"
#include "MochinekoEngine/Box.h"
#include "MochinekoEngine/FBX.h"
#include <DirectXMath.h>

namespace {
	const float PLAYER_JUMPPOWER = 0.6f;
	const float PLAYER_GRAVITY = 0.04f;
	float PLAYER_BODYHEIGHT = 4.5f;
	const float PLAYER_SPEED = 1.0f;
	float PLAYER_HALFWIDTH = 0.925f;

	const float PLAYER_ROTATION_SPEED = DirectX::XMConvertToRadians(1.0f);

	const DirectX::XMVECTOR PLAYER_FRONT = { 0, 0, 1, 1 };
	const DirectX::XMVECTOR PLAYER_RIGHT = { 1, 0, 0, 1 };
}

class Player : public BaseObject{
private:
	Box* cmdBox_;
	bool isCmdOpen_;
	FBX* modelFBX_;
	bool isThree_;

	bool isJumping_;
public:

	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	void UpdateMoveKey();
	void UpdateOtherKey();

	bool IsHitBody(float x, float y, float z);
	bool IsHitHead(float x, float y, float z);
	bool IsHitGround(float x, float y, float z);
};

